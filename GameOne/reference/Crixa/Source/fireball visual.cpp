//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/16/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "fireball visual.h"
#include "view.h"
#include "moving sequence.h"
#include "time sequence.h"
#include "referee.h"
#include "random.h"

//------------------------------------------------------------------------------
FireBall::FireBall (Body *bod, const SArrayPtr &puf) : SeqVisual (bod, SeqPtr (0))	//	constructor
{																																								//	begin
	puff = puf;																																		//	copy the puff sprite
}																																								//	end

//------------------------------------------------------------------------------
FireBall::~FireBall (void)																											//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	FireBall::Draw (CPoint where)																							//	draw the object
{																																								//	begin
	Vector_2d	norm = body->TVelocity ();
	Vector_2d	perp (norm[Y], -norm[X]),
						nvec = norm * gRandom.RndReal (),
						pvec = perp * (R(5.0e-2) * (gRandom.RndReal () + R(-0.5))),
						rvec = nvec + pvec;
	SeqPtr		seq = new MoveSeq (	new TimeSeq (puff), body->TLocation (), FALSE, 3, 
																Vector_2d (ZERO, ZERO), gReferee->Interval (), 
																rvec, R(0.25), 
																Vector_2d (ZERO, ZERO));
	gView->Install (seq, UNDER_LAYER);
}																																								//	end

//------------------------------------------------------------------------------
