//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	07/22/96
//
//------------------------------------------------------------------------------

#ifndef	BODY
#define	BODY

//------------------------------------------------------------------------------
#include "space.h"

//------------------------------------------------------------------------------
class Body																																			//	a body for the physics model
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Real			tInertia;																											//	translational inertia
				Real			overTInertia;																									//	1.0 / tInertia
				Point_2d	tLocation;																										//	translational location
				Vector_2d	tVelocity;																										//	translational velocity
				Real			rInertia;																											//	rotational inertia
				Real			overRInertia;																									//	1.0 / rInertia
				Real			rLocation;																										//	orientation
				Real			rVelocity;																										//	rotational velocity
	public:																																				//	members available externally
  			Body (CPoint loc, Real orient, Real tInertia, Real rInertia);						//	constructor
virtual	~Body (void);																														//	destructor
virtual	void			ApplyForce (CVector force);																		//	apply a force to the body at the center of mass
virtual	void			ApplyForce (CVector force, CPoint where);											//	apply a force to the body at some point distant from the center of mass
virtual	void			ApplyTorque (Real torque);																		//	apply a torque to the body
virtual	void			ApplyTorque (Real force, Real arm);														//	apply a force to the body at some distance, resulting in a torque
virtual	void			AddVelocity (CVector tVel, Real rVel);												//	add velocity to the body
virtual	void			Step (Real time);																							//	step the simulation forward in time
const		Point_2d	&TLocation (void) const;																			//	get the translational location of the body
const		Vector_2d	&TVelocity (void) const;																			//	get the translational velocity of the body
				Real			RLocation (void) const;																				//	get the orientation of the body
				Real			RVelocity (void) const;																				//	get the rotational velocity of the body
				Real			TInertia (void) const;																				//	get the translational inertia of the body
				Real			RInertia (void) const;																				//	get the rotational inertia of the body
				Real			OverTInertia (void) const;																		//	get the reciprocal of the translational inertia of the body
				Real			OverRInertia (void) const;																		//	get the reciprocal of the rotational inertia of the body
				Point_2d	TLocAt (Real time) const;																			//	get the location at a given time
				void			SetLocation (CPoint loc, Real orient);												//	set the location and orientation of the body
				void			SetLocation (CPoint loc);																			//	set the location of the body
};																																							//	end

//------------------------------------------------------------------------------
inline	const Point_2d	&Body::TLocation (void) const														//	get the translational location of the body
{																																								//	begin
	return tLocation;																															//	return the location
}																																								//	end

//------------------------------------------------------------------------------
inline	CVector Body::TVelocity (void) const																		//	get the translational velocity of the body
{																																								//	begin
	return tVelocity;																															//	return the velocity
}																																								//	end

//------------------------------------------------------------------------------
inline	Real	Body::RLocation (void) const																			//	get the orientation of the body
{																																								//	begin
	return rLocation;																															//	return the orientation
}																																								//	end

//------------------------------------------------------------------------------
inline	Real	Body::RVelocity (void) const																			//	get the rotational velocity of the body
{																																								//	begin
	return rVelocity;																															//	return the angular velocity
}																																								//	end

//------------------------------------------------------------------------------
inline	Real	Body::TInertia (void) const																				//	get the translational inertia of the body
{																																								//	begin
	return tInertia;																															//	return the mass
}																																								//	end

//------------------------------------------------------------------------------
inline	Real	Body::RInertia (void) const																				//	get the rotational inertia of the body
{																																								//	begin
	return rInertia;																															//	return the angular inertia
}																																								//	end

//------------------------------------------------------------------------------
inline	Real	Body::OverTInertia (void) const																		//	get the reciprocal of the translational inertia of the body
{																																								//	begin
	return overTInertia;																													//	return the mass
}																																								//	end

//------------------------------------------------------------------------------
inline	Real	Body::OverRInertia (void) const																		//	get the reciprocal of the rotational inertia of the body
{																																								//	begin
	return overRInertia;																													//	return the angular inertia
}																																								//	end

//------------------------------------------------------------------------------
inline	Point_2d	Body::TLocAt (Real time) const																//	get the location at a given time
{																																								//	begin
	return tLocation + (time * tVelocity);																				//	return the computed location
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Body::SetLocation (CPoint loc, Real orient)												//	set the location and orientation of the body
{																																								//	begin
	tLocation = loc;																															//	copy the location
	rLocation = orient;																														//	copy the orientation
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Body::SetLocation (CPoint loc)																		//	set the location of the body
{																																								//	begin
	tLocation = loc;																															//	copy the location
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	BODY
