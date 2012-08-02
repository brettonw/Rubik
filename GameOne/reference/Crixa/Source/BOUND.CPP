//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/21/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "bound.h"
#include "area.h"

//------------------------------------------------------------------------------
Bound		gBound (SCRN_HEIGHT, 0, 0, SCRN_HEIGHT);																//	global play area bound

//------------------------------------------------------------------------------
ClipCode	Bound::TestClip (const Bound &bound)																	//	test this bound against another for clipping
{																																								//	begin
	if (left > bound.right) return OUTSIDE;																				//	if the whole thing is off to the left, return outside
	if (right < bound.left) return OUTSIDE;																				//	if the whole thing is off to the right, return outside
	if (bottom > bound.top) return OUTSIDE;																				//	if the whole thing is off the bottom, return outside
	if (top < bound.bottom) return OUTSIDE;																				//	if the whole thing is off the top, return outside
	if (left < bound.left) return INTERSECTING;																		//	if the left edge is outside, return clipped
	if (right > bound.right) return INTERSECTING;																	//	if the right edge is outside, return clipped
	if (bottom < bound.bottom) return INTERSECTING;																//	if the top edge is outside, return clipped
	if (top > bound.top) return INTERSECTING;																			//	if the bottom edge is outside, return clipped
	return INSIDE;																																//	return inside
}																																								//	end

//------------------------------------------------------------------------------
ClipCode	Bound::TestClip (int x, int y)																				//	test a point against the bound for containment
{																																								//	begin
	if (x >= right) return OUTSIDE;																								//	if the point is off to the right, return outside
	if (x < left) return OUTSIDE;																									//	if the point is off to the left, return outside
	if (y >= top) return OUTSIDE;																									//	if the point is off the top, return outside
	if (y < bottom) return OUTSIDE;																								//	if the point is off the bottom, return outside
	return INSIDE;																																//	return inside
}																																								//	end

//------------------------------------------------------------------------------
