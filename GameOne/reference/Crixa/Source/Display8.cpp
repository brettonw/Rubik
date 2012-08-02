//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/21/97
//	02/17/97	DS		Modified for Crixa-in-a-window
//	02/22/97	DS		Modified display rate reporting
//	03/05/97	DS		Modified Rate Multiplier in TimeRate (was <2.0 set 2.0)
//									default rate is 30.0 fps
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#ifdef	EIGHTBITCOLOR

//------------------------------------------------------------------------------
#include "display8.h"
#include "timer.h"
#include "get parameters.h"
#include "debugger.h"
#include "vid debug.h"

//------------------------------------------------------------------------------
Display		*gDisplay = 0;																												//	global display pointer
extern		HWND		gWindow;																											//	defined in main.cpp

//------------------------------------------------------------------------------
Display::Display (Bool timeRate)																								//	constructor
{																																								//	begin
	rate = R(1.0 / 30.0);																													//	the default rate
	Pixel::BuildAlphaTable ();																										//	build the alpha lookup table
	Pixel::BuildColorTable ();																										//	build the color cube lookup tables
	SetRect (&scrnRect, 0, 0, SCRN_WIDTH, SCRN_HEIGHT);														//	the rectangle of the full screen
	SetRect (&playRect, 0, 0, SCRN_HEIGHT, SCRN_HEIGHT);													//	the rectangle of the play screen
#if !CRIXA_DEBUG_HARMONY
	DXCheck (DirectX::draw2->SetDisplayMode (SCRN_WIDTH, SCRN_HEIGHT, PIXEL_SIZE,0,0));//	set the display mode
	ValidateRect (gWindow, &scrnRect);
#endif

#if CRIXA_DEBUG_HARMONY
	DDSURFACEDESC	ddsd;																														//	a surface description structure for direct draw
	ddsd.dwSize = sizeof (ddsd);																									//	set the size of the surface description structure
	ddsd.dwFlags = DDSD_CAPS;																											//	ds: specify caps
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;																	//	ds: set the caps for our primary surface
	DXCheck (DirectX::draw2->CreateSurface (&ddsd, &primary, 0));									//	create the requested surface

	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;													//	ds: specify caps, width and height
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;																	//	ds: specify an offscreen buffer
	ddsd.dwWidth = SCRN_WIDTH;																										//	ds: buffer should be this wide..
	ddsd.dwHeight = SCRN_HEIGHT;																									//	ds: and this high
	DXCheck (DirectX::draw2->CreateSurface (&ddsd, &back, 0));										//	ds: create our offscreen buffer
#else
	DDSURFACEDESC	ddsd;																														//	a surface description structure for direct draw
	ddsd.dwSize = sizeof (ddsd);																									//	set the size of the surface description structure
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;															//	the capabilites and buffer count will be valid fields, as well as the pixel format
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;//	set the caps to our double buffer design
	ddsd.dwBackBufferCount = 1;																										//	with one back buffer
	DXCheck (DirectX::draw2->CreateSurface (&ddsd, &primary, 0));									//	create the requested surface
	DDSCAPS				caps;																														//	direct draw surface capabilities structure
	caps.dwCaps = DDSCAPS_BACKBUFFER;																							//	set the caps to a back buffer request
	DXCheck (primary->GetAttachedSurface (&caps, &back));													//	fetch the back surface
#endif

	current = new Surface (back);																									//	create a surface of my own
	PALETTEENTRY	entries[256];																										//	a dummy entries table

	int		orig[6] = {0x00, 0x33, 0x66, 0x99, 0xcc, 0xff},
				c1[6] = {0x00, 0x33, 0x66, 0x99, 0xcc, 0xff},
				col = 0;																																//	first color index
	Real	gamma = ParmReal (GAMMA_CORRECTION) / R(1.8);														//	compute the gamma correction exponent

	for (int i = 0; i < 6; i++)																										//	loop over all of the color cube values
		c1[i] = int ((R(255.0) * POW (orig[i] / R(255.0), gamma)) + R(0.5));				//	computing values using the gamma correction exponent

	for (int red = 0; red < 6; red++)																							//	loop through the reds
		for (int green = 0; green < 6; green++)																			//	loop through the greens
			for (int blue = 0; blue < 6; blue++)																			//	loop through the blues
			{
				entries[col].peFlags = 0;
				entries[col].peRed = c1[red];
				entries[col].peGreen = c1[green];
				entries[col].peBlue = c1[blue];
				col++;
			}

	for (i = 0; i < 32; i++, col++)
	{
		int	grey = i << 3;
		entries[col].peFlags = 0;
		entries[col].peRed =  entries[col].peGreen =  entries[col].peBlue = grey;
	}

	for (; col < 256; col++)
	{
		entries[col].peFlags = 0;
		entries[col].peRed = entries[col].peGreen = entries[col].peBlue = 0;
	}


	DXCheck (DirectX::draw2->CreatePalette (DDPCAPS_8BIT, entries, &palette, 0));	//	create the palette
	DXCheck (primary->SetPalette (palette));																			//	set the surface palette
	DXCheck (back->SetPalette (palette));																					//	set the surface palette
	DXCheck (palette->SetEntries(0,0,255,entries));


/*
	for (i = 0; i < 256; i++)
		entries[i].peGreen = entries[i].peBlue = 0;
	DXCheck (DirectX::draw2->CreatePalette (DDPCAPS_8BIT, entries, &redpalette, 0));
*/

	if (timeRate)																																	//	if the timing flag is true
		TimeRate ();																																//	figure the frame time
}																																								//	end

