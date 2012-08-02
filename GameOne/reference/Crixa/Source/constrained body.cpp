//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/25/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "constrained body.h"
#include "pi.h"

//------------------------------------------------------------------------------
ConBody::ConBody (CPoint loc, Real orient, Real mass, Real inertia) : 					//	constructor
Body (loc, orient, mass, inertia)																								//	constructor (continued)
{																																								//	begin
	tAxis (ZERO, ZERO);																														//	zero the constraint axis
}																																								//	end

//------------------------------------------------------------------------------
ConBody::~ConBody (void)																												//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	ConBody::ApplyForce (CVector force)																				//	apply a force to the body at the center of mass
{																																								//	begin
	tVelocity += ((force * overTInertia) | tAxis) * tAxis;												//	accumulate the change in translational velocity along the constraint axis
}																																								//	end

//------------------------------------------------------------------------------
void	ConBody::ApplyForce (CVector f, CPoint where)															//	apply a force to the body at some point distant from the center of mass
{																																								//	begin
	Vector_2d	normal = where - tLocation;																					//	compute the torque arm vector
	rVelocity += ((normal[X] * f[Y]) - (normal[Y] * f[X])) * overRInertia;				//	accumulate the change on rotational velocity
	tVelocity += ((f * overTInertia) | tAxis) * tAxis;														//	accumulate the change in translational velocity along the constraint axis
}																																								//	end

//------------------------------------------------------------------------------
