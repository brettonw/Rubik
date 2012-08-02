//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	12/16/96
//
//------------------------------------------------------------------------------

#ifndef	DIRECTION
#define	DIRECTION

//------------------------------------------------------------------------------
#include "space.h"

//------------------------------------------------------------------------------
#define	DIRECTION_BITS	10																											//	the number of bits of resolution for the direction vector table size
#define	DIRECTION_COUNT	(1 << DIRECTION_BITS)																		//	the number of direction vectors stored in the direction array

//------------------------------------------------------------------------------
void		DefineDirections (void);																								//	fill in the array of direction vectors

//------------------------------------------------------------------------------
extern	Vector_2d	gDirection[DIRECTION_COUNT][2];																//	global array of vectors associated with angles

//------------------------------------------------------------------------------

#endif	//	DIRECTION
