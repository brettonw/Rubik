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
//	Last Revised: 12/24/96
//
//------------------------------------------------------------------------------

#ifndef	PIXEL_8
#define	PIXEL_8

//------------------------------------------------------------------------------
class Pixel																																			//	8 bit pixel class for screen operations
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				uchar		pixel;																													//	the actual data associated with the pixel
static	int			scaleRGB[6][32];																								//	lookup table for alpha scale values
static	int			colorLUT[6][6][6];																							//	color cube lookup table
static	int			inverseCLUT[256][3];																						//	lookup table for pixel to RGB conversion
static	int			clipLUT[11];																										//	lookup table for clipping
	public:																																				//	members available externally
  			Pixel (void) {}																													//	constructor
  			Pixel (const Pixel &pix);																								//	constructor
  			Pixel (int red, int green, int blue);																		//	constructor
  			Pixel (uchar pix);																											//	constructor
  			int				Red (void) const;																							//	return the red component of the pixel
  			int				Green (void) const;																						//	return the green component of the pixel
  			int				Blue (void) const;																						//	return the blue component of the pixel
  			void			operator () (int red, int green, int blue);										//	function call operator
  			void			operator = (const Pixel &pix);																//	assignment operator
  			void			operator = (uchar pix);																				//	assignment operator
  			int				operator == (const Pixel &pix) const;													//	equality operator
  			int				operator != (const Pixel &pix) const;													//	inequality operator
  			int				operator == (uchar pix) const;																//	equality operator
  			int				operator != (uchar pix) const;																//	inequality operator
  			void			operator += (const Pixel &pixel);															//	addition operator for two pixels
				Pixel			operator + (const Pixel &pix);																//	addition operator
				Pixel			Scale (uchar alpha);																					//	scale a pixel according to an alpha value
				Pixel			Scale (Real scale);																						//	scale a pixel to a fraction
 static	void			BuildAlphaTable (void);																				//	build the alpha scaling tables
 static void			BuildColorTable (void);																				//	build the color lookup tables
};																																							//	end

//------------------------------------------------------------------------------
inline	Pixel::Pixel (const Pixel &pix)																					//	constructor
{																																								//	begin
	pixel = pix.pixel;																														//	copy the new value
}																																								//	end

//------------------------------------------------------------------------------
inline	Pixel::Pixel (int red, int green, int blue)															//	constructor
{																																								//	begin
	pixel = colorLUT[red][green][blue];																						//	find the color in the color cube
}																																								//	end

//------------------------------------------------------------------------------
inline	Pixel::Pixel (uchar pix)																								//	constructor
{																																								//	begin
	pixel = pix;																																	//	copy the new pixel value
}																																								//	end

//------------------------------------------------------------------------------
inline	int		Pixel::Red (void) const																						//	return the red component of the pixel
{																																								//	begin
	return inverseCLUT[pixel][0];																									//	return the red component of the color
}																																								//	end

//------------------------------------------------------------------------------
inline	int		Pixel::Green (void) const																					//	return the green component of the pixel
{																																								//	begin
	return inverseCLUT[pixel][1];																									//	return the green component of the color
}																																								//	end

//------------------------------------------------------------------------------
inline	int		Pixel::Blue (void) const																					//	return the blue component of the pixel
{																																								//	begin
	return inverseCLUT[pixel][2];																									//	return the blue component of the color
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Pixel::operator () (int red, int green, int blue)									//	function call operator
{																																								//	begin
	pixel = colorLUT[red][green][blue];
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Pixel::operator = (const Pixel &pix)															//	assignment operator
{																																								//	begin
	pixel = pix.pixel;																														//	copy the new pixel value
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Pixel::operator = (uchar pix)																			//	assignment operator
{																																								//	begin
	pixel = pix;																																	//	copy the new pixel value
}																																								//	end

//------------------------------------------------------------------------------
inline	int		Pixel::operator == (const Pixel &pix) const												//	equality operator
{																																								//	begin
	return pixel == pix.pixel;																										//	return the results of the equality test
}																																								//	end

//------------------------------------------------------------------------------
inline	int		Pixel::operator != (const Pixel &pix) const												//	inequality operator
{																																								//	begin
	return pixel != pix.pixel;																										//	return the results of the inequality test
}																																								//	end

//------------------------------------------------------------------------------
inline	int		Pixel::operator == (uchar pix) const															//	equality operator
{																																								//	begin
	return pixel == pix;																													//	return the results of the equality test
}																																								//	end

//------------------------------------------------------------------------------
inline	int		Pixel::operator != (uchar pix) const															//	inequality operator
{																																								//	begin
	return pixel != pix;																													//	return the results of the inequality test
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Pixel::operator += (const Pixel &pix)															//	self addition operator
{																																								//	begin
	int		red = clipLUT[Red () + pix.Red ()],
				green = clipLUT[Green () + pix.Green ()],
				blue = clipLUT[Blue () + pix.Blue ()];
	pixel = colorLUT[red][green][blue];
}																																								//	end

//------------------------------------------------------------------------------
inline	Pixel	Pixel::operator + (const Pixel &pix)															//	addition operator
{																																								//	begin
	int		red = clipLUT[Red () + pix.Red ()],
				green = clipLUT[Green () + pix.Green ()],
				blue = clipLUT[Blue () + pix.Blue ()];
	return Pixel (red, green, blue);																							//	return the new pixel
}																																								//	end

//------------------------------------------------------------------------------
inline	Pixel	Pixel::Scale (uchar alpha)																				//	scale a pixel according to an alpha value
{																																								//	begin
	int		red = scaleRGB[Red ()][alpha],																					//	compute the new red value
				green = scaleRGB[Green ()][alpha],																			//	compute the new green value
				blue = scaleRGB[Blue ()][alpha];																				//	compute the new blue value
	return Pixel (red, green, blue);																							//	return the new pixel
}																																								//	end

//------------------------------------------------------------------------------
inline	Pixel	Pixel::Scale (Real scale)																					//	scale a pixel to a fraction
{																																								//	begin
	int		red = int ((Red () * scale) + R(0.5)),																	//	compute the new red value
				green = int ((Green () * scale) + R(0.5)),															//	compute the new green value
				blue = int ((Blue () * scale) + R(0.5));																//	compute the new blue value
	return Pixel (red, green, blue);																							//	return the new pixel
}																																								//	end

//------------------------------------------------------------------------------
#define	MakePixel(R, G, B)	Pixel ((R) / 6, (G) / 6, (B) / 6)										//	a 16-bit to 8-bit pixel conversion

//------------------------------------------------------------------------------
#define	PIXEL_SIZE		8																													//	the size of a pixel in bits
#define	PIXEL_SHIFT		0																													//	amount to shift a byte count to get a pixel count
#define	EMPTY_PIXEL		0																													//	an empty pixel color
typedef	Pixel		*pPtr;																													//	a pixel pointer

//------------------------------------------------------------------------------

#endif	//	PIXEL_8