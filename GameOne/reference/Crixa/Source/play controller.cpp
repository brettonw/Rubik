//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/29/97
//	02/27/97	DS		Moved referee creation and destruction to play level
//									from Level because of StopNetwork() dependencies
//	03/01/97	DS		Logic to handle conditional level loading 
//									(this logic is really fragile and needs some thinkin')
//	03/10/97	DS		Screen switching, pausing, sounds added in
//	03/11/97	DS		Timeout for title screen
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "play controller.h"
#include "display.h"
#include "get parameters.h"
#include "sprite array mgr.h"
#include "sound mgr.h"
#include "pi.h"
#include "text buffer.h"
#include "local human.h"
#include "referee.h"
#include "network play.h"
#include "debugger.h"
#include "disk file.h"
#include "timer.h"

//------------------------------------------------------------------------------
PlayController	*gPlayController = 0;																						//	global pointer to the play controller

//------------------------------------------------------------------------------
PlayController::PlayController (HWND wind) : Controller (wind)									//	constructor
{																																								//	begin
	controllers[PLAY_CONTROLLER_ID] = this;																				//	register the play controller in the controller list
}																																								//	end

//------------------------------------------------------------------------------
void	PlayController::Begin (void)																							//	set up the controller
{																																								//	begin
	Controller::Begin ();																													//	do the parental thing
	gSoundMgr = new SoundMgr;
	gDisplay = new Display;																												//	create the display

	while (GetAsyncKeyState (VK_SPACE )) {}																				//	try to trap keyboard
	while (GetAsyncKeyState (VK_ESCAPE)) {}																				//	try to trap keyboard
	Timer mytimer;
	mytimer.Start();
	if (1)
	{
		SArrayPtr	splash = gSpriteMgr.Fetch ("title", FALSE);												//	load the image sprite
		gDisplay->EraseAll ();
		gDisplay->Lock ();																													//	lock the display
		// draw the splash screen (2 halves because of sprite size limit)
		splash[0]->DrawUnclipped ( ((SCRN_WIDTH - 640) / 2) + 31  , (SCRN_HEIGHT - 480) / 2 );
		splash[1]->DrawUnclipped ( ((SCRN_WIDTH - 640) / 2) + 320 , (SCRN_HEIGHT - 480) / 2 );
		gDisplay->Unlock ();																												//	unlock the display
		gDisplay->Swap ();																													//	swap the surfaces to show screen

		StartNetwork (ParmString (SERVER_NAME), ParmInteger (SERVER_PORT), ParmInteger (NETWORK_PLAYERS));
		PreLoad ();

/*
		SArrayPtr loadicon = gSpriteMgr.Fetch ("loading", FALSE);										//	load the loading icon
		loadicon[1]->DrawUnclipped ( ((SCRN_WIDTH - 640) / 2) + 320 - loadicon[1]->Width() / 2, 
																 ((SCRN_HEIGHT - 480) / 2) + 50);
*/

		while (!GetAsyncKeyState (VK_SPACE) && !GetAsyncKeyState (VK_ESCAPE) && (mytimer.Elapsed()<3.0))
		{}
	}

	if (1)
	{
		SArrayPtr	control = gSpriteMgr.Fetch (ParmString (INTERFACE_SPRITE), FALSE);//	load the control image sprite
		gDisplay->EraseAll ();																											//	erase the display
		gDisplay->Lock ();																													//	lock the display
		control[0]->DrawUnclipped (SCRN_HEIGHT, SCRN_HEIGHT - 480);									//	draw the control panel
		gDisplay->Unlock ();																												//	unlock the display
		gDisplay->Swap ();																													//	swap the surfaces
		gDisplay->EraseAll ();																											//	erase the display
		gDisplay->Lock ();																													//	lock the display
		control[0]->DrawUnclipped (SCRN_HEIGHT, SCRN_HEIGHT - 480);									//	draw the control panel
		gDisplay->Unlock ();																												//	unlock the display
		gDisplay->Swap ();																													//	swap the surfaces
	}
	gReferee = new Referee (gDisplay->Rate());																	//	ds
	levelID = ParmInteger (START_LEVEL);
	level = new Level (levelID);
}																																							//	end

