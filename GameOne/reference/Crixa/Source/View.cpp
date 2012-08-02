//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/17/97
//
//	02/21/97	DS		added gCameraSlew (camera slew vector)
//									added gDebugString (debug string)
//									modified Draw() for MY adaptive timestep (temp patch)
//									modified Draw() to draw gDebugString
//	02/25/97	DS		added gCaptureFrame global flag
//	03/05/97	DS		modified adaptive timestep wait
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "view.h"
#include "sprite array mgr.h"
#include "level file.h"
#include "text buffer.h"
#include "display.h"
#include "sprite sequence.h"
#include "stationary sequence.h"
#include "reporter.h"
#include "get parameters.h"
#include "star sequence.h"
#include "partition sequence.h"
#include "random.h"
#include "network play.h"
#include "debugger.h"

//------------------------------------------------------------------------------
View	*gView = 0;																																//	global view
Vector_2d gCameraSlew(R(0.0),R(0.0));																						//	global camera slew vector (for moving camera around)
char	gDebugString[255] = "";																										//	global debug string
Bool	gCaptureFrame = FALSE;

//------------------------------------------------------------------------------
View::View (void)																																//	constructor
{																																								//	begin
	LevFile		levVis ("World");
	TxtBuffer	txt (&levVis);																											//	load the visual elements file
	int				width, 
						height;
	cstr			line;																																//	place to store a string
	txt >> width >> height >> numLayers >> pieceLayer;														//	read the dimensions, number of layers, and the piece layer
	sequencer = new Sequencer[numLayers + 2];																			//	allocate the sequencers plus an additional two on top for HUD effects
	for (int i = 0; i < numLayers; i++)																						//	loop over all of the layers
	{
		Real	parallax;
		txt >> parallax >> line;
		sequencer[i].SetParallax (parallax);
		if (strcmp (line, "stars") == 0)
		{
			Real	density;
			txt >> density;
			StarSeq	*stars = new StarSeq (Real (width), Real (height), parallax, density, density * R(1.0e-2));
			sequencer[i].Install (stars);
		}
	}

	txt >> line;																																//	read a keyword
	while (strcmp (line, "end"))
	{
		int			layer = atoi (line);
		SeqPtr	seq;
		Real		x, y;																															//	place to get coordinates of a visual
		txt >> line;
		if (strcmp (line, "group") == 0)
		{
			seq = new PartitionSeq (width, height);
			txt >> line;
			while (strcmp (line, "end"))
			{
				txt >> x >> y;																													//	get the coordinates
				SeqPtr	spr = new SpriteSeq (gSpriteMgr.Fetch (line));									//	get the sprite sequence that accompanies that visual
				((PartitionSeq *) seq ())->Install (new StatSeq (Point_2d (x, y), spr));//	install the sequence
				txt >> line;
			}
		}
		else
		{
			txt >> x >> y;																														//	get the coordinates
			SeqPtr	spr = new SpriteSeq (gSpriteMgr.Fetch (line));										//	get the sprite sequence that accompanies that visual
			seq = new StatSeq (Point_2d (x, y), spr);																	//	create the stationary sequence
		}
		sequencer[layer].Install (seq);
		txt >> line;																																//	read a keyword
	}
	numLayers++;																																	//	increment the number of layers to include the HUD layers

	frameCount = 0;																																//	zero the frame counter
	totalTime = 0.0;
	for (i = 0; i < FRAME_SMOOTHING_WINDOW; i++)
		frames[i] = 0.0;
	maxFrameInterval = ParmReal (MAXIMUM_FRAME_INTERVAL);
	timer.Start ();																																//	start the interval timer
}																																								//	end

//------------------------------------------------------------------------------
View::~View (void)																															//	destructor
{																																								//	begin
	//DEBUG_STR ("\nView: Average Framerate over FRAME_SMOOTHING WINDOW: %f\n", FRAME_SMOOTHING_WINDOW / totalTime);
	if (totalTime > ZERO)
	DEBUG_STR ("\nView: Average Framerate: %f\n", frameCount / totalTime);
	delete[] sequencer;																														//	release the sequencer array
}																																								//	end

