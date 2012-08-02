//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	03/06/97	JB		creation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "boss explosion sequence.h"
#include "circle.h"
#include "referee.h"
#include "view.h"
#include "sprite array mgr.h"
#include "sound mgr.h"
#include "moving sequence.h"
#include "time sequence.h"
#include "direction.h"
#include "random.h"

//------------------------------------------------------------------------------
char *explo[] = {"explo30", "explo1", "explo2", "explo3",
								 "plasma", "smoke01", "debr05", "debr04"};

//------------------------------------------------------------------------------
BossExplSeq::BossExplSeq (const PiecePtr &ship, Real time, int numPops) :
ExplSeq (ship->GetBody ()->TLocation (), ship->GetBody ()->TVelocity (),				//	constructor (continued)
				 time, ((Circle *) ship->GetGeometry ())->Radius (), numPops)						//	constructor (continued)
{
	gSoundMgr->PlaySound (6, ship);
}

//------------------------------------------------------------------------------
BossExplSeq::~BossExplSeq (void)
{
}

//------------------------------------------------------------------------------
void	BossExplSeq::Pop (void)
{
	int	idx = gRandom.RndInteger ();
	Vector_2d	offset = gDirection[idx][0];
	int	layer = (idx & 1) ? OVER_LAYER : UNDER_LAYER;
	Vector_2d partVel;
	if (idx & 4)
		partVel = offset * (gRandom.RndReal () * R(2.0e2));
	else
		partVel = Vector_2d (ZERO, ZERO);
	cstr exploName = explo[idx & 7];
	gView->Install (
		SeqPtr (
			new MoveSeq (
				new TimeSeq (gSpriteMgr.Fetch (exploName), R(.5)),
				location + (offset * radius * gRandom.RndReal ()), FALSE, 3,
				Vector_2d (ZERO, ZERO), gReferee->Interval (),
				partVel, R(0.45), //velocity, R(0.15),
				Vector_2d (ZERO, ZERO))),
		layer);
}

//------------------------------------------------------------------------------