//------------------------------------------------------------------------------
Display::~Display (void)																												//	destructor
{																																								//	begin
	DXCheck (primary->Release ());																								//	free the surface object
	#if CRIXA_DEBUG_HARMONY
	DXCheck (back->Release ());
	#endif
	DXCheck (DirectX::draw2->RestoreDisplayMode ());																//	restore the previous display mode
}																																								//	end

//------------------------------------------------------------------------------
void	Display::Swap (void)																											//	swap the currently selected buffer to the screen
{																																								//	begin
#if CRIXA_DEBUG_HARMONY
	RECT	sR;
	RECT	dR;
	POINT	pt;

	sR.left = 0;																																	//	ds: Source RECT on offscreen surface
	sR.right = SCRN_WIDTH;
	sR.top = 0;
	sR.bottom = SCRN_HEIGHT;

	GetClientRect (gWindow, &dR);
	pt.x = pt.y = 0;
	ClientToScreen (gWindow, &pt);
	OffsetRect(&dR, pt.x, pt.y);

	DXCheck (primary->Blt (&dR,back,&sR,0, NULL));																//	try to copy the surfaces in direct draw
#else
	DXCheck (primary->Flip (0, DDFLIP_WAIT));																			//	try to flip the surfaces in direct draw
#endif
}																																								//	end

//------------------------------------------------------------------------------
HDC		Display::GetDC (void)																											//	get the display context for the primary surface
{																																								//	begin
	HDC	hdc;																																			//	display context pointer
	DXCheck (primary->GetDC (&hdc));																							//	get the display context from the primary buffer
	return hdc;																																		//	return the pointer
}																																								//	end

//------------------------------------------------------------------------------
void	Display::ReleaseDC (HDC hdc)																							//	release the display context
{																																								//	begin
	DXCheck (primary->ReleaseDC (hdc));																						//	release the display context
}																																								//	end

//------------------------------------------------------------------------------
void	Display::TimeRate (int count)																							//	run a timer on the refresh rate
{																																								//	begin
	EraseAll ();																																	//	erase the screen
	Swap ();																																			//	swap the buffer
	EraseAll ();																																	//	erase again
	Timer	timer;																																	//	timer for computing the rate
	timer.Start ();																																//	start the timer
	for (int i = 0; i < count; i++)																								//	loop for the specified number of operations
	{																																							//	begin
		Erase ();																																		//	erase the back buffer
		Lock ();																																		//	lock the buffer
		Unlock ();																																	//	unlock the buffer
		Swap ();																																		//	swap the buffer
	}																																							//	end
	Erase();																																			//	cleanup after white flash...
	Swap();																																				//	...due to rapid switching
	Erase();																																			//	...and possible DX latency

	DEBUG_STR ("Display8:\n");
	rate = FLOOR (count / timer.Elapsed ());																			//	compute the frame rate
	DEBUG_STR ("    observed flip rate: %.2f\n",rate);
	if (rate>85.0)
		DEBUG_STR ("    (unexpectedly high rate)\n");
	Real	multiplier = FLOOR (rate / ParmReal (TARGET_FRAME_RATE));								//	compute the multiplier for the refresh rate
	if (multiplier < R(1.0))																											//	if the multiplier is less than two
		multiplier = R(1.0);																												//	use a multiplier of two
	rate = multiplier / rate;																											//	compute the frame rate relative to the refresh rate
	DEBUG_STR ("    target rate: %.2f (%.2f)\n\n", R(1.0) / rate, multiplier);		//	report the computed target frame rate
#if CRIXA_DEBUG_HARMONY
	DEBUG_STR ("    *** palettes will be corrupted in window mode ***\n");
#endif
}																																								//	end

//------------------------------------------------------------------------------
void	Display::Restore (void)																										//	restore the display in the event of an error
{																																								//	begin
	DXCheck (primary->SetPalette (syspalette));																		//	set the surface palette back to the system palette
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	EIGHTBITCOLOR
