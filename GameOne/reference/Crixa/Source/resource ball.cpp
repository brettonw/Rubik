//------------------------------------------------------------------------------
//	Copyright ©1997 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//
//	01/31/97	JB		creation. 
//	02/06/97:	JB		comments & description. 
//	02/24/97	DS		Added Flashing Indicator in Damage()
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "resource ball.h"
#include "resource.h"
#include "sprite array mgr.h"
#include "referee.h"
#include "random.h"
#include "circle.h"
#include "moving sequence.h"
#include "time sequence.h"
#include "view.h"

//------------------------------------------------------------------------------
ResBall::ResBall (Player *own, Real armor, CPoint loc, Real rad, SArrayPtr spr,	//	constructor
									int type, int num) :																					//	constructor (continued)
DestBall (own, armor, loc, rad, spr)																						//	constructor (continued)
{																																								//	begin
	resType = type;																																//	the type of the resouce
	resAmount = num;																															//	the amount of the resource
}																																								//	end

//------------------------------------------------------------------------------
ResBall::~ResBall (void)																												//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	ResBall::Damage (const PiecePtr &piece, Real damage)											//	damage the resource ball
{																																								//	begin
	Piece::Damage (piece, damage);																								//	use the default piece damage function
	GetVisual()->SetMode(VISFLASH);																								//	flash!
	if (life == ZERO)																															//	if the ball suffered a death-dealing blow
	{																																							//	begin
		Explode ();																																	//	explode the ball
		Point_2d loc = body->TLocation ();																					//	place the resource where the ball was
		Resource *resource = new Resource (R(4.0), loc, ZERO,												//	create a new resource
						 gSpriteMgr.Fetch ("res"), resType, resAmount);											//	create a new resource (continued)
		resource->GetBody ()->AddVelocity (body->TVelocity (), ZERO);								//	impart the resource with the ball's velocity (before it died)
		gReferee->Install (PiecePtr (resource), NORMAL_INTERSECT);									//	insert the resource into the world
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
void	ResBall::Explode (void)																										//	explode the resource ball
{																																								//	begin
	Vector_2d		velocity = body->TVelocity ();																		//	grab the velocity of the ball
	Point_2d		location = body->TLocation ();																		//	grab the location of the ball
	Real				radius = ((Circle *) geometry)->Radius ();												//	grab the radius of the ball
	static cstr	explo[2] = {"explo1", "explo3"};																	//	some explosions tha we will use
	//	TEMPORARY/LAME EXPLOSIONS.  REPLACE WITH SOMETHING DECENT.  TRANSMISSION OVER.
	for (int i = 0; i < 4; i++)
	{
		SArrayPtr spr = gSpriteMgr.Fetch (explo[gRandom.RndInteger () & 1]);
		gView->Install (SeqPtr (new MoveSeq (new TimeSeq (spr),
			location + (radius * R(0.5)), FALSE, 3, Vector_2d (ZERO, ZERO),
			gReferee->Interval (), velocity, R(0.15), Vector_2d (ZERO, ZERO))));
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
