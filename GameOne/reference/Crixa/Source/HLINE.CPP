//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/02/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "Precompile.pch++"
#endif

//------------------------------------------------------------------------------
#include "hline.h"
#include "math utility.h"

//------------------------------------------------------------------------------
HLine::~HLine (void)																														//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Real	HLine::Intersect (CPoint p, CVector v, Real r, Sect *sect, int *data)			//	return the nearest intersection with a circle
{																																								//	begin
	Real	yVel = R(1.0) / v[Y],																										//	compute the reciprocal of the y velocity
				yRad = (r + EPSILON) * Sign (yVel),																			//	compute the radius to use for the intersection computation
				yHit = y - yRad,																												//	compute the first hit location
				yTime = (yHit - p[Y]) * yVel;																						//	compute the time of the first hit
	if (yTime >= ZERO)																														//	if the intersection time is positive or immediate
	{																																							//	begin
		*data = 0;																																	//	flag this as the first intersection
		sect->plane (ZERO, -Sign (yVel));																						//	set the intersection plane
		sect->point = p + (v * yTime) + (sect->plane * -r);													//	compute the intersection point
		return yTime;																																//	return the intersection time
	}																																							//	end
	yHit = y + yRad;																															//	compute the line to intersect with the center point of the circle
	yTime = (yHit - p[Y]) * yVel;																									//	compute the time of the second hit
	if (yTime >= ZERO)																														//	if the intersection time is positive or immediate
	{																																							//	begin
		*data = 1;																																	//	flag this as the second intersection
		return yTime;																																//	return the intersection time
	}																																							//	end
	return INFINITY;																															//	no intersection occurs
}																																								//	end

//------------------------------------------------------------------------------
Real	HLine::Classify (CPoint p)																								//	return which side of the line the point is on
{																																								//	begin
	return p[Y] - y;																															//	subtract the point location from the line location and return the result
}																																								//	end

//------------------------------------------------------------------------------
