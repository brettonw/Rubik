//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	09/06/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "movie controller.h"
#include "storm.h"
#include "debugger.h"

//------------------------------------------------------------------------------
MovieController	*gBlizzardController = 0;																				//	global pointer to the blizzard logo movie controller
MovieController	*gIntroController = 0;																					//	global pointer to the intro movie controller

//------------------------------------------------------------------------------
MovieController::MovieController (HWND win, cstr mov, int id) : Controller (win)//	constructor
{																																								//	begin
	controllers[id] = this;																												//	register the movie controller in the controller list
	strcpy (moviename, mov);																											//	save the movie name
}																																								//	end

//------------------------------------------------------------------------------
void	MovieController::Begin (void)																							//	set up the controller
{																																								//	begin
	Controller::Begin ();																													//	do the parental thing
	DXCheck (DirectX::draw->SetDisplayMode (640, 480, 8));												//	set the display mode for storm
	PALETTEENTRY	entries[256];																										//	a dummy entries table
	DXCheck (DirectX::draw2->CreatePalette (DDPCAPS_8BIT, entries, &palette, 0));	//	create the palette
	DDSURFACEDESC	ddsd;																														//	a surface description structure for direct draw
	ddsd.dwSize = sizeof (ddsd);																									//	set the size of the surface description structure
	ddsd.dwFlags = DDSD_CAPS;																											//	the capabilites and buffer count will be valid fields, as well as the pixel format
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;																	//	set the caps to single buffer
	DXCheck (DirectX::draw2->CreateSurface (&ddsd, &primary, 0));									//	create the requested surface
	DXCheck (primary->SetPalette (palette));																			//	set the surface palette
	SDrawManualInitialize (window, DirectX::draw, primary, 0, 0, palette, 0);			//	initialize storm
	SVidInitialize ();																														//	initialize the video player
	SVidPlayBegin (moviename);																										//	start the movie
}																																								//	end

//------------------------------------------------------------------------------
void	MovieController::Loop (void)																							//	do one iteration of a control loop
{																																								//	begin
	if (!SVidPlayContinue ())																											//	continue to play the movie
		Switch ();																																	//	switch to the splash controller when done
}																																								//	end

//------------------------------------------------------------------------------
void	MovieController::End (void)																								//	clean up the controller
{																																								//	begin
	SVidPlayEnd ();																																//	clean up after the movie
	DXCheck (primary->Release ());																								//	free the surface object
	DXCheck (palette->Release ());																								//	free the palette object
	DXCheck (DirectX::draw->RestoreDisplayMode ());																//	restore the previous display mode
}																																								//	end

//------------------------------------------------------------------------------
