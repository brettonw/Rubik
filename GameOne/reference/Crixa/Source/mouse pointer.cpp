//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	12/23/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "mouse pointer.h"
#include "area.h"
#include "debugger.h"

//------------------------------------------------------------------------------
MousePointer::MousePointer (int t1, int t2, int s)															//	constructor
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
MousePointer::~MousePointer (void)																							//	destructor
{																																								//	begin
	ClipCursor (&oldClip);																												//	restore the old cursor clipping rectangle
	SystemParametersInfo (SPI_SETMOUSE, 0, oldScale, 0);													//	restore the old mouse scaling
}																																								//	end

//------------------------------------------------------------------------------
Bool	MousePointer::Update (int count)																					//	update the pointer status
{																																								//	begin
	if (PointerDevice::Update (count))																						//	if the update should occur
	{																																							//	begin
		tagPOINT	point;																														//	temporary place to fetch the pointer value into
		GetCursorPos (&point);																											//	get the mouse location
		if ((point.x != where.x) || (point.y != where.y))														//	if the mouse has moved
		{																																						//	begin
			TimeStamp ();																															//	set the time stamp on the pointer
			where = point;																														//	save the new location
		}																																						//	end
		return TRUE;																																//	return true to indicate the update occurred
	}																																							//	end
	return FALSE;																																	//	return false to indicate that no update occurred
}																																								//	end

//------------------------------------------------------------------------------
Vector_2d	MousePointer::GetPos (void)																						//	get the mouse position
{																																								//	begin
	Real	x = (Real (where.x) - mid[X]) / (SCRN_HEIGHT >> 1),											//	compute the normalized x component of the mouse location
				y = -(Real (where.y) - mid[Y]) / (SCRN_HEIGHT >> 1);										//	compute the normalized y component of the mouse location
	return Point_2d (x, y);																												//	return a vector
}																																								//	end

//------------------------------------------------------------------------------
