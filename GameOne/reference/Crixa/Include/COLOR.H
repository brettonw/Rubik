//------------------------------------------------------------------------------
//	Copyright ©1995-96 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	03/01/96
//
//------------------------------------------------------------------------------

#ifndef	COLOR
#define	COLOR

//------------------------------------------------------------------------------
//	Definition of a color
//------------------------------------------------------------------------------
struct	Color																																		//	a basic color type
{																																								//	begin
	uchar	alpha;																																	//	the transparency factor
	uchar	red;																																		//	red
	uchar	green;																																	//	green
	uchar	blue;																																		//	blue
	Color (void) {}																																//	constructor
	Color (uchar red, uchar green, uchar blue);																		//	constructor
	Color (uchar alpha, uchar red, uchar green, uchar blue);											//	constructor
};																																							//	end

//------------------------------------------------------------------------------
inline	Color::Color (uchar r, uchar g, uchar b)																//	constructor
{																																								//	begin
	alpha = 0;																																		//	clear the alpha
	red = r;																																			//	copy the red
	green = g;																																		//	copy the green
	blue = b;																																			//	copy the blue
}																																								//	end

//------------------------------------------------------------------------------
inline	Color::Color (uchar a, uchar r, uchar g, uchar b)												//	constructor
{																																								//	begin
	alpha = a;																																		//	copy the alpha
	red = r;																																			//	copy the red
	green = g;																																		//	copy the green
	blue = b;																																			//	copy the blue
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	COLOR
