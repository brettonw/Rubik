//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/29/97
//
//------------------------------------------------------------------------------

#ifndef	SPRITE_ARRAY
#define	SPRITE_ARRAY

//------------------------------------------------------------------------------
#include "sprite.h"

//------------------------------------------------------------------------------
class SArray																																		//	array of sprites
{																																								//	begin
	friend	class	SArrayPtr;																											//	the sprite array pointer class can play with my private parts
	private:																																			//	members internal to this class only
  			int				refcount;																											//	reference count for the sequence
	protected:																																		//	members internal to this class hierarchy
  			ushort		count;																												//	how many sprites in the sequence
  			Sprite		**sprites;																										//	array of sprite pointers
  			MemFile		*buffer;																											//	the data buffer for the sprites
				ushort		width;																												//	width of the sprites
				ushort		height;																												//	height of the sprites
	public:																																				//	members available externally
  			SArray (cstr name);																											//	constructor
				~SArray (void);																													//	destructor
				ushort		Count (void) const;																						//	return the number of sprites in the array
				Sprite		*GetSprite (int i) const;																			//	return a pointer to the ith sprite
				Sprite		*operator [] (int i) const;																		//	return a pointer to the ith sprite
				int				Width (void);																									//	return the width of the sprite array
				int				Height (void);																								//	return the height of the sprite array
};																																							//	end

//------------------------------------------------------------------------------
inline	ushort	SArray::Count (void) const																			//	return the number of sprites in the array
{																																								//	begin
	return count;																																	//	return the count
}																																								//	end

//------------------------------------------------------------------------------
inline	Sprite	*SArray::GetSprite (int i) const																//	return a pointer to the ith sprite
{																																								//	begin
	return sprites[i];																														//	return the requested pointer
}																																								//	end

//------------------------------------------------------------------------------
inline	Sprite	*SArray::operator [] (int i) const															//	return a pointer to the ith sprite
{																																								//	begin
	return sprites[i];																														//	return the requested pointer
}																																								//	end

//------------------------------------------------------------------------------
inline	int		SArray::Width (void)																							//	return the width of the sprite array
{																																								//	begin
	return width;																																	//	return the width
}																																								//	end

//------------------------------------------------------------------------------
inline	int		SArray::Height (void)																							//	return the height of the sprite array
{																																								//	begin
	return height;																																//	return the height
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	SPRITE_ARRAY