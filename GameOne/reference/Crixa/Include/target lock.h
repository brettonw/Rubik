//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/17/97
//
//------------------------------------------------------------------------------

#ifndef	TARGET_LOCK
#define	TARGET_LOCK

//------------------------------------------------------------------------------
#include "lock.h"
#include "piece ptr.h"

//------------------------------------------------------------------------------
class TLock : public Lock																												//	a lock on a moving target
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				PiecePtr	target;																												//	pointer to the target piece
				PiecePtr	confirm;																											//	the confirmed pointer to the target
				Real			quality;																											//	how good the lock is
				Point_2d	trueLoc;																											//	temporary variable for the true location function
	public:																																				//	members available externally
				TLock (const PiecePtr &target, Real quality = R(1.0));									//	constructor
virtual	~TLock (void);																													//	destructor
virtual	CPoint		Location (Real lead = ZERO);																	//	return the lock location
virtual	CPoint		TrueLocation (Real lead = ZERO);															//	return the lock location without checking quality
virtual	Real			Radius (void);																								//	return the radius of the lock
virtual	Bool			Confirm (void);																								//	return whether or not the lock is valid
virtual	Real			Quality (void);																								//	return the lock quality
				PiecePtr	Target (void);																								//	return the confirmed target pointer
};																																							//	end

//------------------------------------------------------------------------------
inline	PiecePtr	TLock::Target (void)																					//	return the confirmed target pointer
{																																								//	begin
	return confirm;																																//	return the confirmed target
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	TARGET_LOCK
