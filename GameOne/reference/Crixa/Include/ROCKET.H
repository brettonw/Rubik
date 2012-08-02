//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	09/05/96
//
//  Modified:       Mark E. Kern
//									02/12/97						Added parm to hold sound id for rocket "rockEMid"
//------------------------------------------------------------------------------

#ifndef	ROCKET
#define	ROCKET

//------------------------------------------------------------------------------
#include "piece.h"
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
class Rocket : public Piece																											//	a rocket piece
{																																								//	begin
	private:																																			//	members internal to this class only
				long			rockEMid;																											//  unique rocket sound id for emitter
	protected:																																		//	members internal to this class hierarchy
				Real			maxTThrust;																										//	maximum translational thrust
				Real			maxRThrust;																										//	maximum rotational thrust
				Real			damage;
				SArrayPtr	thrustSprite;
	public:																																				//	members available externally
				Rocket (Player *owner, Real radius, CPoint location, Real orientation,	//	constructor
							SArrayPtr sprite, SArrayPtr thrust, Real damage);									//	constructor	(continued)
virtual	~Rocket (void);																													//	destructor
virtual	int				PreCollide (const PiecePtr &piece, Sect *sect);								//	do some work before a collision with another piece
virtual	int				PostCollide (const PiecePtr &piece, Sect *sect);							//	do some work after a collision with another piece
virtual	Real			TraceRay (CPoint origin, CVector direction);									//	return the time at which a ray intersects this piece
				void			AdjustThrust (Real tThrust, Real rThrust);										//	adjust the max thrust values
				void			Thrust (Real thrust);																					//	thrust the ship in the forward/backward axis
				void			Rotate (Real thrust);																					//	rotate the ship about its center of mass
				Real			Point (CVector vec);																					//	return the amount of thrust necessary to rotate the ship to a given vector
				Real			Go (CVector axis);																						//	compute the necessary orientation to move the rocket along a specified vector
};																																							//	end

//------------------------------------------------------------------------------
inline	void	Rocket::AdjustThrust (Real tThrust, Real rThrust)									//	adjust the max thrust values
{																																								//	begin
	maxTThrust *= tThrust;																												//	multiply the translational thrust
	maxRThrust *= rThrust;																												//	multiply the rotational thrust
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	ROCKET
