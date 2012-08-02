//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	02/03/97
//
//  02/19/97	MK		Moved Brief() call to before level load
//	02/19/97	DS		Added State Manager New,Step & Delete calls
//	02/27/97	DS		Moved Referee creation/destruction up a level to
//									PlayController. Added Level Status() report call.
//									Fixed logic for "end of game if no more lives"
//	03/04/97	DS		Extra Life Logic added to DeBrief
//						DS		Improved text effects
//	03/07/97	DS		Removed status text mgr, repurposed mission text mgr
//									Added CD stuff to Trigger(), Brief(), DeBrief()
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "level.h"
#include "sprite array mgr.h"
#include "display.h"
#include "world.h"
#include "floor trigger.h"
#include "trigger array.h"
#include "view.h"
#include "pi.h"
#include "get parameters.h"
#include "independent.h"
#include "referee.h"
#include "eksekaren.h"
#include "shipyard.h"
#include "local human.h"
#include "text mgr.h"
#include "text sequence.h"
#include "tracking sequence.h"
#include "format text.h"
#include "stationary sequence.h"
#include "level file.h"
#include "network play.h"
#include "random.h"
#include "text buffer.h"
#include "crixa ship.h"
#include "debugger.h"
#include "sound mgr.h"
#include "state mgr.h"
#include "resource.h"
#include "timer.h"																															//	used to maintain rate


//------------------------------------------------------------------------------
#define	TEXTALERTSOUND 35

#define ADVANCE_KEY		VK_RETURN
#define	START_KEY			VK_ESCAPE

//------------------------------------------------------------------------------
Level::Level (int level) : Device (0)																						//	constructor
{																																								//	begin
	char levelName[256];
	if (gClient)
		sprintf (levelName, "Network%02d.dat", level);
	else
		sprintf (levelName, "Level%02d.dat", level);
	LevFile::OpenLevel (levelName);

	levelstatus = LV_INIT;

	DEBUG_STR("\n=== INITIALIZING LEVEL ===\n");

	gView = new View;

	SArrayPtr	dfont = gSpriteMgr.Fetch ("scaps7bg"),
						dfontbold = gSpriteMgr.Fetch ("scaps7bb");
	description = new TextMgr (dfont, dfontbold, "Descriptions", 35, TXM_SIMPLE);
	mission = new TextMgr (dfont, dfontbold, "Missions", 35, TXM_LOOPQ | TXM_QUEUE);
	dfont = gSpriteMgr.Fetch ("scaps5bgs");
	dfontbold = gSpriteMgr.Fetch ("scaps5bbs");
	playerstatus = new TextMgr (dfont, dfontbold, 35, TXM_RETRIG);

	Real	left = Real (-(SCRN_HEIGHT >> 1) + 40);
	gView->Install (new StatSeq (Point_2d (left, -left + R(20.0)), mission), HUD_LAYER_1);
	gView->Install (new StatSeq (Point_2d (left, R(-100.0)), description), HUD_LAYER_1);
	gView->Install (new StatSeq (Point_2d (R(30.0), ZERO), playerstatus), HUD_LAYER_1);

	gWorld = new World;
	gReferee->Install (gWorld, FULL_INTERSECT | GLOBAL_INTERSECT);
	gTrigger = new TArray (256);
	gTrigger->SetTrigger (1, this);																								// trigger 1 is reserved for level
	gFloor = new TFloor;
	
	gStateMgr = new StateMgr;																											// ds: declare new state manager
	
	gIndependent = new Independent;
	
	Brief();																																			// (MK), moved from end of function

	gShipyard = new Shipyard;
	if (gNumPlayers == 1)
		gEksekaren	= new Eksekaren;

	for (int i = 0; i < gNumPlayers; i++)
		gPlayers[i]->Reset ();

	gSpriteMgr.CleanUp ();																												//	erase all da sprites

	levelstatus = LV_PLAYING;
	playing = TRUE;
	gRandom.SetIndex (0);
	
}																																								//	end

