//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/06/97
//
//	02/24/97	DS		Added AltDrawMode
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "sprite sequence.h"

//------------------------------------------------------------------------------
SpriteSeq::SpriteSeq (SArrayPtr array, int start)																//	constructor
{																																								//	begin
	sprites = array;																															//	copy the sprites array
	current = start;																															//	set the current sprite
}																																								//	end

//------------------------------------------------------------------------------
SpriteSeq::~SpriteSeq (void)																										//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Bool	SpriteSeq::Step (CPoint where)																						//	draw the next frame
{																																								//	begin
	sprites[current]->Draw (where);																								//	draw the current sprite
	return TRUE;																																	//	return true
}																																								//	end

//------------------------------------------------------------------------------
Bool	SpriteSeq::Step (CPoint where, int mode)																	//	draw the next frame
{																																								//	begin
	switch (mode)
	{
		case 0:	sprites[current]->Draw (where);																			//	draw normally
						break;
		case 1:	sprites[current]->DrawFlash (where);																//	draw all white
						break;
		case 2:	sprites[current]->DrawAdd (where);																	//	draw additive
	}
	return TRUE;																																	//	return true
}																																								//	end

//------------------------------------------------------------------------------
int		SpriteSeq::Width (void)																										//	return the width of the sequence
{																																								//	begin
	return sprites->Width ();																											//	return the width of the sprite array
}																																								//	end

//------------------------------------------------------------------------------
int		SpriteSeq::Height (void)																									//	return the height of the sequence
{																																								//	begin
	return sprites->Height ();																										//	returnt he height of the sprite array
}																																								//	end

//------------------------------------------------------------------------------
