//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	12/31/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "body.h"
#include "pi.h"

//------------------------------------------------------------------------------
Body::Body (CPoint loc, Real orient, Real mass, Real inertia)										//	constructor
{																																								//	begin
	tLocation = loc;																															//	copy the location
	tVelocity (ZERO, ZERO);																												//	zero the velocity
	rLocation = orient;																														//	copy the orientation
	rVelocity = ZERO;																															//	zero the rotational velocity
	tInertia = mass;																															//	copy the mass
	if (tInertia == INFINITY)																											//	if the mass is vary large
		overTInertia = ZERO;																												//	zero the over mass value
	else																																					//	otherwise
		overTInertia = R(1.0) / tInertia;																						//	compute the reciprocal of the mass
	rInertia = inertia;																														//	copy the rotational inertia
	if (inertia == INFINITY)																											//	if the rotational inertia is very large
		overRInertia = ZERO;																												//	zero the over inertia value
	else																																					//	otherwise
		overRInertia = R(1.0) / rInertia;																						//	compute the reciprocal of the rotational inertia
}																																								//	end

//------------------------------------------------------------------------------
Body::~Body (void)																															//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	Body::ApplyForce (CVector force)																					//	apply a force to the body at the center of mass
{																																								//	begin
	tVelocity += force * overTInertia;																						//	accumulate the change in translational velocity
}																																								//	end

//------------------------------------------------------------------------------
void	Body::ApplyForce (CVector f, CPoint where)																//	apply a force to the body at some point distant from the center of mass
{																																								//	begin
	Vector_2d	normal = where - tLocation;																					//	compute the torque arm vector
	rVelocity += ((normal[X] * f[Y]) - (normal[Y] * f[X])) * overRInertia;				//	accumulate the change on rotational velocity
	tVelocity += f * overTInertia;																								//	accumulate the change in translational velocity
}																																								//	end

//------------------------------------------------------------------------------
void	Body::ApplyTorque (Real torque)																						//	apply a torque to the body
{																																								//	begin
	rVelocity += torque * overRInertia;																						//	accumulate the change in rotational velocity
}																																								//	end

//------------------------------------------------------------------------------
void	Body::ApplyTorque (Real force, Real arm)																	//	apply a force to the body at some distance, resulting in a torque
{																																								//	begin
	rVelocity += force * arm * overRInertia;																			//	accumulate the change in rotational velocity
}																																								//	end

//------------------------------------------------------------------------------
void	Body::AddVelocity (CVector tVel, Real rVel)																//	add velocity to the body
{																																								//	begin
	tVelocity += tVel;																														//	add the translational velocity
	rVelocity += rVel;																														//	add the rotational velocity
}																																								//	end

//------------------------------------------------------------------------------
void	Body::Step (Real time)																										//	step the simulation forward in time
{																																								//	begin
	tLocation += tVelocity * time;																								//	update the position
	rLocation += rVelocity * time;																								//	update the orientation
	while (rLocation >= TWO_PI)																										//	if the orientation is out of range
		rLocation -= TWO_PI;																												//	subtract one whole rotation until it is back in range
	while (rLocation < ZERO)																											//	if the orientation is out of range
		rLocation += TWO_PI;																												//	add one whole rotation until it is back in range
}																																								//	end

//------------------------------------------------------------------------------
