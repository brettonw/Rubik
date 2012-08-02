//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Revisions:			01/31/97: creation.  (JB)
//									02/04/97:	spread modifications. (JB)
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "furnace thug.h"
#include "furnace.h"
#include "referee.h"
#include "furnace blaster.h"
#include "sprite array mgr.h"

//------------------------------------------------------------------------------
FurnaceThug::FurnaceThug (CPoint tLoc, Real rLoc, Real rad, SArrayPtr spr,			//	constructor
													int trigger) :																				//	constructor (continued)
Player ("The Furnace"),																													//	constructor (continued)
Device (trigger)																																//	constructor (continued)
{																																								//	begin
	trigger = FALSE;																															//	start out NOT firing
	furnace = new Furnace (this, rad, tLoc, rLoc, spr);														//	create a new turret
	furnace->SetArmor (ZERO);																											//	set the armor (INVINCIBLE)
	Point_2d gunLoc (ZERO, rad / R(2.0));																					//	set the muzzle point
	FurnaceBlaster *furnaceGun = new FurnaceBlaster (R(2.0e2), furnace, gunLoc, R(4.0e-1),//	create a plasma weapon
																	 R(2.5), gSpriteMgr.Fetch ("fplasma"), 2);		//	create a plasma weapon (continued)
	furnaceGun->AddAmmoCount (100000);																						//	give the furnace lots of "fuel"
	((Furnace *) furnace ())->SetWeapon (furnaceGun);															//	attach the plasma weapon
	gReferee->Install (furnace);																									//	install the turret into the world
}																																								//	end

//------------------------------------------------------------------------------
FurnaceThug::~FurnaceThug (void)																								//	destructor
{																																								//	begin
	if (gReferee->Confirm (furnace))																							//	if the turret is still part of the world
		gReferee->Remove (furnace);																									//	remove the turret
}																																								//	end

//------------------------------------------------------------------------------
Bool	FurnaceThug::Play (void)																									//	play time for the furnace
{																																								//	begin
	if (gReferee->Confirm (furnace))																							//	make sure that the furnace is still there
	{																																							//	begin
		if (trigger)																																//	if the turret has been triggered
		{																																						//	begin
			((Furnace *) furnace ())->Fire ();																				//	BLAZE!
			trigger = FALSE;																													//	cancel the trigger (until next frame)
		}																																						//	end
		return TRUE;																																//	the turret is still in play
	}																																							//	end
	return FALSE;																																	//	the turret is out of play
}																																								//	end

//------------------------------------------------------------------------------
int	FurnaceThug::Trigger (const PiecePtr &piece, int code, int code2)
{																																								//	begin
	if (code == DEV_TRIGGER && power)																							//	if the device is being triggered
		trigger = TRUE;																															//	"trigger" the furnace
	return Device::Trigger (piece, code, code2);																	//	trigger the device
}																																								//	end

//------------------------------------------------------------------------------
