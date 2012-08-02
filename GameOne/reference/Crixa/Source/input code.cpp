//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/21/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "input code.h"
#include <limits.h>

//------------------------------------------------------------------------------
Real	Code::scale = R(1.0) / R(3276.7);																					//	the multiplier for converting the thrust values to a scale

//------------------------------------------------------------------------------
Code::Code (Real thr, Real str, Real rot, uint weap, uint fir, uint lck)				//	constructor
{																																								//	begin
	thrust = int (thr * R(3276.7));																								//	compute and assign the thrust value
	strafe = int (str * R(3276.7));																								//	compute and assign the strafe value
	rotate = int (rot * R(3276.7));																								//	compute and assign the rotate value
	weapon = weap;																																//	copy the weapon
	fire = fir;																																		//	copy the fire flag
	lock = lck;																																		//	copy the lock code
	valid = 1;																																		//	set the valid code to TRUE
	quit = 0;																																			//	set the quit code to FALSE
}																																								//	end

//------------------------------------------------------------------------------
Code::Code (const Code &code)																										//	copy constructor
{																																								//	begin
	*(double *) this = *(double *) &code;																					//	copy the code
}																																								//	end

//------------------------------------------------------------------------------
