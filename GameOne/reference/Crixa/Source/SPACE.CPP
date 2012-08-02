//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	03/25/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "space.h"

//------------------------------------------------------------------------------
Vector_2d		Reflect (CVector v, CVector n)																			//	reflect a source vector around an axis
{																																								//	begin
	return v - (n * (R(2.0) * (v | n)));																					//	return the computed vector
}																																								//	end

//------------------------------------------------------------------------------
Vector_2d		Rotate (CVector vec, Real angle)																		//	rotate a source vector some angle
{																																								//	begin
	Real	cosine = COS (angle),																										//	compute the cosine of the angle
				sine = SIN (angle),																											//	compute the sine of the angle
				x = (vec[X] * cosine) - (vec[Y] * sine),																//	compute the rotated x coordinate
				y = (vec[X] * sine) + (vec[Y] * cosine);																//	compute the rotated y coordinate
	return Vector_2d (x, y);																											//	return the computed vector
}																																								//	end

//------------------------------------------------------------------------------
Vector_2d		Rotate (CVector vec, CVector axis)																	//	rotate a source vector some angle
{																																								//	begin
	Real	x = (vec[X] * axis[X]) - (vec[Y] * axis[Y]),														//	compute the rotated x coordinate
				y = (vec[X] * axis[Y]) + (vec[Y] * axis[X]);														//	compute the rotated y coordinate
	return Vector_2d (x, y);																											//	return the computed vector
}																																								//	end

//------------------------------------------------------------------------------
