//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	08/03/96	JB		creation & last undocumented revision.
//	02/12/97	MK		sound emitter calls.
//	02/20/97	JB		modified to use device emitter object.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "laser barrier gen.h"
#include "referee.h"
#include "switch sequence.h"
#include "sprite array mgr.h"
#include "sprite sequence.h"
#include "time sequence.h"
#include "view.h"

//------------------------------------------------------------------------------
#define BIGBASE R(192.0)
#define SMALLBASE R(128.0)
#define BARRIERSTART 49
#define BARRIERLOOP 41
#define BARRIERSTOP 53

//------------------------------------------------------------------------------
LaserBarrierGen::LaserBarrierGen (int type, int id, Real x, Real y, Real width,	//	constructor
																	Real height, Real dam) :											//	constructor (continued)
Device (id),																																		//	constructor (continued)
soundEmitter (id, Point_2d (x, y), BARRIERLOOP, BARRIERSTART, BARRIERSTOP)			//	constructor (continued)
{
	char	lasername[20];																													//	array for name manipulation of the laser barriers
	Real	base;																																		//	??

	switch (type)
	{
		case 0:	//	horiz and thin
			strcpy (lasername, "las01");
			base = SMALLBASE;
			break;
		case 1:	//	vert and thin
			strcpy (lasername, "las03");
			base = SMALLBASE;
			break;
		case 2:	//	horiz and thick
			base = BIGBASE;
			strcpy (lasername, "las02");
			break;
		case 3:	//	vert and thick
			strcpy (lasername, "las04");
			base = BIGBASE;
			break;
	}

	Real new_x, new_y;
	if (width > height)
	{
		length = int (width / base);
		new_x = x - (base / R(2.0)) * Real (length - 1);
		sequences = new SeqPtr [length];
		for (int i = 0; i < length; i++)
			sequences[i] = SeqPtr (new SwitchSeq (Point_2d (new_x + i * base, y),
															SeqPtr (new SpriteSeq (gSpriteMgr.Fetch ("blank"))),
															SeqPtr (new TimeSeq (gSpriteMgr.Fetch (lasername), TRUE, R(0.05)))));;
	}
	else
	{
		length = int (height / base);
		new_y = y - (base / R(2.0)) * Real (length - 1);
		sequences = new SeqPtr [length];
		for (int i = 0; i < length; i++)
			sequences[i] = SeqPtr (new SwitchSeq (Point_2d (x, new_y + i * base),
															SeqPtr (new SpriteSeq (gSpriteMgr.Fetch ("blank"))),
															SeqPtr (new TimeSeq (gSpriteMgr.Fetch (lasername), TRUE, R(0.05)))));
	}

	for (int i = 0; i < length; i++)
		gView->Install (sequences[i]);

	laserbarrier = PiecePtr (new LaserBarrier (0, Point_2d (x, y), width, height, dam));
}

//------------------------------------------------------------------------------
LaserBarrierGen::~LaserBarrierGen (void)
{
}

//------------------------------------------------------------------------------
Bool LaserBarrierGen::SwitchOn (void)																						//	switch the laser barrier on
{
	if (Device::SwitchOn ())																											//	if the laser barrier successfully turns on
	{
		soundEmitter.SwitchOn ();																										//	switch on the sound emitter
		gReferee->Install (laserbarrier, FULL_INTERSECT);														//	install the laser barrier piece in the referee
		for (int i = 0; i < length; i++)																						//	loop through the lbarrier visuals
			((SwitchSeq *) sequences[i] ())->Switch (on);															//	and switch them on
		return TRUE;																																//	successful switch on
	}
	return FALSE;																																	//	failed switch on
}

//------------------------------------------------------------------------------
Bool LaserBarrierGen::SwitchOff (void)																					//	switch off the laser barrier
{
	if (Device::SwitchOff ())																											//	if the laser barrier successfully turns off
	{
		soundEmitter.SwitchOff ();																									//	switch off the sound emitter
		gReferee->Remove (laserbarrier);																						//	remove the laser barrier piece from the referee
		for (int i = 0; i < length; i++)																						//	loop through the lbarrier visuals
			((SwitchSeq *) sequences[i] ())->Switch (on);															//	and switch them off
		return TRUE;																																//	succcessful switch off
	}
	return FALSE;																																	//	failed switch off
}

//------------------------------------------------------------------------------
