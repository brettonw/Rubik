//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/17/97
//
//------------------------------------------------------------------------------

#ifndef	HASH
#define	HASH

//------------------------------------------------------------------------------
typedef	int	(*HashFunc) (const void *ptr, int mask);														//	a common hash function type

//------------------------------------------------------------------------------
int		HashText (const void *string, int mask);																	//	return a hash value for a text string
int		HashPtr (const void *ptr, int mask);																			//	return a hash value for a pointer
int		HashInt (const void *ptr, int mask);																			//	return a hash value for a small int

//------------------------------------------------------------------------------

#endif	//	HASH