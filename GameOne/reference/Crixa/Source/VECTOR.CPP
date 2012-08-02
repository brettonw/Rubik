//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/08/96
//	02/28/97	DS		stronger zero checking
//	03/11/97	DS		reverted to temporary debugging code
//	03/12/97	DS		numerical spackle applied
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "vector.h"
#include "debugger.h"
#include "math utility.h"

//------------------------------------------------------------------------------
Real	Vector_2d::Normalize (void)																								//	reduce the vector to length 1
{																																								//	begin
	Real	norm = *this | *this;																										//	compute the squared length of the vector
	if (norm > ZERO)																															//	if the norm is not zero
	{																																							//	begin
		norm = SQRT (norm);																													//	get the square root of the length

		if (norm > ZERO)																														//	ds: another normalizing check
		{
			xy[X] /= norm;																														//	divide each of the component by the length
			xy[Y] /= norm;																														//	divide each of the component by the length
		}
		else																																				//	this is a big problem...we still want unit vector
		{
			xy[X] = R(0.0);																														//	return an arbitrary unit vector for now
			xy[Y] = R(1.0);
			DEBUG_SOFT("Vector: Loss of precision during Normalize\n");
		}
	}																																							//	end
	return norm;																																	//	return the length
}																																								//	end

//------------------------------------------------------------------------------
Real	Vector_2d::Norm (void) const																							//	return the length of the vector
{																																								//	begin
	Real	norm = *this | *this;																										//	compute the squared length of the vector
	if (norm > ZERO)																															//	if the norm is not zero
		norm = SQRT (norm);																													//	get the square root of the length
	return norm;																																	//	return the length
}																																								//	end

//------------------------------------------------------------------------------
void	Vector_2d::operator += (const Vector_2d &v)																//	add another vector into this one
{																																								//	begin
	xy[X] += v.xy[X]; xy[Y] += v.xy[Y];																						//	add each of the components together
}																																								//	end

//------------------------------------------------------------------------------
void	Vector_2d::operator -= (const Vector_2d &v)																//	subtract another vector from this one
{																																								//	begin
	xy[X] -= v.xy[X]; xy[Y] -= v.xy[Y];																						//	add each of the components together
}																																								//	end

//------------------------------------------------------------------------------
void	Vector_2d::operator *= (Real s)																						//	multiply this vector by a scalar
{																																								//	begin
	xy[X] *= s; xy[Y] *= s;																												//	multiply the components by the scalar
}																																								//	end

//------------------------------------------------------------------------------