//------------------------------------------------------------------------------
int	Level::Status (void)																												//	return the status of the level
{
	if (gLocal->GetLives() == 0)
		levelstatus |= LV_NO_MORE_LIVES;
	if (strcmp (gStateMgr->CurrentStateName(),"LevelWin")==0)
		levelstatus |= LV_MISSION_COMPLETE;
	return levelstatus;
}
//------------------------------------------------------------------------------
Level::~Level (void)																														//	destructor
{																																								//	begin
	Debrief ();
	mission = SeqPtr (0);
	description = SeqPtr (0);
	delete gIndependent;
	delete gShipyard;
	if (gNumPlayers == 1)
		delete gEksekaren;
	delete gTrigger;
	delete gFloor;
	delete gStateMgr;																															//  DS: Delete state manager
	delete gView;
	gSpriteMgr.UnTouch ();
	LevFile::CloseLevel ();
}																																								//	end

//------------------------------------------------------------------------------
int		Level::Trigger (const PiecePtr &piece, int type, int what)								//	trigger the device
{																																								//	begin
	switch (type)
	{
		case DEV_END_LEVEL:
			if (piece->GetOwner () == gLocal)
				if (piece () == gLocal->GetCrixa () ())
				{
					playing = FALSE;																											//	level is done!
					levelstatus |= LV_EXITED;																							//	so update our level status
				}
			break;
		case DEV_PRIVATE_MESSAGE:																										//	unused
			if (piece () != gLocal->GetCrixa () ())
			break;
		case DEV_PUBLIC_MESSAGE:																										//	Print a description
			((TextMgr *) description ())->Message (what);
			break;
		case DEV_MISSION_MESSAGE:																										
			if (what)
				((TextMgr *) mission ())->Message (what);																//	we accept any mission message
			else
				((TextMgr *) mission ())->ClearQueue ();																//	we accept any mission message
			break;
		case DEV_SHIP_MESSAGE:
			((TextMgr *) playerstatus ())->Message (cstr (what));
			break;
		case DEV_TEXT_MESSAGE:
			((TextMgr *) description ())->Message (cstr (what));
			break;
		case DEV_CDPLAYTRACK:																												//	temporary CD control
			gSoundMgr->StopCD();																											//	stop the CD so PlayCD works
			if (what)																																	//	if nonzero, play a track
				gSoundMgr->PlayCDTrack (what);
			break;
		case DEV_PLAYSOUND:																													//	temporary sound playing
			gSoundMgr->PlaySound (what);
			break;
	}
	return 0;
}																																								//	end

//------------------------------------------------------------------------------
Bool	Level::Play (void)																												//	play a frame
{																																								//	begin
	gStateMgr->Step ();
	gView->Draw (gLocal->Camera ());
	gReferee->Step ();
	gSoundMgr->Step ();
	DoNetwork (gLocal->PrePlay ());
	gIndependent->Play ();
	gShipyard->Play ();
	if (gNumPlayers == 1)
	{
		gEksekaren->SetTarget (gLocal->GetCrixa ());
	//	if (gRandom.RndInteger() > 512)
			gEksekaren->Play ();
	}
	return playing;
}																																								//	end

//------------------------------------------------------------------------------
void	ClearQueue (uint types)																										//	clear the event queue of the specified event types
{																																								//	begin
	while (GetQueueStatus (types))
	{
		MSG	msg;
		GetMessage (&msg, 0, 0, 0);
	}
}																																								//	end

