//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/07/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#ifdef	SIXTEENBITCOLOR

//------------------------------------------------------------------------------
#include "pixel16.h"

//------------------------------------------------------------------------------
ushort		Pixel::scale[32][32];																									//	lookup table for alpha scaling of pixels

//------------------------------------------------------------------------------
Pixel	Pixel::Scale (Real scale)																									//	scale a pixel to a fraction
{																																								//	begin
	ushort	r = int ((red * scale) + R(0.5)),																			//	compute the new red component
					g = int ((green * scale) + R(0.5)),																		//	compute the new green component
					b = int ((blue * scale) + R(0.5));																		//	compute the new blue component
	return Pixel (r, g, b);																												//	return the new pixel as a scaled version of this pixel
}																																								//	end

//------------------------------------------------------------------------------
void	Pixel::SwapByteOrder (void)																								//	swap the byte order of the pixel
{																																								//	begin
	uchar	*byte = (uchar *) this,																									//	get a pointer to the two bytes that compose the pixel
				tmp = byte[0];																													//	save the first byte
	byte[0] = byte[1];																														//	copy the second byte to the first location
	byte[1] = tmp;																																//	copy the first byte to the second location
}																																								//	end

//------------------------------------------------------------------------------
void	Pixel::BuildAlphaTable (void)																							//	build the alpha scaling table
{																																								//	begin
	for (int i = 0; i < 32; i++)																									//	loop over all of the color values
		for (int j = 0; j < 32; j++)																								//	loop over all of the alpha values
			scale [i][j] = ushort (((i * j) / R(31.0)) + R(0.5));											//	compute the scaled value
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	SIXTEENBITCOLOR
