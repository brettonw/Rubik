//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/30/97
//	
//	02/22/97	DS		added __cdecl prefix to NewHandler for __fastcall compile
//									compatibility
//
//------------------------------------------------------------------------------

#ifndef	ERROR_MGR
#define	ERROR_MGR

//------------------------------------------------------------------------------
void	Error (cstr call, cstr file, int line);																		//	bail out on an error
int	__cdecl NewHandler (size_t size);																						//	function to call when we are out of memory

//------------------------------------------------------------------------------
#define	Assert(a)																																\
	if (!a)																																				\
	{																																							\
		SetLastError (0);																														\
		Error (#a, __FILE__, __LINE__);																							\
	}																																							//	macro to facilitate assertions
#define	Check(a, b)		b (a, #a, __FILE__, __LINE__)															//	macro to facilitate error checking

//------------------------------------------------------------------------------

#endif	//	ERROR_MGR
