//------------------------------------------------------------------------------
//	Copyright ©1997 qualia, inc. All rights reserved.
//
//	02/11/97	JB		creation.
//	02/13/97	JB		first attempt at animations.
//	02/19/97	JB		mucking around with the naming suffixes.
//	02/24/97	JB		animation adjustment.
//	02/28/97	JB		moved the visual for the barracks into barrack ship, where it belongs.
//	03/05/97	DS		added SetDeathTrigger call for StateMgr
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
enum {SLEEPING, BIRTHING, GESTATING, IMPREGNATE};																//	this order must correspond with the order of the switch sequence parts (in the barrack ship)

//------------------------------------------------------------------------------
#include "barrack.h"
#include "eksekaren.h"
#include "thug.h"
#include "gnatty.h"
#include "player node.h"
#include "sprite array mgr.h"
#include "referee.h"
#include "view.h"
#include "get parameters.h"
#include "random.h"
#include "sound enum.h"

//------------------------------------------------------------------------------
class BugTypeNode : public ListNode
{
	private:
	protected:
	public:
				int bugType;
				BugTypeNode (int bType)	{bugType = bType;}
virtual	~BugTypeNode (void) {}
};

//------------------------------------------------------------------------------
Barrack::Barrack (Player *parent, int indx, int type, Real radius, Real rloc,		//	constructor
									Real armor, CPoint tloc, cstr name, int maxKids, int bugType) ://	constructor (continued)
EksBug (parent, BARRACK),																												//	constructor (continued)
soundDevice (00, tloc, snd_ekbldgEM)
{
	index = indx;																																	//	the index identification
	maxChildren = maxKids;																												//	the maximum kids it can produce
	defaultBugType = bugType;																											//	when it doubt, go with the default
	state = SLEEPING;																															//	the starting state... lazy barrack
	BarrackShip *bShip = new BarrackShip (this, radius, tloc, rloc, name);				//	create a piece for the barrack
	bShip->SetArmor (armor);																											//	set the piece's strength
	ship = PiecePtr (bShip);																											//	wrap the piece in a piece ptr
	gReferee->Install (ship, FULL_INTERSECT);																			//	install it into the referee
	bShip->SetDeathTrigger(indx,32,64);
	soundDevice.SwitchOn ();
}

//------------------------------------------------------------------------------
Barrack::~Barrack (void)																												//	destructor
{
}

//------------------------------------------------------------------------------
Bool	Barrack::PlayDead (void)																									//	the barrack has a special dying function
{
//	Switch (kSwitchOff);
	soundDevice.SwitchOff ();
	gEksekarenCount--;																														//	reduce the global eksekaren count (shouldn't this be in ~EksBug (void)?)
	gEksekaren->AcceptChildren (children);																				//	set the children free
	return FALSE;																																	//	we've died just fine
}

//------------------------------------------------------------------------------
void	Barrack::SetTarget (const PiecePtr &piece)																//	set the barrack's target, set its children's targets
{
	EksBug::SetTarget (piece);																										//	set target
	PlayerNode *node = (PlayerNode *) children.Head ();														//	starting at the top of the list
	while (node)																																	//	loop through the children
	{
		EksBug *eksBug = (EksBug *) node->player;																		//	extract the bug
		eksBug->SetTarget (piece);																									//	tell it to attack
		node = (PlayerNode *) children.Next (node);																	//	get the next
	}
}

//------------------------------------------------------------------------------
void	Barrack::CreateNewBug (void)																							//	create a new bug
{
	Vector_2d	off = Vector_2d (ship->GetBody ()->RLocation ()) * R(1.0e2);				//	create a new bug 100 meters away from the front of the barrack
	off += Vector_2d ((gRandom.RndReal () - R(0.5)) * R(1.0e2), ZERO);						//	continued
	Point_2d	tLoc = ship->GetBody ()->TLocation () + off;												//	continued
	Real			rLoc = ship->GetBody ()->RLocation ();															//	like mother, like daughter?
	EksBug		*eksBug = 0;																												//	init our pointer
	switch (currentBugType)
	{
		case LBUG:
		case HUNTER1:
		case HUNTER2:
		case HUNTER3:
		case DRONE1:
		case DRONE2:
		case DRONE3:
		case PRINCESS:
		{
			eksBug = new Thug (tLoc, rLoc, currentBugType);														//	create a new thug
		} break;

		case GNATTY:
		{
			eksBug = new Gnatty (type, ParmReal (GD_RADIUS), rLoc,										//	create a new gnatty
									 tLoc, gSpriteMgr.Fetch (ParmString (GD_SPRITE)));						//	create a new gnatty (continued)
		} break;
	}
	if (eksBug)																																		//	if we got a valid bug
		AttachBug (eksBug);																													//	make it a child of this barrack
}