//------------------------------------------------------------------------------
void	Level::Brief (void)																												//	brief the player on the mission
{																																								//	begin
	Timer ratelimiter;																														//	use this to limit flip rate (and avoid white flash)
	LevFile		lev ("Brief");																											//	get the level brief file
	TxtBuffer txt (&lev);																													//	load the brief file
	cstr			animation;																													//	place to get the animation name
	txt >> animation;																															//	get the briefing info
	SArrayPtr	brief = gSpriteMgr.Fetch ("brief", FALSE),													//	load the image sprite
						transign = gSpriteMgr.Fetch ("transign", FALSE);
	int				left = gDisplay->PlayRect ().right - 480,
						bottom = gDisplay->PlayRect().bottom - 480;
	Bool			showBriefing = TRUE;

	while (showBriefing) 
	{
		while (strcmp (animation, "end"))
		{
			RECT	r2;
			SetRect (&r2, left + 495, 10, left + 626, 140);
			gDisplay->EraseRect (r2);
			gDisplay->Erase ();
			gDisplay->Lock ();																												//	lock the display
			brief[0]->DrawUnclipped (left, bottom);																		//	draw the brief background
			transign[0]->DrawUnclipped (left + 513, bottom + 365);										//	draw the transmission signal background
			gDisplay->Unlock ();																											//	unlock the display
			gDisplay->Swap ();																												//	swap the surfaces
			gDisplay->EraseRect (r2);
			gDisplay->Erase ();
			gDisplay->Lock ();																												//	lock the display
			brief[0]->DrawUnclipped (left, bottom);																		//	draw the brief background
			transign[0]->DrawUnclipped (left + 513, bottom + 365);										//	draw the transmission signal background
			gDisplay->Unlock ();																											//	unlock the display
			gDisplay->Swap ();																												//	swap the surfaces
			cstr	briefing;
			txt >> briefing;
			FormatText (briefing);
			SArrayPtr	briefanim = gSpriteMgr.Fetch (animation, FALSE),
								font = gSpriteMgr.Fetch ("text7bg"),
								boldfont = gSpriteMgr.Fetch ("text7bb");
			int				count = briefanim->Count (),
								index = 0;
 			TextSeq		seq (font, boldfont, briefing, 150, INT_MAX);										//	text sequence
			Point_2d	zeroPt (left, gDisplay->PlayRect ().bottom - 480),
								whereA = zeroPt + Vector_2d (R(138.0), R(220.0)),
								whereT = zeroPt + Vector_2d (R(270.0), R(360.0));

			while (GetAsyncKeyState (START_KEY)) {}																		//	try to trap keyboard
			while (GetAsyncKeyState (ADVANCE_KEY)) {}																	//	try to trap keyboard

			gSoundMgr->StopCD();																											//	play the music (dih)
			//gSoundMgr->PlayCDTrack (ParmInteger (CD_BRIEF_MUSIC));

			while ((!GetAsyncKeyState (ADVANCE_KEY)) && showBriefing)
			{
				ratelimiter.Start();																										//	keep flip rate around 30fps
				while (ratelimiter.Elapsed() < R(0.03333)) {}														//	some machines will mess up display if flipped too fast
				if (GetAsyncKeyState (START_KEY))
					showBriefing=FALSE;
				RECT	r;
				SetRect (&r, left + 30, 100, left + 245, 420);
				gDisplay->EraseRect (r);
				gDisplay->Lock ();																											//	lock the display
				briefanim[index]->Draw (whereA);																				//	draw the brief background
				seq.Step (whereT);
				index = (index + 1) % count;
				gDisplay->Unlock ();																										//	unlock the display
				gDisplay->Swap ();																											//	swap the surfaces
			}
			txt >> animation;
			while (GetAsyncKeyState (ADVANCE_KEY)) {};
		}
		txt.Reset();
		txt >> animation;																														//	get the briefing info
		while (GetAsyncKeyState (ADVANCE_KEY)) {};
	}
	gSoundMgr->PlaySound(49);																											//	confirming tone
	//gSoundMgr->StopCD();

	while (GetAsyncKeyState (START_KEY)) {}																				//	try to trap keyboard
	ClearQueue (QS_KEY);
}																																								//	end

