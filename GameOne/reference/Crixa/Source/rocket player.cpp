//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/20/97
//	03/19/97	DS		Played with some alternative tracking algorithms
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "rocket player.h"
#include "rocket.h"
#include "referee.h"
#include "world.h"

//------------------------------------------------------------------------------
RockPlay::RockPlay (const PiecePtr &rock, const LockPtr &lock, Real del) : Player ("Rocket")				//	constructor
{																																								//	begin
	rocket = rock;
	target = lock;
	delay = del;
}																																								//	end

//------------------------------------------------------------------------------
RockPlay::~RockPlay (void)																											//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Bool	RockPlay::Play (void)																											//	play one frame
{																																								//	begin

	Real range  = R(1.0), 
			 rotate = R(0.0),
			 thrust = R(0.0);

	if (gReferee->Confirm (rocket))
	{
		if (delay <= ZERO)
		{
			Body	*body = rocket->GetBody ();
			body->ApplyTorque (R(-0.5) * body->RVelocity () * body->RInertia ());
			if (target ())
			{
				Point_2d	locAt = rocket->GetBody ()->TLocation ();
				Vector_2d	dir = target->Location (R(0.1)) - locAt;
				range = gWorld->TraceRay (locAt, dir);
				if (range >= R(1.0))									//	return the time at which a ray intersects this piece
				{
					rotate = ((Rocket *) rocket ())->Go (dir);
					((Rocket *) rocket ())->Rotate (rotate);
				}
			}
		}
		else
		{
			delay -= gReferee->Interval ();
			range = R(2.0);
		}


		thrust = range * R(0.5);
		if (thrust > R(20.0)) thrust = R(20.0);
		thrust *= 1.0-fabs(rotate);

		((Rocket *) rocket ())->Thrust (thrust);
		gReferee->Update (rocket);
		return TRUE;
	}
	return FALSE;
}																																								//	end

//------------------------------------------------------------------------------
