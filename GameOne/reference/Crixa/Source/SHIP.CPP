//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	02/03/97
//
//	02/10/97	JB		broke out Damage into multiple sections
//	02/13/97	JB		new (empty) constructor -- for deriving
//	02/19/97	MK		Added pieceptr to PlaySound Call
//	02/21/97	DS		Added deathTrigID info to constructor, new access functions,
//									Damage() changes
//	03/06/97	DS		Mod Glowing Generic Resources
//	03/07/97	JB		modified Hurt to return the change in life
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "ship.h"
#include "rotating.h"
#include "circle.h"
#include "pi.h"
#include "direction.h"
#include "quadratic.h"
#include "math utility.h"
#include "view.h"
#include "ship explosion sequence.h"
#include "get parameters.h"

//	hacks for explosions
#include "particle sequence.h"
#include "time sequence.h"
#include "stationary sequence.h"
#include "moving sequence.h"
#include "particle sequence.h"
#include "bullet.h"
#include "direction.h"
#include "referee.h"
#include "sprite array mgr.h"
#include "random.h"
#include "pi.h"
#include "circle.h"
#include "sound mgr.h"

#include "pumping sequence.h"
#include "resource.h"

// access for triggering
#include "state mgr.h"

//------------------------------------------------------------------------------
Ship::Ship (Player *own, Real rad, CPoint loc, Real rloc,												//	constructor
						SArrayPtr spr) : Piece (own)																				//	constructor (continued)
{																																								//	begin
	rDamping = R(-2.0e-1);																												//	set a starting value for rotational damping
	maxTThrust = PI * rad * rad;																									//	compute the mass as the area of the plate
	maxRThrust = R(0.5) * maxTThrust * rad * rad;																	//	compute the rotational inertia of a solid sphere
	body = new Body (loc, rloc, maxTThrust, maxRThrust);													//	allocate the body
	visual = new Rotating (body, spr);																						//	allocate the normal visual
	geometry = new Circle (rad);																									//	allocate the geometry, a circle of specified radius
	stun = ZERO;																																	//	set the current stun time to 0
	stunMax = ZERO;																																//	set the stun max to a hundredth of a second
	dead = FALSE;																																	//	the ship is not dead
	numNozzles = 0;																																//	set the number of nozzle points to 0
	thrSprite = gSpriteMgr.Fetch ("eksthr");
	resources = ParmInteger (RESOURCE_SIZE);
	deathTrigID = 0;																															//	ds: default trigID means don't send
}																																								//	end

//------------------------------------------------------------------------------
Ship::Ship (Player *own) : Piece (own)
{
}

//------------------------------------------------------------------------------
Ship::~Ship (void)																															//	destructor
{																																								//	begin
	delete geometry;																															//	release the geometry
	delete visual;																																//	release the visual
	delete body;																																	//	release the body
}																																								//	end

//------------------------------------------------------------------------------
Bool	Ship::Step (Real time)																										//	step the piece ahead in time
{																																								//	begin
	body->Step (time);																														//	step the body ahead in time
	body->AddVelocity (Vector_2d (ZERO, ZERO), rDamping * body->RVelocity ());		//	damp the rotational velocity
	stun -= time;																																	//	subtract the stun time
	if (stun < ZERO)																															//	if the stun time goes negative
		stun = ZERO;																																//	clamp it to 0
	return life > ZERO ? TRUE : FALSE;																						//	return true if the piece is still alive
}																																								//	end

//------------------------------------------------------------------------------
int		Ship::PostCollide (const PiecePtr &piece, Sect *sect)											//	do some work after a collision with another piece
{																																								//	begin
	if ((piece->GetOwner () != owner) && (stun <= stunMax))												//	if this is not an associated piece and the ship is not already stunned
		stun = stunMax;																															//	add the ship stun time
	gSoundMgr->PlaySound (18, this);																							//	(MK) PiecePtr info added. play the bump sound
	return Piece::PostCollide (piece, sect);																			//	return the parental behavior
}																																								//	end

//------------------------------------------------------------------------------
void	Ship::Damage (const PiecePtr &piece, Real damage)
{
	Stun ();
	int check = Hurt (piece, damage);
	AddEffects (check);
	if ((!dead) && (life == ZERO))																								//	if we are dead, but haven't yet exploded
	{																																							//	begin
		dead = TRUE;																																//	set the dead flag
		DoScore (piece);
		Explode (piece);
		DropStuff ();
		if (deathTrigID)																														//	ds: celebrate ship death!
			gStateMgr->Trigger(deathTrigID, piece, deathTrigType, deathTrigMsg);			//	let state manager know we've died
	}
}

