//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	02/02/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	Control Macros for Windows Headers
//------------------------------------------------------------------------------
#define	WIN32_LEAN_AND_MEAN

//------------------------------------------------------------------------------
//	Win95 Headers
//------------------------------------------------------------------------------
#ifdef USING_MFC
#include "stdafx.h"
#include <process.h>
#else
#include <windows.h>
#include <windowsx.h>
#endif
#include <winsock.h>

//------------------------------------------------------------------------------
//	ANSI Headers
//------------------------------------------------------------------------------
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <new.h>

//------------------------------------------------------------------------------
//	types
//------------------------------------------------------------------------------
typedef	unsigned int		uint;																										//	abbreviation
typedef	unsigned short	ushort;																									//	abbreviation
typedef	unsigned long		ulong;																									//	abbreviation
typedef	unsigned char		uchar;																									//	abbreviation
typedef	char						*cstr;																									//	a C style string (NULL terminated)
typedef	uchar						*pstr;																									//	a pascal style string (preceded by a length byte)
typedef	uchar						*uPtr;																									//	unsigned char pointer


//------------------------------------------------------------------------------
//	Stuff for boolean type
//------------------------------------------------------------------------------
typedef	ulong						Bool;																										//	boolean type
#ifndef	TRUE																																		//	if the macro TRUE is undefined
enum	{FALSE = 0, TRUE = 1};																										//	enumerate the values of TRUE and FALSE
#endif																																					//	end if

//------------------------------------------------------------------------------
//	real number type and interfaces
//------------------------------------------------------------------------------
#define	USE_FLOATS	1																														//	whether or not the real type is a float
#if	USE_FLOATS == 1																															//	if the real type is a float
typedef	float						Real;																										//	common name for floating point type
#define	R(num)	Real (num ## F)																									//	want constants to have an F after them to specify float types
#else																																						//	otherwise, use the double versions
typedef	double					Real;																										//	common name for floating point type
#define	R(num)	Real (num)																											//	want constants to have an L after them to specify long double types
#endif																																					//	end real type is a float
#define	COS(x)			Real (cos (Real (x)))																				//	use normal version of function
#define	ACOS(x)			Real (acos (Real (x)))																			//	use normal version of function
#define	SIN(x)			Real (sin (Real (x)))																				//	use normal version of function
#define	ASIN(x)			Real (asin (Real (x)))																			//	use normal version of function
#define	TAN(x)			Real (tan (Real (x)))																				//	use normal version of function
#define	ATAN(x)			Real (atan (Real (x)))																			//	use normal version of function
#define	ATAN2(y, x)	Real (atan2 (Real (y), Real (x)))														//	use normal version of function
#define	SQRT(x)			Real (sqrt (Real (x)))																			//	use normal version of function
#define	FABS(x)			Real (fabs (Real (x)))																			//	use normal version of function
#define	POW(x, y)		Real (pow (Real (x), Real (y)))															//	use normal version of function
#define	FLOOR(x)		Real (floor (Real (x)))																			//	use normal version of function
#define	CEIL(x)			Real (ceil (Real (x)))																			//	use normal version of function

//------------------------------------------------------------------------------
//	math constants
//------------------------------------------------------------------------------
#define	EPSILON			R(1.0e-8)																										//	a very small value
#define	ZERO				R(0.0)																											//	zero
#define	INFINITY		R(1.0e12)																										//	a very big value

//------------------------------------------------------------------------------

