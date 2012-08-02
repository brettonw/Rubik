//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/19/97
//	02/23/97	DS		Added DrawFlash, DrawAdd support
//
//------------------------------------------------------------------------------

#ifndef	SPRITE
#define	SPRITE

//------------------------------------------------------------------------------
#include "space.h"
#include "bound.h"
#include "pixel.h"
#include "memory mapped file.h"

//------------------------------------------------------------------------------
class Sprite																																		//	sprite class for direct drawing
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				struct		Run																														//	a run of pixel data, with or without alpha channel
				{																																				//	begin
					ushort		x;																													//	the x offset of the run
					ushort		length;																											//	the length of the run
					pPtr			pixels;																											//	pointer to the pixel data
					uPtr			alpha;																											//	pointer to the alpha data
				};																																			//	end
				struct		Row																														//	a row of pixel data
				{																																				//	begin
					ulong			numRuns;																										//	the number of runs in the row
					Run				**runs;																											//	pointer to an array of pointers to runs
				};																																			//	end
				int				width;																												//	the width of the sprite
				int				height;																												//	the number of rows in the sprite
				Row				*rows;																												//	pointer to an array of rows
				Vector_2d	offset;																												//	the drawing offset from the sprite center
	public:																																				//	members available externally
  			Sprite (MemFile *file, const Vector_2d &offset);												//	constructor
  			~Sprite (void);																													//	destructor
				void		Draw (CPoint center, Bool clip = TRUE);													//	draw the sprite
				void		DrawUnclipped (int x, int y);																		//	draw the sprite without any clipping
				void		DrawClipped (int x, int y);																			//	draw the sprite clipped to the screen bound

				void		DrawFlash (CPoint center, Bool clip = TRUE);										//	draw the sprite
				void		DrawFlashUnclipped (int x, int y);															//	draw the sprite without any clipping
				void		DrawFlashClipped (int x, int y);																//	draw the sprite clipped to the screen bound
				void		DrawAdd (CPoint center, Bool clip = TRUE);											//	draw the sprite
				void		DrawAddUnclipped (int x, int y);																//	draw the sprite without any clipping
				void		DrawAddClipped (int x, int y);																	//	draw the sprite clipped to the screen bound
				int			Width (void);																										//	return the width
				int			Height (void);																									//	return the height
};																																							//	end

//------------------------------------------------------------------------------
inline	int		Sprite::Width (void)																							//	return the width
{																																								//	begin
	return width;																																	//	return the sprite width
}																																								//	end

//------------------------------------------------------------------------------
inline	int		Sprite::Height (void)																							//	return the height
{																																								//	begin
	return height;																																//	return the sprite height
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	SPRITE