//------------------------------------------------------------------------------
void	Barrack::DoState (void)																										//	the barrack's AI (*cough*)
{
	PlayChildren ();																															//	give the children some play time
	switch (state)																																//	go to the right behavior depending on what it's been doing
	{
		case SLEEPING:
		{
			if (numChildren < maxChildren)																						//	if it's not up to max production
				ChangeState (IMPREGNATE);																								//	start making a new kid
		}	break;

		case IMPREGNATE:
		{
			timeSpent = ZERO;																													//	we've just begun
			currentBugType = DetermineProduction ();																	//	figure out what kind of bug we're going to make
			gestationTime = DetermineGestationTime (currentBugType);									//	figure out how long it will take to make it
			ChangeState (GESTATING);																									//	switch to gestation mode!
		} break;

		case GESTATING:
		{
			if (timeSpent < gestationTime)																						//	if we haven't spent a long enough time
				timeSpent += gReferee->Interval ();																			//	add this frame's time to the total gestation time
			else																																			//	otherwise
				ChangeState (BIRTHING);																									//	switch to birthing mode!
		}	break;

		case BIRTHING:
		{
			Switch (BIRTHING);																												//	switch the graphic rep. of the barrack to the birthing animation
			CreateNewBug ();																													//	create the new bug
			ChangeState (SLEEPING);																										//	whew, that was tough, go back to sleep.
		}	break;
		
		default:
			ChangeState (SLEEPING);																										//	just in case...
	}
}

//------------------------------------------------------------------------------
void	Barrack::Attack (CPoint pt)
{
}

//------------------------------------------------------------------------------
void	Barrack::TrackingTarget (CPoint pt, CVector v)
{
}

//------------------------------------------------------------------------------
void	Barrack::PlayChildren (void)																							//	give the children some play time
{
	PlayerNode *eksNode = (PlayerNode *) children.Head (),												//	start at the top of the children list
						 *killNode;																													//	in case we have to remove any children
	while (eksNode)																																//	while we've got a valid eksnode
	{
		Bool ok = eksNode->player->Play ();																					//	play the bug
		if (!ok)																																		//	if the bug didn't play ok
			killNode = eksNode;																												//	grab a pointer to this bug
		eksNode = (PlayerNode *) children.Next (eksNode);														//	get the next bug
		if (!ok)																																		//	get ready to kill the bad bug
		{
			children.Remove (killNode);																								//	remove the stinky dead bug
			delete killNode;																													//	and delete it!
		}
	}
}

//------------------------------------------------------------------------------
int		Barrack::DetermineProduction (void)																				//	determine the type of production
{
	BugTypeNode *bNode = (BugTypeNode *) deceased.Head ();												//	look at the list of deceased bugs
	if (bNode)																																		//	if we found a dead one
	{
		int bugType = bNode->bugType;																								//	check its bugtype
		deceased.Remove (bNode);																										//	remove it from the list
		delete bNode;																																//	nuke the node
		return bugType;																															//	return its type
	}
	return defaultBugType;																												//	return the default bug type if there weren't any dead ones queued up
}

//------------------------------------------------------------------------------
Real	Barrack::DetermineGestationTime (int bugType)															//	determine the gestation period required for a particular bug type
{
	return R(5.0);																																//	right now the bugs all take the same amount of time
}

//------------------------------------------------------------------------------
void	Barrack::AttachBug (EksBug *eksBug)																				//	attack a bug to a parent for control purposes
{
	eksBug->SetParent (this);																											//	tell the bug about its new parent
	PlayerNode *eksNode = new PlayerNode (eksBug);																//	give it a nice node to hang out in
	children.InsertAfter (eksNode);																								//	add it to the children list
	numChildren++;																																//	inc the barrack's number of children
}

//------------------------------------------------------------------------------
void	Barrack::RegisterDeath (int typ)																					//	so sad.  a poor bug died.
{
	numChildren--;																																//	dec the barrack's number of children
	BugTypeNode *bNode = new BugTypeNode (typ);																		//	and add it to the deceased list
	deceased.InsertAfter (bNode);																									//	and add it to the deceased list (continued)
}

//------------------------------------------------------------------------------
