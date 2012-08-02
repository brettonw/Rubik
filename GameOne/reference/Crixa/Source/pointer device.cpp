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
#include "pointer device.h"
#include "area.h"

//------------------------------------------------------------------------------
Point_2d	PointerDevice::mid;																										//	center location of the screen play area

//------------------------------------------------------------------------------
PointerDevice::PointerDevice (void)																							//	constructor
{																																								//	begin
	mid (SCRN_HEIGHT * R(0.5), SCRN_HEIGHT * R(0.5));															//	compute the coordinates of the mid point of the screen space
	update = -1;																																	//	clear the update flag
	stamp = 0;																																		//	start with a null stamp
}																																								//	end

//------------------------------------------------------------------------------
PointerDevice::~PointerDevice (void)																						//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Bool	PointerDevice::Update (int count)																					//	update the pointer status
{																																								//	begin
	if (count != update)																													//	if the count is not the same as the current update value
	{																																							//	begin
		update = count;																															//	save the count
		return TRUE;																																//	return true to indicate the update should occur
	}																																							//	end
	return FALSE;																																	//	return false to indicate no update should occur
}																																								//	end

//------------------------------------------------------------------------------
