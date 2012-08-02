//------------------------------------------------------------------------------
//	Copyright ©1995 qualia, inc. All rights reserved.
//
//	This code may not be copied or used in any way without the expressed
//	written permission of qualia, inc. Any violation will be prosecuted
//	to the full extent of the law.
//
//	Written by:	Bretton Wade
//	Revised by:	Jeremy Biddle
//
//	Last Revised:	12/24/96
//	
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#ifdef	EIGHTBITCOLOR

//------------------------------------------------------------------------------
#include "pixel8.h"

//------------------------------------------------------------------------------
int		Pixel::scaleRGB[6][32];																										//	(red/blue) lookup table for alpha scaling of pixels
int		Pixel::colorLUT[6][6][6];																									//	lookup table for 8-bit color cube
int		Pixel::inverseCLUT[256][3];																								//	inverse lookup table for palette -> RGB components
int		Pixel::clipLUT[11] = {0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 5};										//	lookup table for clipping

//------------------------------------------------------------------------------
void	Pixel::BuildAlphaTable (void)																							//	build the alpha scaling table
{																																								//	begin
	for (int color = 0; color < 6; color++)																				//	loop over the red/blue color values
		for (int alpha = 0; alpha < 32; alpha++)																		//	loop over the alpha values
			scaleRGB[color][alpha] = int (((color * alpha) / R(31.0)) + R(0.5));			//	compute the scaled value
}																																								//	end

//------------------------------------------------------------------------------
void	Pixel::BuildColorTable (void)																							//	build the 8-bit color cube
{																																								//	begin
	int	 col = 0;																																	//	first color index
	for (int red = 0; red < 6; red++)																							//	loop through the reds
		for (int green = 0; green < 6; green++)																			//	loop through the greens
			for (int blue = 0; blue < 6; blue++)																			//	loop through the blues
			{
				colorLUT[red][green][blue] = col;																				//	set the color index for this color
				inverseCLUT[col][0] = red;
				inverseCLUT[col][1] = green;
				inverseCLUT[col][2] = blue;
				col++;
			}
	for (int j = 0; j < 32; col++, j++)																						//	loop through the gray palette
		inverseCLUT[col][0] = inverseCLUT[col][1] = inverseCLUT[col][2] = int ((j / R(6.0)) + R(0.5));
	for (; col < 256; col++)
		inverseCLUT[col][0] = inverseCLUT[col][1] = inverseCLUT[col][2] = 0;
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	EIGHTBITCOLOR
