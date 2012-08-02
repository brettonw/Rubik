//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/30/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "korves ship.h"
#include "get parameters.h"
#include "sprite array mgr.h"
#include "traik.h"
#include "resource.h"

//------------------------------------------------------------------------------
KorvesShip::KorvesShip (Player *own, Real radius, CPoint loc, Real rloc,				//	constructor
												SArrayPtr ship) :																				//	constructor (continued)
WeaponShip (own, radius, loc, rloc, ship, 10)																		//	constructor (continued)
{																																								//	begin
	AdjustThrust (ParmReal (KORVES_T_THRUST), ParmReal (KORVES_R_THRUST));				//	adjust the korves ship thrust parameters
	SetNozzles (1, Point_2d (ZERO, ZERO));																				//	set the korves ship thrust nozzle points
	SetArmor (ParmReal (KORVES_ARMOR));																						//	set the korves ship armor
	SetHardness (ParmReal (KORVES_HARDNESS));																			//	set the korves ship hardness
	Traik	*traik = new Traik (	R(1.0),
															R(5.0e2), 
															this, 
															Point_2d (R(-31.0), R(61.0)), 
															R(1.0e6),
															R(2.0),
															R(100.0),
															gSpriteMgr.Fetch ("round"),
															gSpriteMgr.Fetch (ParmString (TRAIK_SPLUT)),
															1,
															R(-4.0e-1));																			//	create the traik
	traik->AddAmmoCount (500);
	traik->SetHeating (	ParmReal (TRAIK_HEAT_RATE), 
											ParmReal (TRAIK_COOL_RATE));
	SetWeapon (1, traik);																													//	set the first weapon
	SelectWeapon (1);																															//	select the traik
}																																								//	end

//------------------------------------------------------------------------------
KorvesShip::~KorvesShip (void)																									//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Bool	KorvesShip::AddResource (int type, int number)														//	add a resource to the piece
{																																								//	begin
	switch (type)
	{
		case RESOURCE_KORVES_HEALTH:
			if (life < R(1.0))
			{
				life += Real (number) * R(1.0e-2);
				if (life > R(1.0))
					life = R(1.0);
				return TRUE;																															//	return TRUE to indicate that the resource was collected
			}
			break;
	}
	return FALSE;																																	//	return FALSE to indicate that the resource wa snot collected
}																																								//	end

//------------------------------------------------------------------------------