//------------------------------------------------------------------------------
void	Level::Debrief (void)																											//	tell the player how they did
{																																								//	begin
	int				left = gDisplay->PlayRect ().right - 480,
						bottom = gDisplay->PlayRect().bottom - 480;
	Timer			ratelimiter;
	if (1)
	{
		SArrayPtr	debrief = gSpriteMgr.Fetch ("debrief", FALSE),										//	load the image sprite
							transign = gSpriteMgr.Fetch ("transign", FALSE);
		RECT			r2;
		SetRect (&r2, left + 495, 10, left + 626, 140);
		gDisplay->EraseRect (r2);
		gDisplay->Erase ();
		gDisplay->Lock ();																													//	lock the display
		debrief[0]->DrawUnclipped (left, bottom);																		//	draw the debrief background
		transign[0]->DrawUnclipped (left + 513, bottom + 365);											//	draw the transmission signal background
		gDisplay->Unlock ();																												//	unlock the display
		gDisplay->Swap ();																													//	swap the surfaces
		gDisplay->EraseRect (r2);
		gDisplay->Erase ();
		gDisplay->Lock ();																													//	lock the display
		debrief[0]->DrawUnclipped (left, bottom);																		//	draw the debrief background
		transign[0]->DrawUnclipped (left + 513, bottom + 365);											//	draw the transmission signal background
		gDisplay->Unlock ();																												//	unlock the display
		gDisplay->Swap ();																													//	swap the surfaces
	}
	SArrayPtr	font = gSpriteMgr.Fetch ("text7bg"),
						boldfont = gSpriteMgr.Fetch ("text7bg");
	Point_2d	zeroPt (left, gDisplay->PlayRect ().bottom - 480),
						where[6];
	char			text[6][256];
	where[0] = zeroPt + Vector_2d (R(70.0), R(377.0));
	where[1] = zeroPt + Vector_2d (R(70.0), R(345.0));
	where[2] = zeroPt + Vector_2d (R(70.0), R(278.0));
	where[3] = zeroPt + Vector_2d (R(70.0), R(179.0));
	where[4] = zeroPt + Vector_2d (R(70.0), R(89.0));
	where[5] = zeroPt + Vector_2d (R(70.0), R(58.0));															// ds mission report success or fail

	sprintf (text[0], "Kills\t\t\t\t%d\nTurret Kills\t\t\t%d", gLocal->GetKills (), gLocal->GetTurretKills ());
	sprintf (text[1], "Suicides\t\t\t%d\nDeaths\t\t\t%d", gLocal->GetSuicides (), gLocal->GetDeaths ());
	sprintf (text[2], "Resources\t\t\t%d\nExtra Resources\t\t%d", gLocal->GetResources (), ((CrixaShip *) gLocal->GetCrixa () ())->GetResources ());
	int				deaths = gLocal->GetDeaths ();
	if (deaths < 1)
		deaths = 1;
	sprintf (text[3], "Kill Ratio\t\t\t%.02f", (gLocal->GetTurretKills () + gLocal->GetKills () - gLocal->GetSuicides ()) / double (deaths));
	gLocal->AddScore (0, 0, 0, 0, 0, ((CrixaShip *) gLocal->GetCrixa () ())->GetResources ());
	sprintf (text[4], "Score\t\t\t\t%d", gLocal->GetScore ());

	// Check the level state on end game...if it's named "LevelWin", we won! Otherwise, we lost or something else happened

	if (Level::Status() & LV_MISSION_COMPLETE)
	{
		int xres = ( (CrixaShip *) gLocal->GetCrixa()() )->GetResources();
		if (xres > ParmInteger(CRIXA_COST))
		{
			sprintf (text[5],"Status\t\t\t*** MISSION SUCCESSFUL ***\n\t\t\t\t1000 resources exchanged for extra ship!");
			(gLocal->GetCrixa())->AddResource(RESOURCE_DEDUCTION,ParmInteger(CRIXA_COST));
		}
		else
			sprintf (text[5],"Status\t\t\t*** MISSION SUCCESSFUL ***");
	}
	else
		sprintf (text[5],"Status\t\t\t*** MISSION FAILED ***");
	TextSeq		*seq[6];
	for (int i = 0; i < 6; i++)
	{
		FormatText (text[i]);
		seq[i] = new TextSeq (font, boldfont, text[i], 300, INT_MAX);								//	text sequence
	}

	//gSoundMgr->StopCD ();
	//gSoundMgr->PlayCDTrack (ParmInteger (CD_DEBRIEF_MUSIC));

	gSoundMgr->StopAll();																													// (MK) Stop all emitters & playing sounds
	
	while (GetAsyncKeyState (START_KEY)) {}																				//	try to trap keyboard

	while (!GetAsyncKeyState (START_KEY))
	{
		ratelimiter.Start();																												//	keep display from flipping more than 30fps
		while (ratelimiter.Elapsed() < R(0.03333)) {}																//	otherwise, some machines will goof up the display
		gDisplay->Lock ();																													//	lock the display
		for (i = 0; i < 6; i++)
			seq[i]->Step (where[i]);
		gDisplay->Unlock ();																												//	unlock the display
		gDisplay->Swap ();																													//	swap the surfaces
	}
	gSoundMgr->PlaySound(49);																											//	confirming tone

	for (i = 0; i < 5; i++)
		delete seq[i];

	while (GetAsyncKeyState (START_KEY)) {}																				//	try to trap keyboard
	while (GetAsyncKeyState (ADVANCE_KEY)) {}																			//	try to trap keyboard

	//gSoundMgr->StopCD();
}																																								//	end

//------------------------------------------------------------------------------
