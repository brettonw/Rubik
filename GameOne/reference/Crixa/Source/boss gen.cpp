//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//
//	01/23/97	JB		creation/last revision.
//	03/04/97	JB		boss gen flash.
//	03/06/97	JB		made boss gen a ship.  yay!
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "boss gen.h"
#include "circle.h"
#include "time sequence.h"
#include "stationary sequence.h"
#include "sequence visual.h"
#include "quadratic.h"
#include "get parameters.h"

#include "random.h"
#include "view.h"
#include "alt pumping sequence.h"
#include "boss explosion sequence.h"
#include "pi.h"

//------------------------------------------------------------------------------
const	char* debris[] = {"debr01", "debr02", "debr03", "debr04"};

//------------------------------------------------------------------------------
GenDev::GenDev (int id) : Device (id)
{
	output = 1;
}

//------------------------------------------------------------------------------
GenDev::~GenDev (void)
{
}

//------------------------------------------------------------------------------
int	GenDev::Trigger (const PiecePtr &piece, int type, int what)
{
	if (type == DEV_TOGGLE)																												//	if the generator is to switch states
	{
		type = DEV_POWER;
		what = power ? -output : output;
	}
	return Device::Trigger (piece, type, what);
}

//------------------------------------------------------------------------------
BossGen::BossGen (Player *own, GenDev *dev, Real arm, Point_2d tLoc,
									Real radius, SArrayPtr spr) :
Ship (own)
{
	bGenDev = dev;
	body = new Body (tLoc, ZERO, INFINITY, INFINITY);
	TimeSeq *timeSeq = new TimeSeq (spr, TRUE, R(1.0));
	visual = new SeqVisual (body, SeqPtr (timeSeq));
	geometry = new Circle (radius);
	SetArmor (arm);

	rDamping = R(-2.0e-1);																												//	set a starting value for rotational damping
	stun = ZERO;																																	//	set the current stun time to 0
	stunMax = ZERO;																																//	set the stun max to a hundredth of a second
	dead = FALSE;																																	//	the ship is not dead
	numNozzles = 0;																																//	set the number of nozzle points to 0
	thrSprite = gSpriteMgr.Fetch ("eksthr");
	resources = ParmInteger (RESOURCE_SIZE);

	dyingTime = R(1.0);
}

//------------------------------------------------------------------------------
BossGen::~BossGen (void)
{
	delete bGenDev;
}

//------------------------------------------------------------------------------
int	BossGen::Hurt (const PiecePtr &piece, Real damage)
{
	GetVisual ()->SetMode (VISFLASH);
	int check = int (life * R(10.0));
	Piece::Damage (piece, damage);
	check -= int (life * R(10.0));
	if (life == ZERO)
		bGenDev->Toggle ();
	return check;
}

//------------------------------------------------------------------------------
void	BossGen::AddEffects (int check)
{
	for (int i = 0; i < check; i++)
	{
		Real			radius = ((Circle *) geometry)->Radius ();													//	get the radius of the ship
		Vector_2d offset (gRandom.RndReal () * TWO_PI * R(0.99));
		offset *= (radius * gRandom.RndReal ());
		SArrayPtr	sprite = gSpriteMgr.Fetch ((gRandom.RndInteger () & 0x01) ? "plasma" : "plasma2");
		AltPumpSeq	*pump = new AltPumpSeq (sprite, this, offset);
		gView->Install (pump, 0);
	}
}

//------------------------------------------------------------------------------
void	BossGen::Explode (const PiecePtr &piece)
{
	gView->Install (new BossExplSeq (this, R(2.0), 3));														//	build the explosion
}

//------------------------------------------------------------------------------
Bool	BossGen::Step (Real time)
{
	Bool	alive = Ship::Step (time);
	if (!alive)
	{
		dyingTime -= time;
		if (dyingTime < ZERO)
			return FALSE;
	}
	return TRUE;	
}

//------------------------------------------------------------------------------
