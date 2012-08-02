//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/06/97
//
//	02/24/97	DS		Added AltDrawMode
//
//------------------------------------------------------------------------------

#ifndef	SPRITE_SEQUENCE
#define	SPRITE_SEQUENCE

//------------------------------------------------------------------------------
#include "sequence.h"
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
class SpriteSeq : public Sequence																								//	sequence of sprites
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
  			SArrayPtr	sprites;																											//	array of sprites
  			ushort		current;																											//	the current sprite
	public:																																				//	members available externally
  			SpriteSeq (SArrayPtr sprites, int start = 0);														//	constructor
virtual	~SpriteSeq (void);																											//	destructor
virtual	Bool			Step (CPoint where);																					//	draw the next frame
virtual	Bool			Step (CPoint where,int mode);																	//	draw the next frame
virtual	int				Width (void);																									//	return the width of the sequence
virtual	int				Height (void);																								//	return the height of the sequence
				ushort		Count (void);																									//	return the number of frame sin the sequence
				ushort		GetCurrent (void);																						//	return the current sprite
				void			SetCurrent (ushort which);																		//	set the current sprite
};																																							//	end

//------------------------------------------------------------------------------
inline	ushort	SpriteSeq::Count (void)																					//	return the number of frame sin the sequence
{																																								//	begin
	return sprites->Count ();																											//	return the sprite count
}																																								//	end

//------------------------------------------------------------------------------
inline	ushort	SpriteSeq::GetCurrent (void)																		//	return the current sprite
{																																								//	begin
	return current;																																//	return the sprite index
}																																								//	end

//------------------------------------------------------------------------------
inline	void	SpriteSeq::SetCurrent (ushort which)															//	set the current sprite
{																																								//	begin
	if (which < sprites->Count ())																								//	if the requested value is valid
		current = which;																														//	set the sprite index
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	SPRITE_SEQUENCE