//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/17/97
//
//------------------------------------------------------------------------------

#ifndef	LOCK
#define	LOCK

//------------------------------------------------------------------------------
#include "space.h"

//------------------------------------------------------------------------------
class Lock																																			//	a lock for tracking purposes
{																																								//	begin
	friend	class	LockPtr;																												//	the lock pointer class can play with my private parts
	private:																																			//	members internal to this class only
				int				refcount;																											//	the reference count for the lock
	protected:																																		//	members internal to this class hierarchy
				Point_2d	location;																											//	a location
	public:																																				//	members available externally
				Lock (CPoint loc);																											//	constructor
virtual	~Lock (void);																														//	destructor
virtual	CPoint		Location (Real lead = ZERO);																	//	return the lock location
virtual	CPoint		TrueLocation (Real lead = ZERO);															//	return the lock location without checking quality
virtual	Real			Radius (void);																								//	return the radius of the lock
virtual	Bool			Confirm (void);																								//	return whether or not the lock is valid
virtual	Real			Quality (void);																								//	return the lock quality
};																																							//	end

//------------------------------------------------------------------------------
inline	Lock::Lock (CPoint loc)																									//	constructor
{																																								//	begin
	location = loc;																																//	copy the location
	refcount = 0;																																	//	initialize the reference count
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	LOCK
