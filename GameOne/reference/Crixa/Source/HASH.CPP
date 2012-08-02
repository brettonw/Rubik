//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/17/97
//	03/12/97	DS		Modified HashText to be case insensitive (I think)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "hash.h"

//------------------------------------------------------------------------------
int		HashText (const void *ptr, int mask)																			//	return a hash value for a text string
{																																								//	begin
	cstr	string = cstr (ptr);																										//	make a string pointer out of the ptr
	int		value = 0;																															//	start with a zero value
	while (*string)																																//	loop over the entire string
		value = (value << 1) ^ (((*string++)&0x1F) >> 2);														//	ds: compute a new value
	return value & mask;																													//	return the value masked to the table size
}																																								//	end

//------------------------------------------------------------------------------
int		HashPtr (const void *ptr, int mask)																				//	return a hash value for a pointer
{																																								//	begin
	cstr	string = cstr (&ptr);																										//	get a pointer to the pointer
	return (string[0] ^ string[1] ^ string[2] ^ string[3]) & mask;								//	return the hash value masked to the table size
}																																								//	end

//------------------------------------------------------------------------------
int		HashInt (const void *ptr, int mask)																				//	return a hash value for a small int
{																																								//	begin
	return (*(int *) ptr) & mask;																									//	merely mask the int and return the result
}																																								//	end

//------------------------------------------------------------------------------