//------------------------------------------------------------------------------
void	PlayController::Loop (void)																								//	do one iteration of a control loop
{																																								//	begin
	if (!level->Play ())
	{
		DEBUG_STR("\nPlayController: Exiting Level\n");
		int stat = level->Status();																									//	only way here is through end-of-level trigger
		if (stat & LV_NO_MORE_LIVES) 																								//	no more lives? then return to main
		{
			PostMessage (window, WM_CLOSE, 0, 0);																			//	close the window
			Controller::Switch (QUIT_CONTROLLER_ID);																	//	close the controller and let End() handle deletion
			DEBUG_STR("    Player ran out of lives.\n");
			return;
		}
		else	// still lives left, player must have quit the level on purpose
		{		
			if (stat & LV_MISSION_COMPLETE)	{
				DEBUG_STR("    Mission sucessful.\n");
				levelID++;																															//	time to go to the next level
			}
			else
				DEBUG_STR("    Mission failed. Restarting level.\n");

			char levelName[256];																											//	make sure the level file exists
			sprintf (levelName, "Level%02d.dat", levelID);
			DiskFile exist(levelName);
			if (exist.Open(DF_READ_ONLY))	{
				exist.Close();
				delete level;
				delete gReferee;
				PreLoad ();		// original location.. moving to Level
				gReferee = new Referee (gDisplay->Rate());
				level = new Level (levelID);
				return;
			}
			else {																																		// file doesn't exist
				PostMessage (window, WM_CLOSE, 0, 0);																		//	close the window
				Controller::Switch (QUIT_CONTROLLER_ID);																//	close the controller and let End() handle deletion
				return;
			} // file doesn't exist

		} // player quit on purpose
	} // level wasn't playing
} // Loop()																																			//	end

//------------------------------------------------------------------------------
void	PlayController::End (void)																								//	clean up the controller
{																																								//	begin

	delete level;																																	//	End() is called only when the play controller gets switched

	while (GetAsyncKeyState (VK_RETURN )) {}																			//	try to trap keyboard
	while (GetAsyncKeyState (VK_SPACE )) {}																				//	try to trap keyboard
	while (GetAsyncKeyState (VK_ESCAPE)) {}																				//	try to trap keyboard
	if (0)
	{
		SArrayPtr credits = gSpriteMgr.Fetch("credits",FALSE);
		gDisplay->EraseAll ();
		gDisplay->Lock ();																													//	lock the display
		// draw the credits screen (2 halves because of sprite size limit)
		credits[0]->DrawUnclipped ( ((SCRN_WIDTH - 640) / 2)       , (SCRN_HEIGHT - 480) / 2 );
		credits[1]->DrawUnclipped ( ((SCRN_WIDTH - 640) / 2) + 320 , (SCRN_HEIGHT - 480) / 2 );
		gDisplay->Unlock ();																												//	unlock the display
		gDisplay->Swap ();																													//	swap the surfaces

		while (!GetAsyncKeyState (VK_SPACE) && !GetAsyncKeyState (VK_ESCAPE) && !GetAsyncKeyState (VK_RETURN));
	}

	delete gSoundMgr;
	delete gDisplay;
	StopNetwork ();
	delete gReferee;
	gDisplay = 0;
}																																								//	end

//------------------------------------------------------------------------------
void	PlayController::PreLoad (void)																						//	pre-load sprites from the common directory
{																																								//	begin
	TxtBuffer	buffer ("preload.h");
	cstr	line;
	buffer >> line;
	while (strcmp (line, "end"))
	{
		gSpriteMgr.Fetch (line);
		buffer >> line;
	}
}																																								//	end

//------------------------------------------------------------------------------
