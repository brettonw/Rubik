//------------------------------------------------------------------------------
//	copyright ©1997 qualia, inc. All rights reserved.
//
//	03/18/97	DS		Created new RegenPieceNode class
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "regen piece node.h"
#include "debugger.h"

#include "shipyard.h"
#include "sprite array mgr.h" 
#include "referee.h"
#include "get parameters.h"
#include "sound mgr.h"
#include "view.h"

#include "sequence ptr.h"
#include "stationary sequence.h" 
#include "time sequence.h"
#include "piece ptr.h"

#include "dest wall.h"
#include "resource.h"

//------------------------------------------------------------------------------
#define TELEPORT 55

//------------------------------------------------------------------------------
// CONSTRUCTOR for DWALLS
RegenPieceNode::RegenPieceNode (PiecePtr pc, int typ, Point_2d loc, cstr fn, Real ar, Real w, Real h, int lay)
{
	piece = pc;																																		// piece to monitor
	type = typ;																																		// type of piece
	location = loc;																																// location of piece
	armor = ar;																																		// armor value of piece
	width = w;																																		// width of piece
	height = h;																																		// height of piece
	layer = lay;																																	// layer to install on
	fname = fn;																																		// filename of sprite
	SetRegenTime (ParmReal(DWALL_02_RESPAWN_TIME));																// set default regen time to 30 seconds

	dtype = D_DESTWALL;
	regenerating = FALSE;
}

//------------------------------------------------------------------------------
// CONSTRUCTOR for RESOURCES
RegenPieceNode::RegenPieceNode (PiecePtr pc, int typ, Point_2d loc, int am)
{
	piece = pc;																																		// piece to monitor
	type = typ;																																		// type of piece
	location = loc;																																// location of piece
	amount = am;																																	// amount of resource
	SetRegenTime (ParmReal(RESOURCE_RESPAWN_TIME));																// set default regen time to 30 seconds
	
	dtype = D_RESOURCE;
	regenerating = FALSE;
}

//------------------------------------------------------------------------------
RegenPieceNode::~RegenPieceNode (void)
{
	piece = PiecePtr (0);
}

//------------------------------------------------------------------------------
void RegenPieceNode::CheckPiece (void)
{
	PiecePtr dummypiece;
	if (piece->GetLife()!=ZERO) return;																						// the piece is still alive!
	if (!regenerating) {
		regenwhen = (gReferee->MasterClock()) + regentime;
		regenerating = TRUE;
	}
	if ((gReferee->MasterClock() > regenwhen) && (!gReferee->Contains(location,dummypiece)))
	{
		switch (dtype)
		{
			case D_DESTWALL:	RegenDWall();
												break;
			case D_RESOURCE:	RegenDResource();
												break;
		}
		gView->Install (SeqPtr (new StatSeq (location, new TimeSeq (gSpriteMgr.Fetch ("etel")))));
		gSoundMgr->PlaySound(TELEPORT,piece,FALSE);
		regenerating = FALSE;
	}
}

//------------------------------------------------------------------------------
void RegenPieceNode::RegenDWall (void)
{
	// we only handle destructable resource type 2
	SeqPtr seq (new SpriteSeq (gSpriteMgr.Fetch (fname)));
	DestWall *dwall = new DestWall (gShipyard, armor, location, width, height, seq, layer);
	gReferee->Install (dwall, FULL_INTERSECT);
	piece = PiecePtr(dwall);
}

//------------------------------------------------------------------------------
void RegenPieceNode::RegenDResource (void)
{
 	Resource *resource = new Resource  (R(4.0), location, R(0.0), gSpriteMgr.Fetch ("res"), type, amount);
	gReferee->Install (resource, MOBILE_INTERSECT);
	piece = PiecePtr(resource);
}

//------------------------------------------------------------------------------
