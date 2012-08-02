//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/17/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "lock.h"

//------------------------------------------------------------------------------
Lock::~Lock (void)																															//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
CPoint	Lock::Location (Real lead)																							//	return the lock location
{																																								//	begin
	return location;																															//	return the location
}																																								//	end

//------------------------------------------------------------------------------
CPoint	Lock::TrueLocation (Real lead)																					//	return the lock location without checking quality
{																																								//	begin
	return location;																															//	return the location
}																																								//	end

//------------------------------------------------------------------------------
Real	Lock::Radius (void)																												//	return the radius of the lock
{																																								//	begin
	return R(50.0);																																//	default lock radius
}																																								//	end

//------------------------------------------------------------------------------
Bool	Lock::Confirm (void)																											//	return whether or not the lock is valid
{																																								//	begin
	return TRUE;																																	//	a position lock is always valid
}																																								//	end

//------------------------------------------------------------------------------
Real	Lock::Quality (void)																											//	return the lock quality
{																																								//	begin
	return R(1.0);																																//	a position lock is always valid
}																																								//	end

//------------------------------------------------------------------------------
