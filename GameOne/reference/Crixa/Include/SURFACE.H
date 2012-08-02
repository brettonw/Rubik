//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/31/96
//
//------------------------------------------------------------------------------

#ifndef	SURFACE
#define	SURFACE

//------------------------------------------------------------------------------
#include "direct x.h"
#include "pixel.h"
#include "area.h"

//------------------------------------------------------------------------------
class Surface																																		//	screen class for direct drawing
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				int									width;																							//	the width of the surface
				int									height;																							//	the height of the surface
				int									stride;																							//	bytes to next line
				LPVOID							ptr;																								//	pointer to the top of the surface memory
				pPtr								*array;																							//	array of pointers to rows of pixels
				LPDIRECTDRAWSURFACE	surface;																						//	the direct draw surface
	public:																																				//	members available externally
  			Surface (LPDIRECTDRAWSURFACE surface);																	//	constructor
virtual	~Surface (void);																												//	destructor
    		void			Lock (Bool invert = TRUE);																		//	set up to draw to this surface
				void			Unlock (void);																								//	clean up after drawing
    		void			Erase (void);																									//	erase the surface
				void			EraseRect (RECT &r);
    		void			EraseAll (void);																							//	erase the entire surface area
				Pixel			operator () (int x, int y) const;															//	array indexing operator
				Pixel			&operator () (int x, int y);																	//	array indexing operator
    		pPtr			PPtr (int x, int y);																					//	return a pointer to the requested pixel
    		int				Stride (void);																								//	return the size of a line in pixel
				void			DumpToFile (cstr file);																				//	dump the screen contents to a file
};																																							//	end

//------------------------------------------------------------------------------
inline	Pixel	Surface::operator () (int x, int y) const													//	array indexing operator
{																																								//	begin
	return array[y][x];																														//	return the requested pixel
}																																								//	end

//------------------------------------------------------------------------------
inline	Pixel	&Surface::operator () (int x, int y)															//	array indexing operator
{																																								//	begin
	return array[y][x];																														//	return the requested pixel
}																																								//	end

//------------------------------------------------------------------------------
inline	pPtr	Surface::PPtr (int x, int y)																			//	return a pointer to the requested pixel
{																																								//	begin
	return array[y] + x;																													//	return the requested pointer
}																																								//	end

//------------------------------------------------------------------------------
inline	int		Surface::Stride (void)																						//	return the size of a line in pixels
{																																								//	begin
	return stride;																																//	return the stride
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	SURFACE