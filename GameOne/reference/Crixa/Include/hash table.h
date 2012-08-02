//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/17/97
//
//------------------------------------------------------------------------------

#ifndef	HASH_TABLE
#define	HASH_TABLE

//------------------------------------------------------------------------------
#include "list.h"
#include "hash.h"

//------------------------------------------------------------------------------
class HashTable																																	//	hash table class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				List			*table;																												//	pointer to a table of lists
				int				size;																													//	size of the hash table
				int				mask;																													//	the hash mask
				HashFunc	hash;																													//	the hash function to use
	public:																																				//	members available externally
  			HashTable (HashFunc	hash, int size);																		//	constructor
				~HashTable (void);																											//	destructor
				List			*Fetch (const void *ptr);																			//	return the list associated with 'ptr'
				List			*operator [] (int index);																			//	array indexing operator
				int				Size (void);																									//	return the size of the table
				int				Mask (void);																									//	return the size of the table minus 1
};																																							//	end

//------------------------------------------------------------------------------
inline	List	*HashTable::Fetch (const void *ptr)																//	return the list associated with 'ptr'
{																																								//	begin
	return &(table[(*hash) (ptr, mask)]);																					//	return a pointer to the indexed list
}																																								//	end

//------------------------------------------------------------------------------
inline	List	*HashTable::operator [] (int index)																//	array indexing operator
{																																								//	begin
	return &(table[index]);																												//	return a pointer to the indexed list
}																																								//	end

//------------------------------------------------------------------------------
inline	int		HashTable::Size (void)																						//	return the size of the table
{																																								//	begin
	return size;																																	//	return the size
}																																								//	end

//------------------------------------------------------------------------------
inline	int		HashTable::Mask (void)																						//	return the size of the table minus 1
{																																								//	begin
	return mask;																																	//	return the mask
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	HASH_TABLE