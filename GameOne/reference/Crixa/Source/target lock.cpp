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
#include "target lock.h"
#include "random.h"
#include "referee.h"

//------------------------------------------------------------------------------
TLock::TLock (const PiecePtr &targ, Real qual) : 																//	constructor
Lock (targ->GetBody ()->TLocation ())																						//	constructor (continued)
{																																								//	begin
	target = targ;																																//	copy the target
	quality = qual;																																//	copy the lock quality
	if (gRandom.RndReal () <= quality)																						//	generate a random number and compare it against the lock quality
		confirm = target;																														//	confirm the target
}																																								//	end

//------------------------------------------------------------------------------
TLock::~TLock (void)																														//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
CPoint	TLock::Location (Real lead)																							//	return the lock location
{																																								//	begin
	if (gRandom.RndReal () <= quality)																						//	generate a random number and compare it against the lock quality
	{																																							//	begin
		confirm = target;																														//	confirm the target
		location = target->GetBody ()->TLocAt (lead);																//	update the location from the target
	}																																							//	end
	else																																					//	otherwise, the quality check failed
		confirm = PiecePtr (0);																											//	unconfirm the target
	return Lock::Location ();																											//	do the parental thing
}																																								//	end

//------------------------------------------------------------------------------
CPoint	TLock::TrueLocation (Real lead)																					//	return the lock location without checking quality
{																																								//	begin
	trueLoc = target->GetBody ()->TLocAt (lead);																	//	update the location from the target
	return trueLoc;																																//	return the true location
}																																								//	end

//------------------------------------------------------------------------------
Real	TLock::Radius (void)																											//	return the radius of the lock
{																																								//	begin
	if (gRandom.RndReal () <= quality)																						//	generate a random number and compare it against the lock quality
		return target->GetGeometry ()->Bound ();																		//	return the target object bounding radius
	else																																					//	otherwise, the quality check failed
		return Lock::Radius ();																											//	return the default target radius
}																																								//	end

//------------------------------------------------------------------------------
Bool	TLock::Confirm (void)																											//	return whether or not the lock is valid
{																																								//	begin
	return gReferee->Confirm (target);																						//	return whether or not the target object is alive
}																																								//	end

//------------------------------------------------------------------------------
Real	TLock::Quality (void)																											//	return the lock quality
{																																								//	begin
	return quality;																																//	return the lock quality
}																																								//	end

//------------------------------------------------------------------------------
