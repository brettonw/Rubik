//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/17/97
//
//	02/21/97	DS		added gCameraSlew (camera slew vector)
//									added gDebugString (debug string)
//									modified adaptive timestep with different weighting (patch)

//------------------------------------------------------------------------------

#ifndef	VIEW
#define	VIEW

//------------------------------------------------------------------------------
#include "sequencer.h"
#include "timer.h"

//------------------------------------------------------------------------------
enum	{PIECE_LAYER = -1, HUD_LAYER_1 = -2, HUD_LAYER_2 = -3, UNDER_LAYER = -4, OVER_LAYER = -5};	//	layer codes for the installer

//------------------------------------------------------------------------------
#define	FRAME_SMOOTHING_WINDOW	4																							//	how many frame sto smooth the frame time over

//------------------------------------------------------------------------------
class View																																			//	the view of the world
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Sequencer	*sequencer;																										//	pointer to an array of sequencers
				int				numLayers;																										//	the number of layers in the view
				int				pieceLayer;																										//	the layer to draw the pieces on
				Timer			timer;																												//	timer for the frame rate
				ulong			frameCount;																										//	the frame count
				double		frames[FRAME_SMOOTHING_WINDOW];
				double		totalTime;
				Real			maxFrameInterval;
	public:																																				//	members available externally
				View (void);																														//	constructor
virtual	~View (void);																														//	destructor
				void			Draw (CPoint where);																					//	draw the view
				void			Install (const SeqPtr &sequence, int layer = PIECE_LAYER);		//	add an effect to the view
};																																							//	end

//------------------------------------------------------------------------------
extern	View	*gView;																														//	global view
extern	Vector_2d	gCameraSlew;																									//	global slew vector (for moving camera relative to ship)
extern	char	gDebugString[255];																								//	global debug string
extern	Bool	gCaptureFrame;
//------------------------------------------------------------------------------

#endif	//	VIEW