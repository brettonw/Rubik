//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/21/97
//
//------------------------------------------------------------------------------

#ifndef	SPRING_GUN
#define	SPRING_GUN

//------------------------------------------------------------------------------
#include "gun.h"
#include "piece ptr.h"
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
class SpringGun : public Gun																										//	a spring line gun
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				PiecePtr	target;
				Real			hold;																													//	elapsed time since the target was acquired
				Real			holdTime;
				Real			stunTime;																											//	stun time for the target
				SArrayPtr	splut;																												//	the effect to show when we hit something
	public:																																				//	members available externally
				SpringGun (Real rate, Real range, const PiecePtr &owner, Real hold,			//	constructor 
										Real stun, SArrayPtr splut, int shotSound);									//	constructor (continued)
virtual	~SpringGun (void);																											//	destructor
virtual	void			Reload (Real time);																						//	reload the weapon
virtual	void			Shoot (void);																									//	shoot the projectile
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	SPRING_GUN
