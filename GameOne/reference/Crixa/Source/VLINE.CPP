//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/02/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "vline.h"
#include "math utility.h"

//------------------------------------------------------------------------------
inline	VLine::~VLine (void)																										//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Real	VLine::Intersect (CPoint p, CVector v, Real r, Sect *sect, int *data)			//	return the nearest intersection with a circle
{																																								//	begin
	Real	xVel = R(1.0) / v[X],																										//	compute the reciprocal of the x velocity
				xRad = (r + EPSILON) * Sign (xVel),																			//	compute the radius to use for the intersection computation
				xHit = x - xRad,																												//	compute the first hit location
				xTime = (xHit - p[X]) * xVel;																						//	compute the time of the first hit
	if (xTime >= ZERO)																														//	if the intersection time is positive or immediate
	{																																							//	begin
		*data = 0;																																	//	flag this as the first intersection
		sect->plane (-Sign (xVel), ZERO);																						//	set the intersection plane
		sect->point = p + (v * xTime) + (sect->plane * -r);													//	compute the intersection point
		return xTime;																																//	return the intersection time
	}																																							//	end
	xHit = x + xRad;																															//	compute the line to intersect with the center point of the circle
	xTime = (xHit - p[X]) * xVel;																									//	compute the time of the second hit
	if (xTime >= ZERO)																														//	if the intersection time is positive or immediate
	{																																							//	begin
		*data = 1;																																	//	flag this as the second intersection
		return xTime;																																//	return the intersection time
	}																																							//	end
	return INFINITY;																															//	no intersection occurs
}																																								//	end

//------------------------------------------------------------------------------
Real	VLine::Classify (CPoint p)																								//	return which side of the line the point is on
{																																								//	begin
	return p[X] - x;																															//	subtract the point location from the line location and return the result
}																																								//	end

//------------------------------------------------------------------------------