//------------------------------------------------------------------------------
void	Ship::Stun (void)
{
	if (stun <= stunMax)
		stun = stunMax;
}

//------------------------------------------------------------------------------
int	Ship::Hurt (const PiecePtr &piece, Real damage)
{
	GetVisual ()->SetMode (VISFLASH);																							//	blinky damage effect
	int check = int (life * R(5.0));
	Piece::Damage (piece, damage);
	check -= int (life * R(5.0));
	return check;
}

//------------------------------------------------------------------------------
void	Ship::AddEffects (int check)
{
	for (int i = 0; i < check; i++)
	{
		Real			radius = ((Circle *) geometry)->Radius ();													//	get the radius of the ship
		Vector_2d	offset (gRandom.RndReal () - R(0.5), gRandom.RndReal () - R(0.5));
		offset *= radius;
		SArrayPtr	sprite = gSpriteMgr.Fetch ((gRandom.RndInteger () & 0x01) ? "plasma" : "plasma2");
		PumpSeq	*pump = new PumpSeq (sprite, this, offset);
		gView->Install (pump, 0);
	}
}

//------------------------------------------------------------------------------
void	Ship::DoScore (const PiecePtr &piece)
{
	if (piece ())
	{
		Player	*pOwner = piece->GetOwner ();
		if (pOwner)
			if (pOwner != owner)
				pOwner->AddScore (1, 0, 0, 0, 0, value);
			else
				pOwner->AddScore (0, 0, 0, 0, 1, 0);
	}
	owner->AddScore (0, 1, 0, 0, 0, 0);
}

//------------------------------------------------------------------------------
void	Ship::Explode (const PiecePtr &)
{
	gView->Install (new ShipExpl (this));																					//	build the explosion
}

//------------------------------------------------------------------------------
Real	Ship::TraceRay (CPoint origin, CVector direction)													//	return the time at which a ray intersects this piece
{																																								//	begin
	Real			radius = ((Circle *) geometry)->Radius (),													//	get the radius of the ship
						root1, root2;																												//	place to fetch the roots
	if (Dist (origin - body->TLocation (), direction, radius, root1, root2))			//	if the ray intersects the ship
		if (root1 > ZERO)																														//	if the ray intersects the ship at some later time
			return root1;																															//	return that time
	return INFINITY;																															//	return no intersection for all unhandled cases
}																																								//	end

//------------------------------------------------------------------------------
void	Ship::DropStuff (void)																										//	drop everything we are carrying
{																																								//	begin
	int	packet = ParmInteger (RESOURCE_SIZE),
			count = resources / packet;
	if (count > 8)
		count = 8;
	for (int i = 0; i < count; i++)
		DropResource (RESOURCE_GENERIC, packet);
	resources = 0;
}																																								//	end

//------------------------------------------------------------------------------
void	Ship::DropResource (int type, int number)																	//	drop a single resource
{																																								//	begin
	Vector_2d	vector = gDirection[gRandom.RndInteger ()][0];
	Point_2d	location = body->TLocation () + vector;
 	Resource *resource;
	if (type == RESOURCE_GENERIC)
	{
		resource = new Resource  (R(5.0), location, R(0.0), gSpriteMgr.Fetch ("flare"), type, number, R(3.0) + (gRandom.RndReal () * R(2.0)));
		(resource->GetVisual())->SetMode(VISADD);																		//	use additive draw
	}
	else
		resource = new Resource  (R(4.0), location, R(0.0), gSpriteMgr.Fetch ("res"), type, number);
	Body	*bd = resource->GetBody ();
	bd->AddVelocity (body->TVelocity (), ZERO);
	bd->ApplyForce (vector * gRandom.RndReal () * R(8.0e3));
	gReferee->Install (resource, MOBILE_INTERSECT);
}																																								//	end

//------------------------------------------------------------------------------
void	Ship::AdjustThrust (Real tThrust, Real rThrust, Real dist)								//	adjust the max thrust values
{																																								//	begin
	maxTThrust *= tThrust;																												//	adjust the translational thrust
	maxRThrust *= rThrust;																												//	adjust the rotational thrust
	Real	stopTime = SQRT (R(2.0) * dist / (tThrust * gReferee->Interval ()));		//	compute the number of frames to travel the stop distance at maximum acceleration
	maxTVelocity = tThrust * FLOOR (stopTime + R(0.5));														//	compute an estimate of the maximum handling speed per frame
}																																								//	end

