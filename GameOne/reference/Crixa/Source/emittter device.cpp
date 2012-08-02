//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:		Jeremy Biddle
//
//	02/20/97	JB		creation.
//	02/25/97	JB		added kNoSound constant and emitting boolean.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "emitter device.h"
#include "emitter piece.h"
#include "sound mgr.h"

//------------------------------------------------------------------------------
EmitterDevice::EmitterDevice (int id, Point_2d loc, int emSnd, int startSnd,		//	constructor
															int	stopSnd) :																		//	constructor (continued)
Device (id)																																			//	constructor (continued)
{
	emSound = emSnd;																															//	record the emitter loop sound
	startSound = startSnd;																												//	record the emitter start sound
	stopSound = stopSnd;																													//	record the emitter stop sound
	emPiece = PiecePtr (new EmitterPiece (loc));																	//	determine the spacial location fo the emitter
	emitting = FALSE;
}

//------------------------------------------------------------------------------
EmitterDevice::~EmitterDevice (void)
{
}

//------------------------------------------------------------------------------
Bool	EmitterDevice::SwitchOn (void)																						//	switch the device on
{
	if (Device::SwitchOn ())																											//	if the device successfully switches on
	{
		if (startSound != kNoSound)																									//	if a valid start sound has been specified
			gSoundMgr->PlaySound (startSound, emPiece);																//	play the start sound, once
		emID = gSoundMgr->InsertEMSound (emSound, emPiece);													//	insert an emitter
		emitting = TRUE;
		return TRUE;
	}
	else
		return FALSE;
}

//------------------------------------------------------------------------------
Bool	EmitterDevice::SwitchOff (void)																						//	switch the device off
{
	if (Device::SwitchOff ())																											//	if the device successfully switches off
	{
		if (stopSound != kNoSound)																									//	if a valid stop sound has been specified
			gSoundMgr->PlaySound (stopSound, emPiece);																//	play the stop sound
		gSoundMgr->RemoveEMSound (emSound, emID);																		//	remove the emitter sound
		emitting = FALSE;
		return TRUE;
	}
	else
		return FALSE;
}

//------------------------------------------------------------------------------
