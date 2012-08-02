//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	05/29/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "hash table.h"

//------------------------------------------------------------------------------
HashTable::HashTable (HashFunc hsh, int sz)																			//	constructor
{																																								//	begin
	hash = hsh;																																		//	copy the hash function
	size = sz;																																		//	copy the table size
	mask = size - 1;																															//	compute the hash mask
	table = new List[size];																												//	allocate the table of lists
}																																								//	end

//------------------------------------------------------------------------------
HashTable::~HashTable (void)																										//	destructor
{																																								//	begin
	delete[] table;																																//	release the table
}																																								//	end

//------------------------------------------------------------------------------
