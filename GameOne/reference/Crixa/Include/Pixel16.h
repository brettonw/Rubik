//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/07/97
//
//------------------------------------------------------------------------------

#ifndef	PIXEL_16
#define	PIXEL_16

//------------------------------------------------------------------------------
class Pixel																																			//	pixel class for screen operations
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				ushort		blue:5;																												//	5 bits for the red component
				ushort		unused:1;																											//	1 bit of unused data
				ushort		green:5;																											//	5 bits for the green component
				ushort		red:5;																												//	5 bits for the blue component
 static	ushort			scale[32][32];																							//	lookup table for alpha scaling of pixels
	public:																																				//	members available externally
  			Pixel (void) {}																													//	constructor (empty)
  			Pixel (const Pixel &pix);																								//	constructor (copy)
  			Pixel (ushort red, ushort green, ushort blue);													//	constructor from three values
  			Pixel (ushort pix);																											//	constructor from a value
  			int				Red (void) const;																							//	return the red component of the pixel
  			int				Green (void) const;																						//	return the green component of the pixel
  			int				Blue (void) const;																						//	return the blue component of the pixel
  			void			operator () (ushort red, ushort green, ushort blue);					//	function call operator
  			void			operator = (const Pixel &pix);																//	assignment operator
  			void			operator = (ushort pix);																			//	assignment operator
  			int				operator == (const Pixel &pix) const;													//	equality operator
  			int				operator != (const Pixel &pix) const;													//	inequality operator
  			int				operator == (ushort pix) const;																//	equality operator
  			int				operator != (ushort pix) const;																//	inequality operator
  			void			operator += (const Pixel &pix);																//	self addition operator for two pixels
				Pixel			operator + (const Pixel &pix);																//	addition operator
				Pixel			Scale (uchar alpha);																					//	scale a pixel according to an alpha value
				Pixel			Scale (Real scale);																						//	scale a pixel to a fraction
				void			SwapByteOrder (void);																					//	swap the byte order of the pixel
 static	void			BuildAlphaTable (void);																				//	build the alpha scaling table
};																																							//	end

//------------------------------------------------------------------------------
inline	Pixel::Pixel (const Pixel &pix)																					//	constructor (copy)
{																																								//	begin
	*(ushort*) this = *(ushort*) &pix;																						//	copy the data field directly from the other pixel
}																																								//	end

//------------------------------------------------------------------------------
inline	Pixel::Pixel (ushort r, ushort g, ushort b)															//	constructor from three values
{																																								//	begin
	blue = b;																																			//	copy the blue component
	unused = 0;																																		//	clear the unused bit
	green = g;																																		//	copy the green component
	red = r;																																			//	copy the red component
}																																								//	end

//------------------------------------------------------------------------------
inline	Pixel::Pixel (ushort pix)																								//	constructor from an int
{																																								//	begin
	*(ushort*) this = pix;																												//	copy the short word into the pixel data space
}																																								//	end

//------------------------------------------------------------------------------
inline	int		Pixel::Red (void) const																						//	return the red component of the pixel
{																																								//	begin
	return red;																																		//	return the red component of the pixel
}																																								//	end

//------------------------------------------------------------------------------
inline	int		Pixel::Green (void) const																					//	return the green component of the pixel
{																																								//	begin
	return green;																																	//	return the green component of the pixel
}																																								//	end

//------------------------------------------------------------------------------
inline	int		Pixel::Blue (void) const																					//	return the blue component of the pixel
{																																								//	begin
	return blue;																																	//	return the blue component of the pixel
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Pixel::operator () (ushort r, ushort g, ushort b)									//	function call operator
{																																								//	begin
	blue = b;																																			//	copy the blue component
	unused = 0;																																		//	clear the unused bit
	green = g;																																		//	copy the green component
	red = r;																																			//	copy the red component
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Pixel::operator = (const Pixel &pix)															//	assignment operator
{																																								//	begin
	*(ushort*) this = *(ushort*) &pix;																						//	copy the pixel data space directly into mine
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Pixel::operator = (ushort pix)																		//	assignment operator
{																																								//	begin
	*(ushort*) this = pix;																												//	copy the short word directly into my pixel data space
}																																								//	end

//------------------------------------------------------------------------------
inline	int		Pixel::operator == (const Pixel &pix) const												//	equality operator
{																																								//	begin
	return *(ushort*) this == *(ushort*) &pix;																		//	return whether or not my data space is the same as the pixel
}																																								//	end

//------------------------------------------------------------------------------
inline	int		Pixel::operator != (const Pixel &pix) const												//	inequality operator
{																																								//	begin
	return *(ushort*) this != *(ushort*) &pix;																		//	return whether or not my data space is different from the pixel
}																																								//	end

//------------------------------------------------------------------------------
inline	int		Pixel::operator == (ushort pix) const															//	equality operator
{																																								//	begin
	return *(ushort*) this == pix;																								//	return whether or not my data space is the same as the short word
}																																								//	end

//------------------------------------------------------------------------------
inline	int		Pixel::operator != (ushort pix) const															//	inequality operator
{																																								//	begin
	return *(ushort*) this != pix;																								//	return whether or not my data space is different from the short word
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Pixel::operator += (const Pixel &pix)															//	self addition operator
{																																								//	begin
	ushort	r = red + pix.red,																										//	compute the red component
					g = green + pix.green,																								//	compute the green component
					b = blue + pix.blue;																									//	compute the blue component
	if (r > 0x001f) r = 0x001f;																										//	clamp the red component
	if (g > 0x001f) g = 0x001f;																										//	clamp the green component
	if (b > 0x001f) b = 0x001f;																										//	clamp the blue component
	blue = b;																																			//	copy the blue component
	green = g;																																		//	copy the green component
	red = r;																																			//	copy the red component
}																																								//	end

//------------------------------------------------------------------------------
inline	Pixel	Pixel::operator + (const Pixel &pix)															//	addition operator for two pixels
{																																								//	begin
	ushort	r = red + pix.red,																										//	compute the red component
					g = green + pix.green,																								//	compute the green component
					b = blue + pix.blue;																									//	compute the blue component
	if (r > 0x001f) r = 0x001f;																										//	clamp the red component
	if (g > 0x001f) g = 0x001f;																										//	clamp the green component
	if (b > 0x001f) b = 0x001f;																										//	clamp the blue component
	return Pixel (r, g, b);																												//	return a new pixel that is the sum of the two pixels
}																																								//	end

//------------------------------------------------------------------------------
inline	Pixel	Pixel::Scale (uchar alpha)																				//	scale a pixel according to an alpha value
{																																								//	begin
	return Pixel (scale[alpha][red], scale[alpha][green], scale[alpha][blue]);		//	return the new pixel as a scaled version of this pixel
}																																								//	end

//------------------------------------------------------------------------------
#define	MakePixel(R, G, B)	Pixel (R, G, B)																			//	a 16-bit pixel conversion provided for compatibility with 8-bit pixel

//------------------------------------------------------------------------------
#define	PIXEL_SIZE		16																												//	the size of a pixel in bits
#define	PIXEL_BYTES		2																													//	the size of a pixel in bytes
#define	PIXEL_SHIFT		1																													//	amount to shift a byte count to get a pixel count
#define	EMPTY_PIXEL		0																													//	an empty pixel color
typedef	Pixel		*pPtr;																													//	a pixel pointer

//------------------------------------------------------------------------------

#endif	//	PIXEL_16