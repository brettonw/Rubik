//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade 
//	Last Revision:	02/03/97
//
//	02/10/97	JB		broke out Damage into multiple sections 
//	02/13/97	JB		new constructor, Setup (rad)
//	02/21/97	DS		added piece death trigger 
//	02/24/97	DS		added piece flash flags
//	03/07/97	JB		modified Hurt to return an int (for checking life stuff).
//
//------------------------------------------------------------------------------

#ifndef	SHIP
#define	SHIP

//------------------------------------------------------------------------------
#include "piece.h"
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
class Ship : public Piece																												//	a ship piece
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Real			rDamping;																											//	how much to damp the rotational velocity at each step
				Real			maxTThrust;																										//	maximum translational thrust
				Real			maxRThrust;																										//	maximum rotational thrust
				Real			maxTVelocity;																									//	top speed for maximum handling capabilities
				Real			stun;																													//	the stun time
				Real			stunMax;																											//	the amount of stun time to set in an impact
				Bool			dead;																													//	whether or not the ship is dead
				int				numNozzles;																										//	how many nozzle points the ship has
				Point_2d	*nozzle;																											//	array of nozzle points
				SArrayPtr	thrSprite;																										//	pointer to the sprite to use for thrusting
				int				resources;																										//	the resource count

				int				deathTrigID;																									//	ds: for deathly things
				int				deathTrigType;																								//	ds:
				int				deathTrigMsg;																									//	ds:

				bool			flashed;																											//	ds: flashed last frame

	public:																																				//	members available externally
				Ship (Player *owner);
				Ship (Player *owner, Real radius, CPoint location, Real orientation,		//	constructor
							SArrayPtr sprite);																								//	constructor	(continued)
virtual	~Ship (void);																														//	destructor
virtual	Bool			Step (Real time);																							//	step the piece ahead in time
virtual	int				PostCollide (const PiecePtr &piece, Sect *sect);							//	do some work after a collision with another piece
virtual	void			Damage (const PiecePtr &piece, Real damage);									//	damage the piece
virtual	void			Stun (void);
virtual	int				Hurt (const PiecePtr &piece, Real damage);
virtual	void			AddEffects (int check);
virtual	void			DoScore (const PiecePtr &piece);
virtual	void			Explode (const PiecePtr &piece);
virtual	void			DropStuff (void);																							//	drop everything we are carrying
virtual	Real			TraceRay (CPoint origin, CVector direction);									//	return the time at which a ray intersects this piece
				void			DropResource (int type, int number);													//	drop a single resource
				void			AdjustThrust (Real tThrust, Real rThrust, Real d = R(1.0e2));	//	adjust the max thrust values
				Real			GetMaxSpeed (void);																						//	return the maximum handling speed of the ship
				void			SetDamping (Real damping);																		//	set the rotational damping
				void			Thrust (Real thrust);																					//	thrust the ship in the forward/backward axis
				void			Strafe (Real thrust);																					//	thrust the ship in the left/right axis
				void			Rotate (Real thrust);																					//	rotate the ship about its center of mass
				Real			Point (CVector vec);																					//	point the ship in a particular direction
				Real			PointAt (CPoint pt);																					//	point the ship at a selected location
				Real			Go (CVector axis);																						//	point the ship to go in a certain direction
				Real			GoTo (CPoint pt);																							//	point the ship to go to a certain location
				void			Go (CVector axis, Real &rotate, Real &thrust);								//	compute the necessary actions to move the ship along a specified velocity vector
				Bool			GoTo (CPoint pt, Real &rotate, Real &thrust, Real tol);				//	compute the necessary actions to move the ship to a specified point
				void			EliteGo (CVector axis, Real &thrust, Real &strafe);						//	compute the necessary actions to move the ship along a specified velocity vector
				Bool			EliteGoTo (CPoint pt, Real &thrust, Real &strafe, Real tol);	//	compute the necessary actions to move the ship to a specified point
				void			Stop (Real &thrust, Real &strafe);														//	compute the necessary actions to stop the ship
				void			StopX (Real &thrust, Real &strafe);														//	compute the necessary actions to stop the ship along the ship relative X axis of motion (left/right)
				void			StopY (Real &thrust, Real &strafe);														//	compute the necessary actions to stop the ship along the ship relative y axis of motion (forward/backward)
				void			SetStun (Real time);																					//	set the amount of time the ship is stunned by an impact
virtual	void			Stun (Real time);																							//	stun the piece
				void			SetNozzles (int count, ...);																	//	set the thrust nozzles
				void			SetThrustSprite (const SArrayPtr &thr);												//	set the thrust sprite
				int				GetResources (void);																					//	return the resource count
				void			AdjustResources (int adjust);																	//	adjust the number of resource packs held by the ship
				void			SetDeathTrigger (int trigID, int type = 0, int msg = 0);			//	ds: set the death trigger type
};																																							//	end

//------------------------------------------------------------------------------
inline	Real	Ship::GetMaxSpeed (void)																					//	return the maximum handling speed of the ship
{																																								//	begin
	return maxTVelocity;																													//	return the maximum velocity
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Ship::SetDeathTrigger (int trigID, int type, int msg)							//	set the death trigger
{																																								//	begin
	deathTrigID = trigID;
	deathTrigType = type;
	deathTrigMsg = msg;
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Ship::SetDamping (Real damping)																		//	set the rotational damping
{																																								//	begin
	rDamping = damping;																														//	copy the value
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Ship::SetStun (Real time)																					//	set the amount of time the ship is stunned by an impact
{																																								//	begin
	stunMax = time;																																//	set the max stun time
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Ship::SetThrustSprite (const SArrayPtr &thr)											//	set the thrust sprite
{																																								//	begin
	thrSprite = thr;																															//	copy the sprite pointer
}																																								//	end

//------------------------------------------------------------------------------
inline	int		Ship::GetResources (void)																					//	return the resource count
{																																								//	begin
	return resources;																															//	return the count
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Ship::AdjustResources (int adjust)																//	adjust the number of resource packs held by the ship
{																																								//	begin
	resources *= adjust;																													//	adjust the resource count
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	SHIP
