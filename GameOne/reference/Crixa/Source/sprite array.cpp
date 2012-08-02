//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/29/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "sprite array.h"
#include "debugger.h"

//------------------------------------------------------------------------------
SArray::SArray (cstr name)																											//	constructor
{																																								//	begin
	refcount = 0;																																	//	set the reference count to 0
	count = 0;																																		//	zero the count before reading
	char	filename[64];																														//	place to hold the filename
	sprintf (filename, "%02d\\%s", PIXEL_SIZE, name);															//	build the filename
	buffer = new MemFile (filename);																							//	the buffer for the sprite
	if (buffer->Open ())																													//	if the buffer was properly built
	{																																							//	begin
		buffer->Read (&count, sizeof (ushort));																			//	the number of sprites in the sequence
		if (count > 256)
			DEBUG_HARD ("SArray: Sprite %s has suspiciously high count!\n",name);
		buffer->Read (&width, sizeof (ushort));																			//	read the width
		buffer->Read (&height, sizeof (ushort));																		//	read the height
		buffer->Move (sizeof (ushort));																							//	skip the unused field
		sprites = new Sprite*[count];																								//	allocate the sprite pointer array
		Vector_2d	offset = Vector_2d (width, height) * R(0.5);											//	compute the offset vector from the bottom left corner to the center point of the sprite
		for (int i = 0; i < count; i++)																							//	loop over all of the sprites
			sprites[i] = new Sprite (buffer, offset);																	//	create a new sprite
		buffer->Close ();																														//	close the buffer
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
SArray::~SArray (void)																													//	destructor
{																																								//	begin
	if (count)																																		//	if sprites were allocated
	{																																							//	begin
		for (int i = 0; i < count; i++)																							//	loop over all of the sprites
			delete sprites[i];																												//	delete the sprite
		delete[] sprites;																														//	delete the sprite pointer array
		delete buffer;																															//	delete the sprite data
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
