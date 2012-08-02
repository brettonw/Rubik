//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/28/97
//
//------------------------------------------------------------------------------

#ifndef	AREA
#define	AREA

//------------------------------------------------------------------------------
#include "space.h"

//------------------------------------------------------------------------------
#if	1
#define	SCRN_WIDTH	640																													//	the visible screen width
#define	SCRN_HEIGHT	480																													//	the visible screen height
#elif 1
#define	SCRN_WIDTH	800																													//	the visible screen width
#define	SCRN_HEIGHT	600																													//	the visible screen height
#else
#define	SCRN_WIDTH	1024																												//	the visible screen width
#define	SCRN_HEIGHT	768																													//	the visible screen height
#endif

//------------------------------------------------------------------------------
extern	Vector_2d	gCenter;																											//	the center of the game area

//------------------------------------------------------------------------------

#endif	//	AREA