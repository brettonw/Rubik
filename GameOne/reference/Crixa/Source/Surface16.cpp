//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	12/24/96
//
//	Modified by:		Dave Seah
//	Last Revision:	01/23/97			Added EraseRect()
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#ifdef	SIXTEENBITCOLOR

//------------------------------------------------------------------------------
#include "surface.h"
#include "debugger.h"

//------------------------------------------------------------------------------
Surface::Surface (LPDIRECTDRAWSURFACE surf)																			//	constructor
{																																								//	begin
	surface = surf;																																//	copy the direct draw surface
	DDSURFACEDESC	desc;																														//	a surface description for direct draw
	desc.dwSize = sizeof (desc);																									//	set the size of the description structure
	DXCheck (surface->Lock (0, &desc, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, 0));	//	if the surface is successfully locked
	array = new pPtr[desc.dwHeight];																							//	allocate the indexing array
	DXCheck (surface->Unlock (desc.lpSurface));																		//	unlock the surface
}																																								//	end

//------------------------------------------------------------------------------
Surface::~Surface (void)																												//	destructor
{																																								//	begin
	delete array;																																	//	free up the indexing array
}																																								//	end

//------------------------------------------------------------------------------
void	Surface::Erase (void)																											//	erase a rectangular region
{																																								//	begin
	RECT		bound = {0, 0, SCRN_HEIGHT, SCRN_HEIGHT};															//	rectangle to specify the erasable area
	DDBLTFX	ddbltfx;																															//	blit effects structure
	ddbltfx.dwSize = sizeof (ddbltfx);																						//	set the size of the blit effects structure
	ddbltfx.dwFillColor = 0;																											//	set the fill color to black
	DXCheck (surface->Blt (&bound, 0, 0, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx));//	call the (hardware?) blitter to erase the rectangle
}																																								//	end

//------------------------------------------------------------------------------
void	Surface::EraseRect (RECT &bound)																					//	ds: erase a specified rectangular region
{																																								//	begin
	DDBLTFX	ddbltfx;																															//	blit effects structure
	ddbltfx.dwSize = sizeof (ddbltfx);																						//	set the size of the blit effects structure
	ddbltfx.dwFillColor = 0;																											//	set the fill color to black
	DXCheck (surface->Blt (&bound, 0, 0, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx));//	call the (hardware?) blitter to erase the rectangle
}

//------------------------------------------------------------------------------
void	Surface::EraseAll (void)																									//	erase the entire surface area
{																																								//	begin
	DDBLTFX	ddbltfx;																															//	blit effects structure
	ddbltfx.dwSize = sizeof (ddbltfx);																						//	set the size of the blit effects structure
	ddbltfx.dwFillColor = 0;																											//	set the fill color to black
	DXCheck (surface->Blt (0, 0, 0, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx));			//	call the (hardware?) blitter to erase the rectangle
}																																								//	end

//------------------------------------------------------------------------------
void	Surface::Lock (Bool invert)																								//	prepare the surface for drawing
{																																								//	begin
	pPtr					base;																														//	a base pointer
	DDSURFACEDESC	desc;																														//	surface description for direct draw
	desc.dwSize = sizeof (desc);																									//	set the size of the surface description
	DXCheck (surface->Lock (0, &desc, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, 0));	//	if the surface is successfully locked
	ptr = desc.lpSurface;																													//	get the base pointer to the video memory
	base = pPtr (ptr);																														//	assign it to the base
	height = desc.dwHeight;																												//	get the height
	width = desc.dwWidth;																													//	get the width
	stride = desc.lPitch >> PIXEL_SHIFT;																					//	compute the stride
	if (invert)																																		//	if we want an inversed coordinate system
	{																																							//	begin
		base += (height - 1) * stride;																							//	put the base address at the last row of the array
		stride = -stride;																														//	negate the stride
	}																																							//	end
	for (int i = 0; i < height; i++)																							//	loop over all of the rows
		array[i] = base + (i * stride);																							//	computing the offset for that row
}																																								//	end

//------------------------------------------------------------------------------
void	Surface::Unlock (void)																										//	clean up after drawing
{																																								//	begin
	DXCheck (surface->Unlock (ptr));																							//	all done, unlock the surface
}																																								//	end

//------------------------------------------------------------------------------
void	Surface::DumpToFile (cstr filename)																				//	dump the screen contents to a file
{																																								//	begin
	FILE	*fp = fopen (filename, "wb");																						//	open the file for writing
	if (fp)																																				//	if successful
	{																																							//	begin
		Lock (FALSE);																																//	lock the surface
		ulong	*row = new ulong[width];																							//	allocate an array of longs for the dump
		for (int i = 0; i < height; i++)																						//	loop over all of the rows
		{																																						//	begin
			for (int j = 0; j < width; j++)																						//	loop over all of the pixels in the row
			{																																					//	begin
				ulong		red = array[i][j].Red () << 3,																	//	get the red channel
								green = array[i][j].Green () << 11,															//	get the green channel
								blue = array[i][j].Blue () << 19;																//	get the blue channel
				row[j] = red | green | blue;																						//	assign the long word pixel
			}																																					//	end
			fwrite (row, sizeof (ulong), width, fp);																	//	write the row to the file
		}																																						//	end
		delete[] row;																																//	release the row
		surface->Unlock (ptr);																											//	all done, unlock the surface
		fclose (fp);																																//	close the file
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	SIXTEENBITCOLOR
