//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	05/29/96
//
//------------------------------------------------------------------------------

#ifndef	SPRITE_ARRAY_MGR
#define	SPRITE_ARRAY_MGR

//------------------------------------------------------------------------------
#include "sprite array ptr.h"
#include "hash table.h"

//------------------------------------------------------------------------------
class SArrayMgr																																	//	sprite array manager class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				HashTable	table;																												//	the hash table used by the manager
	public:																																				//	members available externally
  			SArrayMgr (int size = 128);																							//	constructor
				SArrayPtr	Fetch (cstr name, Bool save = TRUE);													//	return the sprite array ptr associated with 'name'
				void			UnTouch (void);																								//	untouch all of the loaded sprites
				void			CleanUp (void);																								//	unload all of the untouched sprite arrays
};																																							//	end

//------------------------------------------------------------------------------
extern	SArrayMgr	gSpriteMgr;																										//	global sprite array manager

//------------------------------------------------------------------------------

#endif	//	SPRITE_ARRAY_MGR