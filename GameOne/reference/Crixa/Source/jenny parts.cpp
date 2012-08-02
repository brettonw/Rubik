//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	01/23/97	JB		half-pass destruction sequences.
//	02/19/97	JB		added new turret types for mark.
//	03/04/97	JB		damage flashing.
//	03/07/97	JB		changed JForeHaunchPart to JHaunchPart
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#include "jenny parts.h"
#include "circle.h"
#include "pi.h"
#include "sound mgr.h"
#include "random.h"
#include "direction.h"
#include "sprite array mgr.h"
#include "time sequence.h"
#include "stationary sequence.h"
#include "moving sequence.h"
#include "referee.h"
#include "view.h"
#include "boss explosion sequence.h"

//------------------------------------------------------------------------------
//	default jennypart constructor
//------------------------------------------------------------------------------
JennyPart::JennyPart (Player *own, Real radius, CPoint loc, Real rLoc,					//	constructor
SArrayPtr spr) : Turret (own, radius, loc, rLoc, spr)														//	constructor (continued)
{																																								//
}																																								//

//------------------------------------------------------------------------------
//	jennypart destructor
//------------------------------------------------------------------------------
JennyPart::~JennyPart (void)																										//	destructor
{																																								//
}																																								//

//------------------------------------------------------------------------------
//	final jennypart explosion sequence
//------------------------------------------------------------------------------
void	JennyPart::Explode (void)																									//	explode the jenny part
{																																								//
	lastPop = deadTime;																														//
	static		cstr explo[2] = {"explo2", "explo3"};																//	the explosions we will be using
	Real			angle = gRandom.RndReal () * TWO_PI;																//	randomly choose an angle
	Vector_2d	dirVec (angle);																											//	create a direction vector
	dirVec *= R(3.0e2) * gRandom.RndReal ();																			//	choose a distance for the explosion to travel
	TimeSeq	*timeSeq = new TimeSeq (gSpriteMgr.Fetch (explo[gRandom.RndInteger () & 1]));//	make a sequence of the animation
	MoveSeq	*moveSeq = new MoveSeq (SeqPtr (timeSeq), body->TLocation (), FALSE,	//	make a sequence of the animation (continued)
																	1, dirVec, gReferee->Interval (), dirVec);		//	make a sequence of the animation (continued)
	gView->Install (SeqPtr (moveSeq), OVER_LAYER);																//	install the sequence
	gView->Install (new BossExplSeq (this, R(1.0), 2));														//	build the explosion
}																																								//

//------------------------------------------------------------------------------
//	damage the jennypart
//------------------------------------------------------------------------------
void	JennyPart::Damage (const PiecePtr &piece, Real damage)										//	damage the jenny part
{																																								//
	Piece::Damage (piece, damage);																								//	use the piece damage function
	GetVisual()->SetMode(VISFLASH);																								//	ds: flash!
	if (life == ZERO && deadTime == INFINITY)																			//	if the piece has just died
	{																																							//
		Real			radius = ((Circle *) geometry)->Radius ();												//
		lastPop = deadTime = R(0.01) * radius * (R(0.5) + gRandom.RndReal ());			//	calculate the duration of the explosion
	}
}

//------------------------------------------------------------------------------
Bool	JennyPart::Step (Real time)																								//	step the part
{																																								//
	Bool	alive = Turret::Step (time);																						//	step the turret and see if it's still alive
	if (!alive)																																		//	if the turret has died
		gSoundMgr->PlaySound (38);																									//	play a jenny shriek
	return alive;																																	//	return the state of life or death of the turret
}																																								//

//------------------------------------------------------------------------------
void	JennyPart::Kill (void)																										//	outright kill the piece
{																																								//
	life = ZERO;																																	//	bring the life down to zero (dead)
	Damage (PiecePtr (), ZERO);																										//	call damage so the deadtime gets set
}																																								//

//------------------------------------------------------------------------------
JFacePart::JFacePart (Player *own, Real radius, CPoint loc, Real rLoc, SArrayPtr spr) :
JennyPart (own, radius, loc, rLoc, spr)
{
}

//------------------------------------------------------------------------------
JFacePart::~JFacePart (void)
{
}

//------------------------------------------------------------------------------
JJawPart::JJawPart (Player *own, Real radius, CPoint loc, Real rLoc, SArrayPtr spr) :
JennyPart (own, radius, loc, rLoc, spr)
{
}

//------------------------------------------------------------------------------
JJawPart::~JJawPart (void)
{
}

//------------------------------------------------------------------------------
JMandiblePart::JMandiblePart (Player *own, Real radius, CPoint loc, Real rLoc, SArrayPtr spr) :
JennyPart (own, radius, loc, rLoc, spr)
{
}

//------------------------------------------------------------------------------
JMandiblePart::~JMandiblePart (void)
{
}

//------------------------------------------------------------------------------
JLegPart::JLegPart (Player *own, Real radius, CPoint loc, Real rLoc, SArrayPtr spr) :
JennyPart (own, radius, loc, rLoc, spr)
{
}

//------------------------------------------------------------------------------
JLegPart::~JLegPart (void)
{
}

//------------------------------------------------------------------------------
JCiliaPart::JCiliaPart (Player *own, Real radius, CPoint loc, Real rLoc, SArrayPtr spr) :
JennyPart (own, radius, loc, rLoc, spr)
{
}

//------------------------------------------------------------------------------
JCiliaPart::~JCiliaPart (void)
{
}

//------------------------------------------------------------------------------
JHaunchPart::JHaunchPart (Player *own, Real radius, CPoint loc, Real rLoc, SArrayPtr spr) :
JennyPart (own, radius, loc, rLoc, spr)
{
}

//------------------------------------------------------------------------------
JHaunchPart::~JHaunchPart (void)
{
}

//------------------------------------------------------------------------------
JMarkPart::JMarkPart (Player *own, Real radius, CPoint tLoc, Real rLoc, SArrayPtr spr) :
JennyPart (own, radius, tLoc, rLoc, spr)
{
}

//------------------------------------------------------------------------------
JMarkPart::~JMarkPart (void)
{
}

//------------------------------------------------------------------------------
