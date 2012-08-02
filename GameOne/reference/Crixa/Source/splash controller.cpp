//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/08/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "splash controller.h"
#include "play controller.h"
#include "display.h"
#include "sprite array mgr.h"

//------------------------------------------------------------------------------
SplashController	*gSplashController = 0;																				//	global pointer to the play controller

//------------------------------------------------------------------------------
SplashController::SplashController (HWND wind) : Controller (wind)							//	constructor
{																																								//	begin
	controllers[SPLASH_CONTROLLER_ID] = this;																			//	register the splash controller in the controller list
}																																								//	end

//------------------------------------------------------------------------------
void	SplashController::Begin (void)																						//	set up the controller
{																																								//	begin
	gDisplay = new Display (FALSE);																								//	create the display
	{																																							//	begin (scoping operator to dump control sprite when done)
		SArrayPtr	splash = gSpriteMgr.Fetch ("title", FALSE);												//	load the control image sprite
		gDisplay->EraseAll ();
		gDisplay->Lock ();																													//	lock the display
		splash[0]->DrawUnclipped (44, 0);																						//	draw the left half of the splash screen
		splash[1]->DrawUnclipped (320, 0);																					//	draw the right half of the splash screen
		gDisplay->Unlock ();																												//	unlock the display
		gDisplay->Swap ();																													//	swap the surfaces
	}																																							//	end
	timer.Start ();
}																																								//	end

//------------------------------------------------------------------------------
void	SplashController::Loop (void)																							//	do one iteration of a control loop
{																																								//	begin
	if (timer.Elapsed () > R(5.0))
		Switch ();
}																																								//	end

//------------------------------------------------------------------------------
void	SplashController::End (void)																							//	clean up the controller
{																																								//	begin
	delete gDisplay;																															//	release the display when done
}																																								//	end

//------------------------------------------------------------------------------
