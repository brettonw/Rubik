//------------------------------------------------------------------------------
//	Copyright ©1997 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Revisions:			01/29/97: creation. (JB)
//									02/05/97:	breakout of state functions. (JB)
//									02/19/97:	broke out mini piece node, mad gd mini piece node (JB)
//  02/21/97				MK				Added Approch, charging & detonation noises to Gnatty
//														Added "attacking" flag to debounce charging sound
//	03/07/97	JB		prevented gnattys from trying to fly through other pieces.
//
//------------------------------------------------------------------------------

/*//----------------------------------------------------------------------------

DESCRIPTION:
the gnatty dread is an eks that attaches itself with a springline to its target,
then blows up after a time, attempting to injure the target in the process.

gnatty phase I, complete.  awaiting tuning & feedback.

*///----------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "gnatty.h"
#include "gnatty ship.h"
#include "spring gun.h"
#include "referee.h"
#include "sprite array mgr.h"
#include "floor trigger.h"
#include "world.h"
#include "target lock.h"
#include "random.h"
#include "circle.h"
#include "view.h"
#include "stationary sequence.h"
#include "time sequence.h"
#include "direction.h"
#include "get parameters.h"
#include "mini piece node.h"
#include "Sound Mgr.h"
#include "target lock.h"

#define  GNATAPRO  66
#define  GNATCHARGE 65
#define  GNATEXPLO 10
#define  GNATANGER 66

//------------------------------------------------------------------------------
class	GDMiniPieceNode : public MiniPieceNode
{
	private:
	protected:
				int					numHits;
	public:
				GDMiniPieceNode (const PiecePtr &piec);
				void				AddHit (int numhits = 1);
				int					NumHits (void);
};

//------------------------------------------------------------------------------
GDMiniPieceNode::GDMiniPieceNode (const PiecePtr	&piec) :
MiniPieceNode (piec)
{
	numHits = 0;
}

//------------------------------------------------------------------------------
inline void	GDMiniPieceNode::AddHit (int numhits)
{
	numHits += numhits;
}

//------------------------------------------------------------------------------
inline int	GDMiniPieceNode::NumHits (void)
{
	return numHits;
}

//------------------------------------------------------------------------------
enum {DRIFT = 0, TRACKING_TARGET};																							//	gnatty states

//------------------------------------------------------------------------------
Gnatty::Gnatty (int type, Real rad, Real rLoc, CPoint tLoc, SArrayPtr sptr) :		//	constructor
EksBug (0, GNATTY)																															//	constructor (continued)
{
	damage = ParmReal (GD_PART_DMG);																							//	get the damage of the gnatty explosion particles
	state = DRIFT;																																//	start the gnatty out just drifting
	GnattyShip *gship = new GnattyShip (this, rad, tLoc, rLoc, sptr, 1);					//	create a new weapon ship for the gnatty
	gship->AdjustThrust (ParmReal (GD_T_THRUST), ParmReal (GD_R_THRUST));					//	set the gnatty's thrust capabilities
	gship->SetArmor (ParmReal (GD_ARMOR));																				//	set the gnatty's armor
	gship->SetHardness (ParmReal (GD_HARDNESS));																	//	set the gnatty's hardness
	ship = PiecePtr (gship);																											//	encapsulate the weaponship in a piece ptr
	SpringGun *sg = new SpringGun (ZERO, ParmReal (GD_WEAPON_RANGE), ship,				//	create a spring gun
									ParmReal (SPRING_HOLD_TIME), ZERO,														//	create a spring gun (continued)
									gSpriteMgr.Fetch(ParmString (SPRING_SPLUT)), -1);							//	create a spring gun (continued)
	sg->AddAmmoCount (ParmInteger (GD_AMMO_COUNT));																//	give the spring gun a certain amount of ammo
	gship->SetWeapon (0, sg);																											//	attach the spring gun to the gnatty
	gReferee->Install (ship);																											//	install the gnatty's ship into the referee
	ekthrustsound = GNATAPRO;																											//	(MK) assign approach sound to gnatty
	SetSounds(GNATAPRO, GNATANGER);																								//  install the sounds
	attacking = FALSE;																														//	Debounce flag for attack sound
}

//------------------------------------------------------------------------------
Gnatty::~Gnatty (void)																													//	destructor
{
}

