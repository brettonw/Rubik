//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/12/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "pointer.h"
#include "area.h"

//------------------------------------------------------------------------------
Point_2d	Pointer::mid;																													//	center location of the screen play area

//------------------------------------------------------------------------------
Pointer::Pointer (void)																													//	destructor
{																																								//	begin
	mid (SCRN_HEIGHT * R(0.5), SCRN_HEIGHT * R(0.5));															//	compute the coordinates of the mid point of the screen space
}																																								//	end

//------------------------------------------------------------------------------
Pointer::~Pointer (void)																												//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