//------------------------------------------------------------------------------
void	Ship::Thrust (Real thrust)																								//	thrust the ship in the forward/backward axis
{																																								//	begin
	if (stun <= ZERO)																															//	if the ship is not stunned
	{																																							//	begin
		Rotating	*rot = (Rotating *) visual;																				//	get a pointer to the rotating visual
		CVector		norm = rot->GetDirection ()[0],																		//	get the vector for the facing direction
							perp = rot->GetDirection ()[1];																		//	get the perpendicular vector to the norm vector
		Real			showThrust = FABS (thrust);
		body->ApplyForce (norm * (thrust * maxTThrust));														//	apply the thrust to the body along the thrust vector
		if ((showThrust > ZERO) && (showThrust < R(0.5)))
			showThrust = R(0.5);
		int				numParticles = int (R(2.0) * showThrust);
		if (FABS (thrust) > R(1.25))
			thrust = Sign (thrust) * R(1.25);
		for (int j = 0; j < numNozzles; j++)
		{
			Vector_2d	nozzleOffset = (perp * nozzle[j][X]) + (norm * nozzle[j][Y]);		//	compute the offset of the nozzle point
			Point_2d	nozzlePoint = body->TLocation () + nozzleOffset;
			for (int i = 0; i < numParticles; i++)
			{
				Vector_2d	nvec = norm * gRandom.RndReal () * thrust,
									pvec = perp * (R(0.25) * (gRandom.RndReal () + R(-0.5)));
				Vector_2d	rvec = ((nvec + pvec) * R(-3.0e2)) + body->TVelocity ();
				SeqPtr		seq = new MoveSeq (	new TimeSeq (thrSprite), nozzlePoint, FALSE, 3, 
																			Vector_2d (ZERO, ZERO), gReferee->Interval (), 
																			rvec, R(0.5), 
																			Vector_2d (ZERO, ZERO));
				gView->Install (seq, UNDER_LAYER);
			}
		}
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
void	Ship::Strafe (Real thrust)																								//	thrust the ship in the left/right axis
{																																								//	begin
	if (stun <= ZERO)																															//	if the ship is not stunned
	{																																							//	begin
		Rotating	*rot = (Rotating *) visual;																				//	get a pointer to the rotating visual
		CVector		norm = rot->GetDirection ()[0],																		//	get the vector for the facing direction
							perp = rot->GetDirection ()[1];																		//	get the perpendicular vector to the norm vector
		Real			showThrust = FABS (thrust);
		body->ApplyForce (perp * (thrust * maxTThrust));														//	apply the thrust to the body along the thrust vector
		if ((showThrust > ZERO) && (showThrust < R(0.5)))
			showThrust = R(0.5);
		int				numParticles = int (R(2.0) * showThrust);
		if (FABS (thrust) > R(1.25))
			thrust = Sign (thrust) * R(1.25);
		for (int j = 0; j < numNozzles; j++)
		{
			Vector_2d	nozzleOffset = (perp * nozzle[j][X]) + (norm * nozzle[j][Y]);		//	compute the offset of the nozzle point
			Point_2d	nozzlePoint = body->TLocation () + nozzleOffset;
			for (int i = 0; i < numParticles; i++)
			{
				Vector_2d	nvec = perp * gRandom.RndReal () * thrust,
									pvec = norm * (R(0.25) * (gRandom.RndReal () + R(-0.5)));
				Vector_2d	rvec = ((nvec + pvec) * R(-3.0e2)) + body->TVelocity ();
				SeqPtr		seq = new MoveSeq (	new TimeSeq (thrSprite), nozzlePoint, FALSE, 3, 
																			Vector_2d (ZERO, ZERO), gReferee->Interval (), 
																			rvec, R(0.5), 
																			Vector_2d (ZERO, ZERO));
				gView->Install (seq, UNDER_LAYER);
			}
		}
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
void	Ship::Rotate (Real thrust)																								//	rotate the ship about its center of mass
{																																								//	begin
	if (stun <= ZERO)																															//	if the ship is not stunned
		body->ApplyTorque (thrust * maxRThrust);																		//	apply the scaled torque to the body
}																																								//	end

//------------------------------------------------------------------------------
Real	Ship::Point (CVector vec)																									//	return the amount of thrust necessary to rotate the ship to a given vector
{																																								//	begin
	Real	angle = ATAN2 (vec[Y], vec[X]),																					//	compute the angle of the goal vector
				delta = angle - body->RLocation ();																			//	compute the difference between that and the current location
	while (delta > PI)																														//	while the angle is out of range
		delta -= TWO_PI;																														//	subtract 2 PI to bring it into range
	while (delta < -PI)																														//	while the angle is out of range
		delta += TWO_PI;																														//	add two pi to bring it into range
	Real	deltaVelocity = (delta / gReferee->Interval ()) - body->RVelocity (),		//	compute the velocity change necessary to bring the angle to the desired one in the next frame
				thrust = (body->RInertia () * deltaVelocity) / maxRThrust;							//	compute the fraction of thrust necessary to achieve the velocity change
	if (FABS (thrust) > R(1.0))																										//	if the fraction is greater than unit value
		thrust = Sign (thrust);																											//	clamp it
	return thrust;																																//	return the thrust fraction
}																																								//	end

//------------------------------------------------------------------------------
Real	Ship::PointAt (CPoint pt)																									//	point the ship at a selected location
{																																								//	begin
	return Point (pt - body->TLocation ());																				//	point at the difference vector betweent the point and my location
}																																								//	end

//------------------------------------------------------------------------------
Real	Ship::Go (CVector ax)																											//	point the ship to go in a certain direction
{																																								//	begin
	Vector_2d	axis = ax;																													//	copy the goal axis
	if (axis.Normalize () > ZERO)																									//	make it length one, if this is successful
	{																																							//	make it length one
		if ((axis | body->TVelocity ()) > ZERO)																			//	if the velocity along the axis is positive
			return Point (-Reflect (body->TVelocity (), axis));												//	the pointing direction to obtain that motion is the reflection of the current direction about the axis
		return Point (-body->TVelocity () + axis);																	//	otherwise, we have to gradually bring ourselves into line
	}																																							//	end
	return ZERO;																																	//	return zero to indicate no action is necessary
}																																								//	end

//------------------------------------------------------------------------------
Real	Ship::GoTo (CPoint pt)																										//	point the ship to go to a certain location
{																																								//	begin
	return Go (pt - body->TLocation ());																					//	go along the difference vector between the point and my location
}																																								//	end

//------------------------------------------------------------------------------
void	Ship::Go (CVector ax, Real &rotate, Real &thrust)													//	compute the necessary actions to move the ship at a spoecified velocity vector
{																																								//	begin
	Vector_2d	axis = ax;
	Real			dist = axis.Normalize ();
	if (dist > maxTVelocity)
		dist = maxTVelocity;
	Vector_2d	goal = (axis * dist) - body->TVelocity ();													//	compute the goal axis to thrust along
	Real			goalVel = goal.Normalize ();																				//	compute the goal acceleration and normalize the vector
	rotate = ZERO;																																//	don't rotate
	thrust = ZERO;																																//	don't thrust
	if (goalVel > EPSILON)																												//	if we really want to move
	{																																							//	begin
		rotate = Point (goal);																											//	point the ship at the goal vector
		if (rotate < R(1.0))																												//	if we are really close to pointing the right way
		{																																						//	begin
			if (goalVel > maxTVelocity)																								//	if the goal vel is faster than the handling speed of the ship
				goalVel = maxTVelocity;																									//	clamp it down
			thrust = (body->TInertia () * goalVel) / maxTThrust;											//	compute the normalized thrust to obtain the desired acceleration
			if (FABS (thrust) > R(1.0))																								//	if the thrust is greater than the max value
				thrust = Sign (thrust);																									//	clamp it
		}																																						//	end
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
Bool	Ship::GoTo (CPoint pt, Real &rotate, Real &thrust, Real tolerance)				//	compute the necessary actions to move the ship to a specified point
{																																								//	begin
	Vector_2d	axis = pt - body->TLocation ();																			//	compute the goal axis
	if ((axis | axis) > (tolerance * tolerance))																	//	if the ship is more than it's tolerance from the goal point
	{																																							//	begin
		Go (axis, rotate, thrust);																									//	go along the axis between the body and the point
		return TRUE;																																//	return true to indicate that we are travelling
	}																																							//	end
	else																																					//	otherwise, we are pretty much where we want to be
	{																																							//	begin
		rotate = ZERO;																															//	don't rotate
		thrust = ZERO;																															//	don't thrust
		return FALSE;																																//	return false to indicate that no action was taken
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
void	Ship::EliteGo (CVector axis, Real &thrust, Real &strafe)									//	compute the necessary actions to move the ship at a spoecified velocity vector
{																																								//	begin
	Vector_2d	goal = axis - body->TVelocity ();																		//	compute the goal axis for thrusting
	Real			goalVel = goal.Normalize ();																				//	compute the goal acceleration
	if (goalVel < EPSILON)																												//	if the goal acceleration is small
		goalVel = ZERO;																															//	make it zero
	if (goalVel > maxTVelocity)																										//	if the goal vel is faster than the handling speed of the ship
		goalVel = maxTVelocity;																											//	clamp it down
	thrust = body->TInertia () * goalVel;																					//	compute the thrust required to obtain the goal acceleration
	Rotating	*rot = (Rotating *) visual;																					//	get a pointer to the rotating visual
	Real			dot = goal | rot->GetDirection ()[1];																//	project the goal vector onto the strafe direction
	strafe = (thrust * dot) / maxTThrust;																					//	compute the normalized strafing force
	if (FABS (strafe) > R(1.0))																										//	if strafe is greater than the max
		strafe = Sign (strafe);																											//	clamp it
	dot = goal | rot->GetDirection ()[0];																					//	project th egoal vector onto the thrust direction
	thrust = (thrust * dot) / maxTThrust;																					//	compute the normalized thrusting force
	if (FABS (thrust) > R(1.0))																										//	if thrust is greater than the max
		thrust = Sign (thrust);																											//	clamp it
}																																								//	end

//------------------------------------------------------------------------------
Bool	Ship::EliteGoTo (CPoint pt, Real &thrust, Real &strafe, Real tolerance)		//	compute the necessary actions to move the ship to a specified point
{																																								//	begin
	Vector_2d	axis = pt - body->TLocation ();																			//	compute the goal axis
	if ((axis | axis) > (tolerance * tolerance))																	//	if the ship is more than it's tolerance from the goal point
	{																																							//	begin
		EliteGo (axis, thrust, strafe);																							//	go along the axis between the body and the point
		return TRUE;																																//	return true to indicate that we are travelling
	}																																							//	end
	else																																					//	otherwise, we are pretty much where we want to be
	{																																							//	begin
		thrust = ZERO;																															//	don't thrust
		strafe = ZERO;																															//	don't strafe
		return FALSE;																																//	return false to indicate that no action was taken
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
void	Ship::Stop (Real &thrust, Real &strafe)																		//	compute the necessary actions to stop the ship
{																																								//	begin
	if ((body->TVelocity () | body->TVelocity ()) > EPSILON)											//	if the object is moving
		EliteGo (Vector_2d (ZERO, ZERO), thrust, strafe);														//	go for a target velocity of zero
}																																								//	end

//------------------------------------------------------------------------------
void	Ship::StopX (Real &thrust, Real &strafe)																	//	compute the necessary actions to stop the ship along the ship relative X axis of motion (left/right)
{																																								//	begin
	Rotating	*rot = (Rotating *) visual;																					//	get a pointer to the rotating visual
	CVector		norm = rot->GetDirection ()[0],																			//	get the vector for the facing direction
						perp = rot->GetDirection ()[1];																			//	get the perpendicular vector to the norm vector
	Vector_2d	goal = body->TVelocity () | norm,
						stop = body->TVelocity () | perp;
	if ((stop | stop) > EPSILON)																									//	if the object is moving
		EliteGo (goal, thrust, strafe);																							//	go for a target velocity of zero
}																																								//	end

//------------------------------------------------------------------------------
void	Ship::StopY (Real &thrust, Real &strafe)																	//	compute the necessary actions to stop the ship along the ship relative y axis of motion (forward/backward)
{																																								//	begin
	Rotating	*rot = (Rotating *) visual;																					//	get a pointer to the rotating visual
	CVector		norm = rot->GetDirection ()[0],																			//	get the vector for the facing direction
						perp = rot->GetDirection ()[1];																			//	get the perpendicular vector to the norm vector
	Vector_2d	goal = body->TVelocity () | perp,
						stop = body->TVelocity () | norm;
	if ((stop | stop) > EPSILON)																									//	if the object is moving
		EliteGo (goal, thrust, strafe);																							//	go for a target velocity of zero
}																																								//	end

//------------------------------------------------------------------------------
void	Ship::Stun (Real time)																										//	stun the piece
{																																								//	begin
	stun = time;																																	//	stun the ship
}																																								//	end

//------------------------------------------------------------------------------
void	Ship::SetNozzles (int count, ...)																					//	set the thrust nozzles
{																																								//	begin
	numNozzles = count;
	nozzle = new Point_2d[count];
	va_list	var;																																	//	variable argument list
	va_start (var, count);																												//	set up the variable argument list
	for (int i = 0; i < count; i++)
		nozzle[i] = va_arg (var, Point_2d);
	va_end (var);																																	//	clean up the argument list
}																																								//	end

//------------------------------------------------------------------------------
