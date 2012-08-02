//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/06/96
//
//	02/24/97	DS		Added AltDrawMode
//
//------------------------------------------------------------------------------

#ifndef	ROTATING
#define	ROTATING

//------------------------------------------------------------------------------
#include "visual.h"
#include "sprite sequence.h"
#include "direction.h"

//------------------------------------------------------------------------------
class Rotating : public Visual																									//	visual for a rotating body
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				SpriteSeq	sprites;																											//	sequence of rotated sprites
				int				shift;																												//	how many bits to shift an index into the directions table
	public:																																				//	members available externally
				Rotating (Body *body, SArrayPtr sprites);																//	constructor
virtual	~Rotating (void);																												//	destructor
virtual	void			Draw (CPoint where);																					//	draw the object
				void			Update (void);																								//	update the visual
				ushort		GetCurrent (void);																						//	return the current sprite index
				int				Shift (void);																									//	return the number of bits to shift an index into the directions table
				Vector_2d	*GetDirection (void);																					//	return the current direction vector for the rotating visual
};																																							//	end

//------------------------------------------------------------------------------
inline	ushort	Rotating::GetCurrent (void)																			//	return the current sprite index
{																																								//	begin
	return sprites.GetCurrent ();																									//	return the current sprite
}																																								//	end

//------------------------------------------------------------------------------
inline	int		Rotating::Shift (void)																						//	return the number of bits to shift an index into the directions table
{																																								//	begin
	return shift;																																	//	return the bit shift value
}																																								//	end

//------------------------------------------------------------------------------
inline	Vector_2d	*Rotating::GetDirection (void)																//	return the current direction vector for the rotating visual
{																																								//	begin
	return gDirection[sprites.GetCurrent () << shift];														//	return the pointer to the direction vectors
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	ROTATING