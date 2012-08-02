//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	05/07/96
//
//------------------------------------------------------------------------------

#ifndef	MATH_UTILITY
#define	MATH_UTILITY

//------------------------------------------------------------------------------
#include "pi.h"

//------------------------------------------------------------------------------
inline	Real	Sign (Real value)																									//	return a unit multiplier representing the sign of the value
{																																								//	begin
	return (value > ZERO) ? R(1.0) : (value < ZERO) ? R(-1.0) : ZERO;							//	return 1 for positive, -1 for negative, and 0 for 0
}																																								//	end

//------------------------------------------------------------------------------
inline	Real		Positive (Real value)																						//	return whether or not the value is positive
{																																								//	begin
	return (value > ZERO) ? R(1.0) : ZERO;																				//	return 1 for positive, and 0 otherwise
}																																								//	end

//------------------------------------------------------------------------------
inline	Real		Negative (Real value)																						//	return whether or not the value is negative
{																																								//	begin
	return (value < ZERO) ? R(-1.0) : ZERO;																				//	return -1 for negative, and 0 otherwise
}																																								//	end

//------------------------------------------------------------------------------
inline	Real	DegreesToRadians (Real angle)																			//	convert degrees to radians
{																																								//	begin
	return angle * PI * (R(1.0) / R(180.0));																			//	multiply the angle by the conversion factor
}																																								//	end

//------------------------------------------------------------------------------
inline	Real	RadiansToDegrees (Real angle)																			//	convert radians to degrees
{																																								//	begin
	return angle * R(180.0) * OVER_PI;																						//	multiply the angle by the conversion factor
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	MATH_UTILITY