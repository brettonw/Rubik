//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/30/96
//
//	01/24/97	JB		added BossRoomTrap door 14, BossRoomExit doors 15-17
//	02/03/97	JB		Furnace doors & pistons 18, 19, 20, 21, 22, 23
//	02/06/97	JB		modified 22, 23, added  24-26
//	02/19/97	MK		Added PiecePtr info to sound calls
//	03/04/97	JB		added 49-51, modified 27-34.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "door player.h"
#include "door.h"
#include "referee.h"
#include "sprite array mgr.h"
#include "math utility.h"
#include "sound mgr.h"
#include "sound enum.h"

//------------------------------------------------------------------------------
DoorPlayer::DoorPlayer (CPoint tLoc, int type, int id) :												//	constructor
Player ("Shipyard"),																														//	constructor cont'd
Device (id)																																			//	constructor cont'd
{																																								//	begin
	switch (type)
	{
		case 4:
			doorStart = 25;
			doorStop = 26;
			count = 2;
			part = new DoorPart[2];
			part[0].lock = tLoc + Vector_2d (R(40.0), ZERO);
			part[0].axis (R(1.0), ZERO);
			part[0].maxPosition = R(50.0);
			part[0].maxVelocity = R(5.0e1);
			part[0].maxForce = R(1.0e8);
			part[0].piece = new Door (this, part[0].lock, R(80.0), R(32.0), gSpriteMgr.Fetch ("door04e"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			part[1].lock = tLoc - Vector_2d (R(40.0), ZERO);
			part[1].axis (R(-1.0), ZERO);
			part[1].maxPosition = R(50.0);
			part[1].maxVelocity = R(5.0e1);
			part[1].maxForce = R(1.0e8);
			part[1].piece = new Door (this, part[1].lock, R(80.0), R(32.0), gSpriteMgr.Fetch ("door04w"), part[1].axis);
			gReferee->Install (part[1].piece, FULL_INTERSECT);
			break;
		case 5:
			doorStart = 27;
			doorStop = 28;
			count = 1;
			part = new DoorPart[1];
			part[0].lock = tLoc;
			part[0].axis (R(1.0), ZERO);
			part[0].maxPosition = R(48.0);
			part[0].maxVelocity = R(1.0e2);
			part[0].maxForce = R(1.0e8);
			part[0].piece = new Door (this, tLoc, R(48.0), R(16.0), gSpriteMgr.Fetch ("door05"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;
		case 6:
			doorStart = 27;
			doorStop = 28;
			count = 1;
			part = new DoorPart[1];
			part[0].lock = tLoc;
			part[0].axis (ZERO, R(1.0));
			part[0].maxPosition = R(48.0);
			part[0].maxVelocity = R(1.0e2);
			part[0].maxForce = R(1.0e8);
			part[0].piece = new Door (this, tLoc, R(16.0), R(48.0), gSpriteMgr.Fetch ("door06"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;
		case 8:
			doorStart = 29;
			doorStop = 30;
			count = 1;
			part = new DoorPart[1];
			part[0].lock = tLoc;
			part[0].axis (R(1.0), ZERO);
			part[0].maxPosition = R(336.0);
			part[0].maxVelocity = R(6.0e1);
			part[0].maxForce = R(2.0e8);
			part[0].piece = new Door (this, tLoc, R(348.0), R(44.0), gSpriteMgr.Fetch ("door08"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;
		case 9:
			doorStart = 31;
			doorStop = 32;
			count = 2;
			part = new DoorPart[2];
			part[0].lock = tLoc + Vector_2d (R(104.0), ZERO);
			part[0].axis (R(1.0), ZERO);
			part[0].maxPosition = R(192.0);
			part[0].maxVelocity = R(2.5e1);
			part[0].maxForce = R(7.0e7);
			part[0].piece = new Door (this, part[0].lock, R(204.0), R(72.0), gSpriteMgr.Fetch ("door09e"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			part[1].lock = tLoc - Vector_2d (R(104.0), ZERO);
			part[1].axis (R(-1.0), ZERO);
			part[1].maxPosition = R(192.0);
			part[1].maxVelocity = R(2.5e1);
			part[1].maxForce = R(7.0e7);
			part[1].piece = new Door (this, part[1].lock, R(204.0), R(72.0), gSpriteMgr.Fetch ("door09w"), part[1].axis);
			gReferee->Install (part[1].piece, FULL_INTERSECT);
			break;
		case 10:
			doorStart = 33;
			doorStop = 34;
			count = 2;
			part = new DoorPart[2];
			part[0].lock = tLoc + Vector_2d (ZERO, R(72.0));
			part[0].axis (ZERO, R(1.0));
			part[0].maxPosition = R(144.0);
			part[0].maxVelocity = R(5.0e1);
			part[0].maxForce = R(1.0e8);
			part[0].piece = new Door (this, part[0].lock, R(48.0), R(144.0), gSpriteMgr.Fetch ("door10n"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			part[1].lock = tLoc - Vector_2d (ZERO, R(72.0));
			part[1].axis (ZERO, R(-1.0));
			part[1].maxPosition = R(144.0);
			part[1].maxVelocity = R(5.0e1);
			part[1].maxForce = R(1.0e8);
			part[1].piece = new Door (this, part[1].lock, R(48.0), R(144.0), gSpriteMgr.Fetch ("door10s"), part[1].axis);
			gReferee->Install (part[1].piece, FULL_INTERSECT);
			break;
	//------------------------------
//	NEW DOOR 13
		case 13:
			doorStart = 27;	//	these numbers are wrong
			doorStop = 28;	//	these numbers are wrong
			count = 2;
			part = new DoorPart[2];
			part[0].lock = tLoc - Vector_2d (Vector_2d (R(24.0), ZERO));
			part[0].axis (R(-1.0), ZERO);
			part[0].maxPosition = R(48.0);
			part[0].maxVelocity = R(1.0e2);
			part[0].maxForce = R(2.0e8);
			part[0].piece = new Door (this, part[0].lock, R(48.0), R(96.0), gSpriteMgr.Fetch ("door13w"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			part[1].lock = tLoc + Vector_2d (Vector_2d (R(24.0), ZERO));
			part[1].axis (R(1.0), ZERO);
			part[1].maxPosition = R(48.0);
			part[1].maxVelocity = R(1.0e2);
			part[1].maxForce = R(2.0e8);
			part[1].piece = new Door (this, part[1].lock, R(48.0), R(96.0), gSpriteMgr.Fetch ("door13e"), part[1].axis);
			gReferee->Install (part[1].piece, FULL_INTERSECT);
			break;
//------------------------------
		case 14:
			doorStart = snd_lgdoor3;
			doorStop = snd_lgdrstp3;
			count = 1;
			part = new DoorPart [1];
			part[0].lock = tLoc;
			part[0].axis (ZERO, R(1.0));
			part[0].maxPosition = R(1.84e3);
			part[0].maxVelocity = R(2.5e1);
			part[0].maxForce = R(7.0e7);
			part[0].piece = new Door (this, part[0].lock, R(224.0), R(112.0), gSpriteMgr.Fetch ("door14"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

			//	BossRoom Exit doors: 15 fast, 16 med, 17 slow

		case 15:
			doorStart = 25;
			doorStop = 26;
			count = 2;
			part = new DoorPart[2];
			part[0].lock = tLoc + Vector_2d (R(40.0), ZERO);
			part[0].axis (R(1.0), ZERO);
			part[0].maxPosition = R(50.0);
			part[0].maxVelocity = R(1.0e2);
			part[0].maxForce = R(1.0e8);
			part[0].piece = new Door (this, part[0].lock, R(80.0), R(64.0), gSpriteMgr.Fetch ("door04e"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			part[1].lock = tLoc - Vector_2d (R(40.0), ZERO);
			part[1].axis (R(-1.0), ZERO);
			part[1].maxPosition = R(50.0);
			part[1].maxVelocity = R(1.0e2);
			part[1].maxForce = R(1.0e8);
			part[1].piece = new Door (this, part[1].lock, R(80.0), R(64.0), gSpriteMgr.Fetch ("door04w"), part[1].axis);
			gReferee->Install (part[1].piece, FULL_INTERSECT);
			break;

		case 16:
			doorStart = 25;
			doorStop = 26;
			count = 2;
			part = new DoorPart[2];
			part[0].lock = tLoc + Vector_2d (R(40.0), ZERO);
			part[0].axis (R(1.0), ZERO);
			part[0].maxPosition = R(50.0);
			part[0].maxVelocity = R(5.0e1);
			part[0].maxForce = R(1.0e8);
			part[0].piece = new Door (this, part[0].lock, R(80.0), R(64.0), gSpriteMgr.Fetch ("door04e"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			part[1].lock = tLoc - Vector_2d (R(40.0), ZERO);
			part[1].axis (R(-1.0), ZERO);
			part[1].maxPosition = R(50.0);
			part[1].maxVelocity = R(5.0e1);
			part[1].maxForce = R(1.0e8);
			part[1].piece = new Door (this, part[1].lock, R(80.0), R(64.0), gSpriteMgr.Fetch ("door04w"), part[1].axis);
			gReferee->Install (part[1].piece, FULL_INTERSECT);
			break;

		case 17:
			doorStart = 25;
			doorStop = 26;
			count = 2;
			part = new DoorPart[2];
			part[0].lock = tLoc + Vector_2d (R(40.0), ZERO);
			part[0].axis (R(1.0), ZERO);
			part[0].maxPosition = R(50.0);
			part[0].maxVelocity = R(2.5e1);
			part[0].maxForce = R(1.0e8);
			part[0].piece = new Door (this, part[0].lock, R(80.0), R(64.0), gSpriteMgr.Fetch ("door04e"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			part[1].lock = tLoc - Vector_2d (R(40.0), ZERO);
			part[1].axis (R(-1.0), ZERO);
			part[1].maxPosition = R(50.0);
			part[1].maxVelocity = R(2.5e1);
			part[1].maxForce = R(1.0e8);
			part[1].piece = new Door (this, part[1].lock, R(80.0), R(64.0), gSpriteMgr.Fetch ("door04w"), part[1].axis);
			gReferee->Install (part[1].piece, FULL_INTERSECT);
			break;

		case 18:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (R(1.0), ZERO);
			part[0].maxPosition = R(80.0);
			part[0].maxVelocity = R(2.0e2);
			part[0].maxForce = R(1.0e9);
			part[0].piece = new Door (this, part[0].lock, R(208.0), R(48.0), gSpriteMgr.Fetch ("door18"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case 19:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (R(-1.0), ZERO);
			part[0].maxPosition = R(80.0);
			part[0].maxVelocity = R(2.0e2);
			part[0].maxForce = R(1.0e9);
			part[0].piece = new Door (this, part[0].lock, R(208.0), R(48.0), gSpriteMgr.Fetch ("door19"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case 20:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (R(1.0), ZERO);
			part[0].maxPosition = R(80.0);
			part[0].maxVelocity = R(2.5e1);
			part[0].maxForce = R(1.0e8);
			part[0].piece = new Door (this, part[0].lock, R(80.0), R(96.0), gSpriteMgr.Fetch ("door20"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case 21:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (R(-1.0), ZERO);
			part[0].maxPosition = R(80.0);
			part[0].maxVelocity = R(2.5e1);
			part[0].maxForce = R(1.0e8);
			part[0].piece = new Door (this, part[0].lock, R(80.0), R(96.0), gSpriteMgr.Fetch ("door21"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case 22:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (R(1.0), ZERO);
			part[0].maxPosition = R(64.0);
			part[0].maxVelocity = R(2.5e1);
			part[0].maxForce = R(1.0e8);
			part[0].piece = new Door (this, part[0].lock, R(160.0), R(160.0), gSpriteMgr.Fetch ("door22"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case 23:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (R(-1.0), ZERO);
			part[0].maxPosition = R(64.0);
			part[0].maxVelocity = R(2.5e1);
			part[0].maxForce = R(1.0e8);
			part[0].piece = new Door (this, part[0].lock, R(160.0), R(160.0), gSpriteMgr.Fetch ("door23"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case 24:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (ZERO, R(1.0));
			part[0].maxPosition = R(64.0);
			part[0].maxVelocity = R(4.0e2);
			part[0].maxForce = R(5.0e8);
			part[0].piece = new Door (this, part[0].lock, R(64.0), R(80.0), gSpriteMgr.Fetch ("door24"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case 25:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (R(-1.0), ZERO);
			part[0].maxPosition = R(32.0);
			part[0].maxVelocity = R(4.0e2);
			part[0].maxForce = R(5.0e8);
			part[0].piece = new Door (this, part[0].lock, R(96.0), R(96.0), gSpriteMgr.Fetch ("door25"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case 26:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (R(1.0), ZERO);
			part[0].maxPosition = R(32.0);
			part[0].maxVelocity = R(4.0e2);
			part[0].maxForce = R(5.0e8);
			part[0].piece = new Door (this, part[0].lock, R(96.0), R(96.0), gSpriteMgr.Fetch ("door26"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;
	
		// JB MY DOORS!!!!!

		//	new slew of doors, 27-46.  if 39-42 don't work, use 43-47 instead.
		case  27:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (R(-1.0), ZERO);
			part[0].maxPosition = R(64.0);
			part[0].maxVelocity = R(50.0);
			part[0].maxForce = R(5.0e8);
			part[0].piece = new Door (this, part[0].lock, R(160.0), R(160.0), gSpriteMgr.Fetch ("door27"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case  28:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (R(-1.0), ZERO);
			part[0].maxPosition = R(64.0);
			part[0].maxVelocity = R(5.0e1);
			part[0].maxForce = R(5.0e8);
			part[0].piece = new Door (this, part[0].lock, R(160.0), R(160.0), gSpriteMgr.Fetch ("door28"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case  29:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (R(1.0), ZERO);
			part[0].maxPosition = R(64.0);
			part[0].maxVelocity = R(5.0e1);
			part[0].maxForce = R(5.0e8);
			part[0].piece = new Door (this, part[0].lock, R(160.0), R(160.0), gSpriteMgr.Fetch ("door29"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case  30:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (R(1.0), ZERO);
			part[0].maxPosition = R(64.0);
			part[0].maxVelocity = R(5.0e1);
			part[0].maxForce = R(5.0e8);
			part[0].piece = new Door (this, part[0].lock, R(160.0), R(160.0), gSpriteMgr.Fetch ("door30"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case  31:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (ZERO, R(1.0));
			part[0].maxPosition = R(64.0);
			part[0].maxVelocity = R(5.0e1);
			part[0].maxForce = R(5.0e8);
			part[0].piece = new Door (this, part[0].lock, R(160.0), R(160.0), gSpriteMgr.Fetch ("door31"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case  32:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (ZERO, R(1.0));
			part[0].maxPosition = R(64.0);
			part[0].maxVelocity = R(5.0e1);
			part[0].maxForce = R(5.0e8);
			part[0].piece = new Door (this, part[0].lock, R(160.0), R(160.0), gSpriteMgr.Fetch ("door32"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case  33:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (ZERO, R(-1.0));
			part[0].maxPosition = R(64.0);
			part[0].maxVelocity = R(5.0e1);
			part[0].maxForce = R(5.0e8);
			part[0].piece = new Door (this, part[0].lock, R(160.0), R(160.0), gSpriteMgr.Fetch ("door33"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case  34:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (ZERO, R(-1.0));
			part[0].maxPosition = R(64.0);
			part[0].maxVelocity = R(5.0e1);
			part[0].maxForce = R(5.0e8);
			part[0].piece = new Door (this, part[0].lock, R(160.0), R(160.0), gSpriteMgr.Fetch ("door34"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case  35:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (R(1.0), ZERO);
			part[0].maxPosition = R(64.0);
			part[0].maxVelocity = R(7.5e1);
			part[0].maxForce = R(5.0e8);
			part[0].piece = new Door (this, part[0].lock, R(64.0), R(48.0), gSpriteMgr.Fetch ("door35"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case  36:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (R(-1.0), ZERO);
			part[0].maxPosition = R(64.0);
			part[0].maxVelocity = R(7.5e1);
			part[0].maxForce = R(5.0e8);
			part[0].piece = new Door (this, part[0].lock, R(64.0), R(48.0), gSpriteMgr.Fetch ("door36"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case  37:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (ZERO, R(-1.0));
			part[0].maxPosition = R(64.0);
			part[0].maxVelocity = R(7.5e1);
			part[0].maxForce = R(5.0e8);
			part[0].piece = new Door (this, part[0].lock, R(64.0), R(48.0), gSpriteMgr.Fetch ("door37"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case  38:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (ZERO, R(1.0));
			part[0].maxPosition = R(64.0);
			part[0].maxVelocity = R(7.5e1);
			part[0].maxForce = R(5.0e8);
			part[0].piece = new Door (this, part[0].lock, R(64.0), R(48.0), gSpriteMgr.Fetch ("door38"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		//	do the last diagonal doors.
		case  39:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (R(-1.0), R(1.0));
			part[0].maxPosition = R(96.0) * SQRT (R(2.0));
			part[0].maxVelocity = R(1.5e1);
			part[0].maxForce = R(5.0e8);
			part[0].piece = new Door (this, part[0].lock, R(128.0), R(128.0), gSpriteMgr.Fetch ("door39"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case  40:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (R(1.0), R(1.0));
			part[0].maxPosition = R(96.0) * SQRT (R(2.0));
			part[0].maxVelocity = R(1.5e1);
			part[0].maxForce = R(5.0e8);
			part[0].piece = new Door (this, part[0].lock, R(128.0), R(128.0), gSpriteMgr.Fetch ("door40"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case  41:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (R(-1.0), R(-1.0));
			part[0].maxPosition = R(96.0) * SQRT (R(2.0));
			part[0].maxVelocity = R(1.5e1);
			part[0].maxForce = R(5.0e8);
			part[0].piece = new Door (this, part[0].lock, R(128.0), R(128.0), gSpriteMgr.Fetch ("door41"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case  42:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (R(1.0), R(-1.0));
			part[0].maxPosition = R(96.0) * SQRT (R(2.0));
			part[0].maxVelocity = R(1.5e1);
			part[0].maxForce = R(5.0e8);
			part[0].piece = new Door (this, part[0].lock, R(128.0), R(128.0), gSpriteMgr.Fetch ("door42"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case  43:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (ZERO, R(-1.0));
			part[0].maxPosition = R(64.0);
			part[0].maxVelocity = R(1.5e1);
			part[0].maxForce = R(5.0e8);
			part[0].piece = new Door (this, part[0].lock, R(128.0), R(128.0), gSpriteMgr.Fetch ("door43"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case  44:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (ZERO, R(1.0));
			part[0].maxPosition = R(64.0);
			part[0].maxVelocity = R(1.5e1);
			part[0].maxForce = R(5.0e8);
			part[0].piece = new Door (this, part[0].lock, R(128.0), R(128.0), gSpriteMgr.Fetch ("door44"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case  45:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (R(1.0), ZERO);
			part[0].maxPosition = R(64.0);
			part[0].maxVelocity = R(1.5e1);
			part[0].maxForce = R(5.0e8);
			part[0].piece = new Door (this, part[0].lock, R(128.0), R(128.0), gSpriteMgr.Fetch ("door45"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case  46:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (R(-1.0), ZERO);
			part[0].maxPosition = R(64.0);
			part[0].maxVelocity = R(1.5e1);
			part[0].maxForce = R(5.0e8);
			part[0].piece = new Door (this, part[0].lock, R(128.0), R(128.0), gSpriteMgr.Fetch ("door46"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case	47:	//	030397	JB
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (ZERO, R(-1.0));
			part[0].maxPosition = R(64.0);
			part[0].maxVelocity = R(1.0e2);
			part[0].maxForce = R(2.0e8);
			part[0].piece = new Door (this, part[0].lock, R(32.0), R(64.0), gSpriteMgr.Fetch ("door47"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case	48:	//	030397	JB
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (R(1.0), ZERO);
			part[0].maxPosition = R(64.0);
			part[0].maxVelocity = R(1.0e2);
			part[0].maxForce = R(2.0e8);
			part[0].piece = new Door (this, part[0].lock, R(64.0), R(32.0), gSpriteMgr.Fetch ("door48"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case	49:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart[count];
			part[0].lock = tLoc;
			part[0].axis (ZERO, R(-1.0));
			part[0].maxPosition = R(80.0);
			part[0].maxVelocity = R(2.0e2);
			part[0].maxForce = R(3.0e8);
			part[0].piece = new Door (this, part[0].lock, R(48.0), R(80.0), gSpriteMgr.Fetch ("door49"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case	50:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (ZERO, R(1.0));
			part[0].maxPosition = R(80.0);
			part[0].maxVelocity = R(2.0e2);
			part[0].maxForce = R(3.0e8);
			part[0].piece = new Door (this, part[0].lock, R(48.0), R(80.0), gSpriteMgr.Fetch ("door50"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;

		case	51:
			doorStart = 25;
			doorStop = 26;
			count = 1;
			part = new DoorPart [count];
			part[0].lock = tLoc;
			part[0].axis (R(1.0), ZERO);
			part[0].maxPosition = R(80.0);
			part[0].maxVelocity = R(2.0e2);
			part[0].maxForce = R(3.0e8);
			part[0].piece = new Door (this, part[0].lock, R(80.0), R(48.0), gSpriteMgr.Fetch ("door51"), part[0].axis);
			gReferee->Install (part[0].piece, FULL_INTERSECT);
			break;
	}
	open = FALSE;
	stayopen = FALSE;
	route = TRUE;
}																																								//	end

//------------------------------------------------------------------------------
DoorPlayer::~DoorPlayer (void)																									//	destructor
{																																								//	begin
	for (int i = 0; i < count; i++)																								//	loop over all of the door parts
		if (gReferee->Confirm (part[i].piece))																			//	if the door piece is still part of the world
			gReferee->Remove(part[i].piece);																					//	remove the door piece
}																																								//	end

//------------------------------------------------------------------------------
Bool	DoorPlayer::Play (void)																										//	play one frame
{																																								//	begin
	open |= stayopen;																															//	force open TRUE if stayopen TRUE. Heh.
	for (int i = 0; i < count; i++)																								//	loop over all of the door parts
	{
		Body	*body = part[i].piece->GetBody ();
		Real	deltaVelocity;
		if (power)
		{
			Vector_2d	deltaVector = body->TLocation () - part[i].lock;
			Real			position = deltaVector | part[i].axis,
								deltaPosition;
			if (open)
				deltaPosition = part[i].maxPosition - position;
			else
				deltaPosition = -position;
			deltaVelocity = deltaPosition / gReferee->Interval ();

			Real	v = body->TVelocity () | part[i].axis,
						fv = FABS (v);
			if ((FABS (deltaPosition) < R(5.0e-2)) && (fv > R(1.0e-2)))
				gSoundMgr->PlaySound (doorStop, part[i].piece);																			// (MK) Pass PiecePtr Info
			if (FABS (deltaPosition) > R(5.0))
			 if ((fv < EPSILON) || (Sign (deltaVelocity) != Sign (v)))
				gSoundMgr->PlaySound (doorStart, part[i].piece);																		// (MK) Pass PiecePtr Info
			if (FABS (deltaVelocity) > part[i].maxVelocity)
				deltaVelocity = Sign (deltaVelocity) * part[i].maxVelocity;
			deltaVelocity -= body->TVelocity () | part[i].axis;
		}
		else
		{
			deltaVelocity = -body->TVelocity () | part[i].axis;
			if ((FABS (deltaVelocity) > ZERO) && (FABS (deltaVelocity) < R(1.0e-2)))
				gSoundMgr->PlaySound (doorStop, part[i].piece);																			// (MK) Pass PiecePtr Info
		}
		Real	deltaForce = deltaVelocity * body->TInertia ();
		if (FABS (deltaForce) > part[i].maxForce)
			deltaForce = Sign (deltaForce) * part[i].maxForce;
		if (deltaForce != ZERO)
		{
			body->ApplyForce (part[i].axis * deltaForce);
			gReferee->Update (part[i].piece);
		}
	}
	open = FALSE;
	return TRUE;
}																																								//	end

//------------------------------------------------------------------------------
int		DoorPlayer::Trigger (const PiecePtr &piece, int type, int what)						//	trigger the device
{																																								//	begin
	Device::Trigger (piece, type, what);
	if (type == DEV_TRIGGER)
	switch (what)
	{
		case JAM_OPEN:
			stayopen = TRUE;
			break;
		case UNJAM:	
			stayopen = FALSE;
			break;
		case GENERIC:																																//	normal open/close behavior
			open = TRUE;
	}
	return 0;
}																																								//	end

//------------------------------------------------------------------------------