//------------------------------------------------------------------------------
void	View::Draw (CPoint clocation)																							//	draw the view
{																																								//	begin
		while (timer.Elapsed () < (gDisplay->Rate () * R(0.95))) {}									//	delay until the next frame time is here
//	while (timer.Elapsed () < (gReferee->Interval () * R(0.5))) {}							//	ds: delay tweakin'

	if (!gClient)																																	//	if not network playing
	{
		if (1)	// dave-style adaptive timestep (gameplay more adaptive, but jumpy framerate)
		{
			frames[0]= timer.Interval();
			Real newFrameInterval = Real((frames[0]*4 + frames[1]*2 + frames[2]) * (1.0 / 7.0));
			if (newFrameInterval > maxFrameInterval) 
				newFrameInterval = maxFrameInterval;
			gReferee->SetInterval (newFrameInterval);
			frames[2]=frames[1];
			frames[1]=frames[0];
			totalTime += frames[0];
			frameCount++;
		}
		else		// bretton-style adaptive timestep (framerate more consistent, but gameplay chuggy)
		{
			int		index = ++frameCount & (FRAME_SMOOTHING_WINDOW - 1);
			totalTime -= frames[index];
			frames[index] = timer.Interval ();
			if (frames[index] > maxFrameInterval)
				frames[index] = maxFrameInterval;
			totalTime += frames[index];
			if (frameCount > FRAME_SMOOTHING_WINDOW)
			{
				Real	newFrameInterval = Real (totalTime * (1.0 / FRAME_SMOOTHING_WINDOW));
				gReferee->SetInterval (newFrameInterval);
			}	
		}
	} // end !gClient

	gDisplay->Swap ();																														//	swap the surfaces

	CPoint location = clocation + gCameraSlew;																		//	ds: calculate slew

	if (ParmInteger (CAPTURE_FRAMES) || gCaptureFrame)														//	if the frame capture option is on
	{																																							//	begin
		char	filename[256];																												//	place to build the capture file name
		sprintf (filename, "%s%03d.raw", ParmString (CAPTURE_NAME), frameCount);		//	build the capture file name
		gDisplay->DumpToFile (filename);																						//	dump the screen to a file
		gCaptureFrame = FALSE;
	}																																							//	end
	gDisplay->Erase ();																														//	erase the display
	gDisplay->Lock ();																														//	lock the display
	for (int curseq = 0; curseq < pieceLayer; curseq++)
		sequencer[curseq].Step (location);																					//	draw each one in turn
	Reporter	reporter (gReferee);																								//	prepare to access the pieces
	PiecePtr	piece = reporter.GetPiece ();																				//	get the first piece
	Point_2d	pLoc = location - gCenter;																					//	compute the drawing offset for pieces
	pLoc.Floor ();																																//	flatten the tuple
	while (piece ())																															//	loop over all of the pieces
	{																																							//	begin
		piece->Draw (pLoc);																													//	draw the piece
		reporter.Next ();																														//	advance to the next piece
		piece = reporter.GetPiece ();																								//	grab a pointer to the piece
	}																																							//	end
	for (; curseq < numLayers; curseq++)
		sequencer[curseq].Step (location);																					//	draw each one in turn
	sequencer[numLayers].Step (Point_2d (ZERO, ZERO));														//	draw the top HUD layer

	// ds debug string
	SArrayPtr font = gSpriteMgr.Fetch("text7bg");	
	int x = 10; int idx;
	for (int i = 0; gDebugString[i]; i++)
	{
		if ((idx=gDebugString[i])>32)																								//	ignore control characters
		{
			font[idx]->Draw (Point_2d ( Real (x), Real (17)), FALSE);									//	draw the string up to the current point
			x += font[gDebugString[i]]->Width();
		}
		else
			x += 4;
	}

	gDisplay->Unlock ();																													//	unlock the display
}																																								//	end

//------------------------------------------------------------------------------
void	View::Install (const SeqPtr &sequence, int layer)													//	add an effect to the view
{																																								//	begin
	switch (layer)
	{
		case PIECE_LAYER:
			layer = pieceLayer;																												//	set it to the piece layer
			break;																																		//	end case
		case HUD_LAYER_1:
			layer = numLayers;																												//	set the layer to the top layer
			break;																																		//	end case
		case HUD_LAYER_2:
			layer = numLayers - 1;																										//	set the layer to the nearly top layer
			break;																																		//	end case
		case UNDER_LAYER:
			layer = pieceLayer - 1;																										//	set it to the layer under the piece layer
			break;																																		//	end case
		case OVER_LAYER:
			layer = pieceLayer + 1;																										//	set it to the layer over the piece layer
			break;																																		//	end case
	}
	sequencer[layer].Install (sequence);																					//	insert the sequence
}																																								//	end

//------------------------------------------------------------------------------
