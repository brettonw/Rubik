//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/03/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "mouse.h"
#include "area.h"
#include "debugger.h"

//------------------------------------------------------------------------------
int		Mouse::lookup[3] = {VK_LBUTTON, VK_RBUTTON, VK_MBUTTON};									//	table of button ids for lookup

//------------------------------------------------------------------------------
Mouse::Mouse (int t1, int t2, int s)																						//	constructor
{																																								//	begin
	RECT	clip = {0, 0, SCRN_HEIGHT, SCRN_HEIGHT};																//	set up a clipping rectangle for the mouse travel
	GetClipCursor (&oldClip);																											//	get the current clip rectangle for later restoration
	ClipCursor (&clip);																														//	clip the cursor location
	int	newScale[3];																															//	array of the new threshold values for the mouse
	newScale[0] = t1;																															//	copy the first threshold
	newScale[1] = t2;																															//	copy the second threshold
	newScale[2] = s;																															//	copy the speed
	SystemParametersInfo (SPI_GETMOUSE, 0, oldScale, 0);													//	get the current mouse scaling for later restoration
	SystemParametersInfo (SPI_SETMOUSE, 0, newScale, 0);													//	set the new scaling
}																																								//	end

//------------------------------------------------------------------------------
Mouse::~Mouse (void)																														//	destructor
{																																								//	begin
	ClipCursor (&oldClip);																												//	restore the old cursor clipping rectangle
	SystemParametersInfo (SPI_SETMOUSE, 0, oldScale, 0);													//	restore the old mouse scaling
}																																								//	end

//------------------------------------------------------------------------------
void	Mouse::Update (void)																											//	update the pointer status
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Vector_2d	Mouse::GetPos (void)																									//	get the mouse position
{																																								//	begin
	tagPOINT	where;																															//	place to get the mouse location
	GetCursorPos (&where);																												//	get the location
	return Point_2d (Real (where.x) - mid[X], -(Real (where.y) - mid[Y]));				//	return a vector
}																																								//	end

//------------------------------------------------------------------------------
Bool	Mouse::Button (int button)																								//	get a button status
{																																								//	begin
	return GetAsyncKeyState (lookup[button]) ? TRUE : FALSE;											//	return true if the queried button is down
}																																								//	end

//------------------------------------------------------------------------------
