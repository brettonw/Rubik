//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	05/29/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "direction.h"
#include "pi.h"

//------------------------------------------------------------------------------
Vector_2d	gDirection[DIRECTION_COUNT][2];																				//	global array of vectors associated with angles and their perpendiculars

//------------------------------------------------------------------------------
void	DefineDirections (void)																										//	fill in the array of direction vectors
{																																								//	begin
	Real	delta = TWO_PI / Real (DIRECTION_COUNT),																//	compute the delta angle between each direction
				angle = ZERO;																														//	start with an angle of 0
	for (int i = 0; i < DIRECTION_COUNT; i++)																			//	loop over all of the direction vectors
	{																																							//	begin
		gDirection[i][0] = Vector_2d (angle);																				//	compute and assign the ith direction vector
		gDirection[i][1] = Vector_2d (angle - PI_OVER_TWO);													//	compute and assign the ith direction vector
		angle += delta;																															//	increment the angle
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
