//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	02/03/97
//
//	Revisions:			02/14/97:	resource health exchange enum (JB)
//
//------------------------------------------------------------------------------

#ifndef	RESOURCE
#define	RESOURCE

//------------------------------------------------------------------------------
#include "piece ptr.h"
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
enum	{
				RESOURCE_HEALTH = 0,
				RESOURCE_TRAIK_AMMO,

				RESOURCE_FLECHETTE_GUN,
				RESOURCE_FLECHETTE_AMMO,

				RESOURCE_LASER_GUN,
				RESOURCE_LASER_AMMO,

				RESOURCE_PLASMA_GUN,
				RESOURCE_PLASMA_AMMO,

				RESOURCE_BUSTER_GUN,
				RESOURCE_BUSTER_AMMO,

				RESOURCE_RUPTOR_GUN,
				RESOURCE_RUPTOR_AMMO,

				RESOURCE_ROCKET_GUN,
				RESOURCE_ROCKET_AMMO,

				RESOURCE_TACMINE_GUN,
				RESOURCE_TACMINE_AMMO,

				RESOURCE_MINE_GUN,
				RESOURCE_MINE_AMMO,

				RESOURCE_GENERIC,

				RESOURCE_HEALTH_EXCHANGE,
				RESOURCE_AMMO_EXCHANGE,

				RESOURCE_DEDUCTION
			};

//------------------------------------------------------------------------------
class Resource : public Piece																										//	a resource
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				int				type;
				int				number;
	public:																																				//	members available externally
				Resource (Real radius, CPoint location, Real orientation, SArrayPtr spr,//	constructor
									int type, int number, Real time = INFINITY);									//	constructor (continued)
virtual	~Resource (void);																												//	destructor
virtual	Bool			Step (Real time);																							//	step the piece ahead in time
virtual	int				PostCollide (const PiecePtr &piece, Sect *sect);							//	do some work after a collision with another piece
virtual	Real			TraceRay (CPoint origin, CVector direction);									//	return the time at which a ray intersects this piece
				void			PrintName (int type);																					//	display the resource name on the screen
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	RESOURCE