//------------------------------------------------------------------------------
void	Gnatty::DoState (void)
{
	CPoint	loc = ship->GetBody ()->TLocation ();
	if (lock () && lock->Confirm ())
	{
		Vector_2d toTarget = lock->Location () - loc;
		Bool spotted = FALSE;
		if (gWorld->TraceRay (loc, toTarget) > R(1.0))
		{
			PiecePtr hitPiece;
			gReferee->TraceRay (loc, toTarget, hitPiece);
			if (hitPiece () == (((TLock *) lock ())->Target ()) ())
				spotted = TRUE;
		}
		if (spotted)
			switch (state)
			{
				case TRACKING_TARGET:
					TrackingTarget (loc, toTarget);
					break;

				default:
					ChangeState (TRACKING_TARGET);
			}
		else
			switch (state)
			{
				case TRACKING_TARGET:
					attacking = FALSE;
					ChangeState (DRIFT);
					break;

				default:
					attacking = FALSE;
					Drifting ();
			}
	}
}

//------------------------------------------------------------------------------
void	Gnatty::TrackingTarget (CPoint loc, CVector toTarget)
{
	Weapon	*weapon = ((WeaponShip *) ship ())->GetCurrentWeapon ();
	Real		range = weapon->Range ();
	if ((toTarget | toTarget) > (range * range))
		GoTo (lock->Location (R(0.1)), lock->Radius ());
	else
		Attack (lock->Location ());
}

//------------------------------------------------------------------------------
void	Gnatty::Attack (CPoint pt)
{
	Real	rotate = ((WeaponShip *) ship ())->AimAt (pt);
	((Ship *) ship ())->Rotate (rotate);
	if (FABS (rotate) < R(1.0))
	{
		WeaponShip *wship = (WeaponShip *) ship ();
		wship->Fire ();
		
		if (!attacking){
			attacking = TRUE;
			gSoundMgr->PlaySound(GNATCHARGE, ship);																		//  Charging Sound for Gnatty
		}
		if (wship->GetCurrentWeapon ()->AmmoCount () == 0)
		{
			SelfDestruct ();
			ship->SetArmor (R(1.0));
			ship->Damage (PiecePtr (), R(1.0));
		}
	}
	gReferee->Update (ship);
}

//------------------------------------------------------------------------------
void	Gnatty::SelfDestruct (void)																								//	have the gnatty self destruct
{
	Circle	*circle = (Circle *) ship->GetGeometry ();														//	get the ship's circular geometry
	Body		*body = ship->GetBody ();																							//	get the ship's body
	Real		radius = circle->Radius (),																						//	get the ship's radius
					range = ParmReal (GD_EXPLO_RANGE);

	List pieces;																																	//	create a list of pieces
	const int kRayInterval = DIRECTION_COUNT >> 3;																//	get the ray interval (8 extending rays)
	for (int dir = 0; dir < DIRECTION_COUNT; dir += kRayInterval)									//	loop through the rays
	{
		PiecePtr piece;																															//	the piece ptr that will hold the intersected piece
		Real dist = gReferee->TraceRay (body->TLocation (), gDirection[dir][0], piece);//	shoot out a ray for the explosion
		if (dist < range)																														//	if the intersected distance is less than the range of the explosion
		{
			Bool	foundPiece = FALSE;																									//	have we found this piece already?
			GDMiniPieceNode *node = (GDMiniPieceNode *) pieces.Head ();								//	start at the beginning of the already found pieces
			while (node)																															//	while there still are pieces
			{
				if (node->piece () == piece ())																					//	if the currently found piece has already been found
				{
					foundPiece = TRUE;																										//	we already found the piece
					node->AddHit ();																											//	add a hit to it
					node = 0;																															//	break out of the loop
				}
				else																																		//	we haven't found the piece yet
					node = (GDMiniPieceNode *) pieces.Next (node);												//	so get the next piece on the list
			}
			if (!foundPiece)																													//	if we didn't find the piece on the list
			{
				GDMiniPieceNode *gdMPnode = new GDMiniPieceNode (piece);
				gdMPnode->AddHit ();
				pieces.InsertAfter (gdMPnode);																					//	add the piece to the list
			}
		}
	}


	gSoundMgr->PlaySound(GNATEXPLO, ship);																				//  (MK)play the explosion sound
//	damage the enemies
	Real	damage = ParmReal (GD_EXPLO_DMG),																				//	get the damage amount
				adjDmg = damage / R(2.0);																								//	the random damage will fluctuate 1/4 each way
	damage -= damage / R(4.0);																										//	the random damage will fluctuate 1/4 each way
	GDMiniPieceNode *node = (GDMiniPieceNode *) pieces.Head ();										//	loop through the hit pieces
	while (node)																																	//	while there are still pieces
	{
		Real	modDmg = (damage + adjDmg * gRandom.RndReal ()) * node->NumHits ();		//	calculate the modified damage
		node->piece->Damage (ship, modDmg);																					//	damage the hit piece
		node = (GDMiniPieceNode *) pieces.Next (node);															//	get the next
	}
	gView->Install (SeqPtr (new StatSeq (body->TLocation (),
													new TimeSeq (gSpriteMgr.Fetch ("etel")))));
}

//------------------------------------------------------------------------------
