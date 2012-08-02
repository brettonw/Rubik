//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/02/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "dot.h"
#include "bound.h"
#include "display.h"

//------------------------------------------------------------------------------
Dot::Dot (Body *body, Pixel col) : Visual (body)																//	constructor
{																																								//	begin
	color = col;																																	//	copy the color parameter
	fadedColor = color.Scale (R(0.75));																						//	compute the faded color
	last[0] = last[1] = body->TLocation ();																				//	save the last location of the particle
}																																								//	end

//------------------------------------------------------------------------------
Dot::~Dot (void)																																//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	Dot::Draw (CPoint where)																									//	draw the object
{																																								//	begin
	Vector_2d	loc = last[0] - where;																							//	compute the drawing location for the position of the particle two frames ago
	last[0] = last[1];																														//	save the current position into the last space
	int				x = int (loc[X]),																										//	compute the x component of the integer coordinate of the dot
						y = int (loc[Y]);																										//	compute the y component of the integer coordinate of the dot
	if (gBound.TestClip (x, y) == INSIDE)																					//	if the	point is in the bound
		(*gDisplay) (x, y) = fadedColor;																						//	set the pixel to the color of the faded dot
	loc = last[1] - where;																												//	compute the drawing location for the last position of the particle
	last[1] = body->TLocation ();																									//	save the current position into the last space
	x = int (loc[X]);																															//	compute the x component of the integer coordinate of the dot
	y = int (loc[Y]);																															//	compute the y component of the integer coordinate of the dot
	if (gBound.TestClip (x, y) == INSIDE)																					//	if the	point is in the bound
		(*gDisplay) (x, y) = fadedColor;																						//	set the pixel to the color of the faded dot
	loc = body->TLocation () - where;																							//	compute the drawing location for the particle
	x = int (loc[X]);																															//	compute the x component of the integer coordinate of the dot
	y = int (loc[Y]);																															//	compute the y component of the integer coordinate of the dot
	if (gBound.TestClip (x, y) == INSIDE)																					//	if the	point is in the bound
		(*gDisplay) (x, y) = color;																									//	set the pixel to the color of the dot
}																																								//	end

//------------------------------------------------------------------------------
