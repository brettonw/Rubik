//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	07/25/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "semaphore.h"

//------------------------------------------------------------------------------
Bool	Semaphore::Grab (void)																										//	grab the semaphore
{																																								//	begin
	if (grabbed)																																	//	if the semaphore has already been grabbed
		return FALSE;																																//	return false to indicate failure
	
	grabbed = TRUE;																																//	set the grabbed flag
	return grabbed;																																//	return the grab

}																																								//	end

//------------------------------------------------------------------------------
void	Semaphore::Release (void)																									//	release the semaphore
{																																								//	begin
	grabbed = FALSE;																															//	clear the grabbed flag
}																																								//	end

//------------------------------------------------------------------------------
