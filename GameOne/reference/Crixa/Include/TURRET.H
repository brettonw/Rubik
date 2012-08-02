//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	07/17/96
//
//	Revision:				02/07/97			Broke out explode() from step() JB
//
//------------------------------------------------------------------------------

#ifndef	TURRET
#define	TURRET

//------------------------------------------------------------------------------
#include "piece.h"
#include "sprite array ptr.h"
#include "weapon.h"

//------------------------------------------------------------------------------
class Turret : public Piece																											//	a gun turret piece
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Real			rDamping;																											//	how much to damp the rotational velocity at each step
				Real			maxRThrust;																										//	maximum rotational thrust
				Weapon		*weapon;																											//	pointer to the weapon
				Real			deadTime;
				Real			lastPop;
	public:																																				//	members available externally
				Turret (Player *own, Real radius, CPoint loc, Real rloc, SArrayPtr spr);//	constructor
virtual	~Turret (void);																													//	destructor
virtual	Bool			Step (Real time);																							//	step the piece ahead in time
virtual	void			Explode (void);																								//	explode the turret
virtual	int				PostCollide (const PiecePtr &piece, Sect *sect);							//	do some work after a collision with another piece
virtual	void			Damage (const PiecePtr &piece, Real damage);									//	damage the piece
virtual	Real			TraceRay (CPoint origin, CVector direction);									//	return the time at which a ray intersects this piece
				void			AdjustThrust (Real rThrust);																	//	adjust the max thrust value
				void			SetDamping (Real damping);																		//	set the rotational damping
				void			Rotate (Real thrust);																					//	rotate the ship about its center of mass
				Real			Point (CVector vec);																					//	point the ship in a particular direction
				Real			PointAt (CPoint pt);																					//	point the ship at a selected location
				void			SetWeapon (Weapon *weapon);																		//	set the weapon
				Weapon    *GetWeapon (void);																						//	get the turret's weapon
				Real			Range (void);																									//	return the range of the currently selected weapon
				Bool			Fire (void);																									//	fire the weapon
};																																							//	end

//------------------------------------------------------------------------------
inline	void	Turret::AdjustThrust (Real rThrust)																//	adjust the max thrust value
{																																								//	begin
	maxRThrust *= rThrust;																												//	adjust the rotational thrust
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Turret::SetDamping (Real damping)																	//	set the rotational damping
{																																								//	begin
	rDamping = damping;																														//	copy the value
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Turret::SetWeapon (Weapon *weap)																	//	set the weapon
{																																								//	begin
	weapon = weap;																																//	copy the pointer
}																																								//	end

//------------------------------------------------------------------------------
inline	Weapon *Turret::GetWeapon (void)																				//	get the weapon
{																																								//	begin
	return weapon;																																//	return the weapon
}																																								//	end

//------------------------------------------------------------------------------
inline	Real	Turret::Range (void)																							//	return the range of the currently selected weapon
{																																								//	begin
	if (weapon)																																		//	if the selected weapon is a valid weapon
		return weapon->Range ();																										//	return its range
	return ZERO;																																	//	return zero otherwise
}																																								//	end

//------------------------------------------------------------------------------
inline	Bool	Turret::Fire (void)																								//	fire the weapon
{																																								//	begin
	if (weapon)																																		//	if the selected weapon is a valid weapon and we are alive
		return weapon->Fire ();																											//	fire the current weapon and return the result
	return FALSE;																																	//	return false to indicate failure
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	TURRET
