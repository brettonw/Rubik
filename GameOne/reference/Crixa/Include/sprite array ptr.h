//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	03/25/96
//
//------------------------------------------------------------------------------

#ifndef	SPRITE_ARRAY_PTR
#define	SPRITE_ARRAY_PTR

//------------------------------------------------------------------------------
#include "sprite array.h"

//------------------------------------------------------------------------------
class SArrayPtr																																	//	sprite array pointer class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
  			SArray		*ptr;																													//	pointer to the sequence
	public:																																				//	members available externally
  			SArrayPtr (void);																												//	constructor
  			SArrayPtr (cstr name);																									//	constructor
  			SArrayPtr (const SArrayPtr &ptr);																				//	constructor
  			~SArrayPtr (void);																											//	destructor
				SArray		*operator -> (void) const;																		//	pointer dereference operator
				SArray		*operator () (void) const;																		//	function call operator
				Sprite		*operator [] (int i) const;																		//	return a sprite pointer
				SArrayPtr	&operator = (const SArrayPtr &ptr);														//	assignment operator
};																																							//	end

//------------------------------------------------------------------------------
inline	SArray	*SArrayPtr::operator -> (void) const														//	pointer dereference operator
{																																								//	begin
	return ptr;																																		//	return the pointer
}																																								//	end

//------------------------------------------------------------------------------
inline	SArray	*SArrayPtr::operator () (void) const														//	function call operator
{																																								//	begin
	return ptr;																																		//	return the pointer
}																																								//	end

//------------------------------------------------------------------------------
inline	Sprite	*SArrayPtr::operator [] (int i) const														//	return a sprite pointer
{																																								//	begin
	return ptr->GetSprite (i);																										//	return the requested sprite
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	SPRITE_ARRAY_PTR