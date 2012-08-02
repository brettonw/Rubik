//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/12/96
//
//------------------------------------------------------------------------------

#ifndef	ROCKET_LAUNCHER
#define	ROCKET_LAUNCHER

//------------------------------------------------------------------------------
#include "gun.h"
#include "lock ptr.h"
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
class RocketL : public Gun																											//	a rocket launching weapon
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				SArrayPtr	rocketSpr;																										//	sprite array for the projectile
				LockPtr		lock;
				Real			tThrust;
				Real			rThrust;
	public:																																				//	members available externally
				RocketL (const PiecePtr &owner, Real rate, Real radius, CPoint muzzle, 	//	constructor
									Real damage, SArrayPtr rocket, Real tThrust, Real rThrust,		//	constructor (continued)
									int shotSound);																								//	constructor (continued)
virtual	~RocketL (void);																												//	destructor
virtual	void			SetLock (const LockPtr &lock);																//	set the weapon lock, if applicable
virtual	void			Shoot (void);																									//	shoot the projectile
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	ROCKET_LAUNCHER
