//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/10/97
//
//------------------------------------------------------------------------------

#ifndef	SPRING
#define	SPRING

//------------------------------------------------------------------------------
#include "piece ptr.h"
#include "pixel.h"

//------------------------------------------------------------------------------
void	Spring (PiecePtr &a, PiecePtr &b, Real spring, Real damp, Real length);		//	apply spring forces to two objects
void	Spring (PiecePtr &a, CPoint loc, Real spring, Real damp, Real length);		//	apply spring forces to a single relative to a location
void	DrawSpring (CPoint a, CPoint b, int dots, Pixel color);										//	draw a spring line between two objects

//------------------------------------------------------------------------------

#endif	//	SPRING
