//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	02/03/97
//
//	Revisions:			02/14/97:	added RESOURCE_HEALTH_EXCHANGE stuff (JB)
//									02/19/97:	ammo hack for adding ammo from ammo upgraders (JB)
// 
//	02/19/97	MK		Added Sound Defines to map sound names to numbers
//						MK		Added SPRINGSOUND to SpringLine Weapon Creation
//						MK		Added Add Health capability to Generic Resources
//						MK    Added PiecePtr info to Generic Resource Sound Call
//	03/04/97	DS		Modified Generic Resources so can subtract.	
//	03/05/97	DS		Modified AddResource to eat Health Resources, also play noise.
//	03/18/97	DS		Modified AddResource to be maxammo aware
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "crixa ship.h"
#include "sound mgr.h"
#include "traik.h"
#include "flechette.h"
#include "get parameters.h"
#include "sprite array mgr.h"
#include "pixel.h"
#include "spring gun.h"
#include "plasma gun.h"
#include "laser gun.h"
#include "mine launcher.h"
#include "rocket launcher.h"
#include "cluster weapon.h"
#include "minetraik.h"
#include "resource.h"
#include "ship explosion sequence.h"
#include "view.h"
#include "trigger array.h"
#include "debugger.h"

// (MK) Sound Defs used to pass sounds to PlaySound
#define SPRINGSOUND 46
#define RESOURCESOUND 58
#define CRIXAHIT 59
#define SHIELDHIT 52
#define WEAPONPICKUP 17
#define TACMINE_WEP_SOUND	67


//------------------------------------------------------------------------------
CrixaShip::CrixaShip (Player *own, Real radius, CPoint loc, Real rloc,					//	constructor
												SArrayPtr ship) :																				//	constructor (continued)
WeaponShip (own, radius, loc, rloc, ship, 10)																		//	constructor (continued)
{																																								//	begin
	maxammo = new int [10];
	maxammo[0] = ParmInteger (CRIXA_SPRINGLINE_MAXAMMO);
	maxammo[1] = ParmInteger (CRIXA_TRAIK_MAXAMMO);
	maxammo[2] = ParmInteger (CRIXA_FLECHETTE_MAXAMMO);
	maxammo[3] = ParmInteger (CRIXA_LASER_MAXAMMO);
	maxammo[4] = ParmInteger (CRIXA_PLASMA_MAXAMMO);
	maxammo[5] = ParmInteger (CRIXA_NONE_MAXAMMO);
	maxammo[6] = ParmInteger (CRIXA_NONE_MAXAMMO);
	maxammo[7] = ParmInteger (CRIXA_ROCKET_MAXAMMO);
	maxammo[8] = ParmInteger (CRIXA_TACMINE_MAXAMMO);
	maxammo[9] = ParmInteger (CRIXA_MINE_MAXAMMO);
	ammocost = new int [10];
	ammocost[0] = ParmInteger (CRIXA_SPRINGLINE_AMMOCOST);
	ammocost[1] = ParmInteger (CRIXA_TRAIK_AMMOCOST);
	ammocost[2] = ParmInteger (CRIXA_FLECHETTE_AMMOCOST);
	ammocost[3] = ParmInteger (CRIXA_LASER_AMMOCOST);
	ammocost[4] = ParmInteger (CRIXA_PLASMA_AMMOCOST);
	ammocost[5] = ParmInteger (CRIXA_NONE_AMMOCOST);
	ammocost[6] = ParmInteger (CRIXA_NONE_AMMOCOST);
	ammocost[7] = ParmInteger (CRIXA_ROCKET_AMMOCOST);
	ammocost[8] = ParmInteger (CRIXA_TACMINE_AMMOCOST);
	ammocost[9] = ParmInteger (CRIXA_MINE_AMMOCOST);

	life += R(1.0);																																//	add extra life to crixa ship
	maxlevel = 1;																																	//	upgrade level of ship
	power = R(0.0);
	AdjustThrust (ParmReal (CRIXA_T_THRUST), 
													ParmReal (CRIXA_R_THRUST), 
													ParmReal (CRIXA_STOPDIST));														//	adjust the crixa ship thrust parameters
	SetNozzles (2, Point_2d (R(9.0), R(-12.0)), Point_2d (R(-9.0), R(-12.0)));		//	set the crixa ship thrust nozzle points
	SetArmor (ParmReal (CRIXA_ARMOR));																						//	set the crixa ship armor
	SetHardness (ParmReal (CRIXA_HARDNESS));																			//	set the crixa ship hardness
	SpringGun	*spring = new SpringGun
	(
		ParmReal (SPRING_RATE), 
		ParmReal (SPRING_RANGE), 
		this, 
		ParmReal (SPRING_HOLD_TIME),
		ParmReal (SPRING_STUN_TIME),
		gSpriteMgr.Fetch (ParmString (SPRING_SPLUT)),
		SPRINGSOUND
	);																																						//	create the spring gun
	spring->AddAmmoCount (9999);
	SetWeapon (0, spring);																												//	set the fourth weapon
	Traik	*traik = new Traik (	ParmReal (TRAIK_RATE),
															INFINITY, 
															this, 
															Point_2d (ParmReal (TRAIK_MUZZLE_X), ParmReal (TRAIK_MUZZLE_Y)), 
															ParmReal (TRAIK_FORCE),
															ParmReal (TRAIK_RADIUS),
															ParmReal (TRAIK_DAMAGE),
															gSpriteMgr.Fetch (ParmString (TRAIK_SPRITE)),
															gSpriteMgr.Fetch (ParmString (TRAIK_SPLUT)),
															1,
															ParmReal (TRAIK_RECOIL_FACTOR));									//	create the traik
	traik->AddAmmoCount (1500);
	traik->SetHeating (	ParmReal (TRAIK_HEAT_RATE), 
											ParmReal (TRAIK_COOL_RATE));
	SetWeapon (1, traik);																													//	set the first weapon
	SelectWeapon (1);																															//	select the traik
	SetThrustSprite (gSpriteMgr.Fetch ("crthrust"));
	AdjustResources (ParmInteger (CRIXA_RESOURCES));
	//AddResource (RESOURCE_TACMINE_GUN,1000);
	//AddResource (RESOURCE_ROCKET_GUN,1000);
}																																								//	end

//------------------------------------------------------------------------------
CrixaShip::~CrixaShip (void)																										//	destructor
{																																								//	begin
	delete[] ammocost;
	delete[] maxammo;
}																																								//	end

//------------------------------------------------------------------------------
void	CrixaShip::Damage (const PiecePtr &piece, Real damage)										//	damage the piece
{																																								//	begin
	stun = stunMax;																																//	add the ship stun time
	Piece::Damage (piece, damage);																								//	ds: pass remaining damage along
	
	if (life > R(1.0))
		gSoundMgr->PlaySound(SHIELDHIT, this);																			//	(MK)If armor left, play shield hit sound
	else
		gSoundMgr->PlaySound(CRIXAHIT, this);																				// (MK) If no armor, play direct hit sound
	
	if ((!dead) && (life == ZERO))																								//	if we are dead, but haven't yet exploded
	{																																							//	begin
		dead = TRUE;																																//	set the dead flag
		char	buf[256];
		if (piece ())
		{
			Player	*pOwner = piece->GetOwner ();
			if (pOwner)
			{
				if (pOwner != owner)
					pOwner->AddScore (1, 0, 0, 0, 0, value);
				else
					pOwner->AddScore (0, 0, 0, 0, 1, value);
				sprintf (buf, "%s destroyed by %s", owner->Name (), pOwner->Name ());
			}
			else
				sprintf (buf, "%s destroyed by laser barrier", owner->Name ());
		}
		else
			sprintf (buf, "%s destroyed by debris", owner->Name ());
		owner->AddScore (0, 1, 0, 0, 0, 0);
		gTrigger->Trigger (1, this, DEV_TEXT_MESSAGE, int (buf));
		gView->Install (new ShipExpl (this));																				//	build the explosion
		DropStuff ();																																//	drop all the stuff we are carrying
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
Bool	CrixaShip::AddResource (int type, int number)															//	add a resource to the piece
{																																								//	begin
	switch (type)
	{
		case RESOURCE_HEALTH:																												//	modified by ds to support enhanced life behavior
		{
			gSoundMgr->PlaySound (RESOURCESOUND, this);																// Generic Resource Pickup Sound
			Real maxlife = Real(maxlevel) + R(1.0);
			life += number;
			if (life > maxlife) life = maxlife;
			return TRUE;																															// return TRUE regardless to eat up health resource
		}
			break;
		case RESOURCE_TRAIK_AMMO:
			if (number + weapon[1]->AmmoCount() > maxammo[1])
				number = maxammo[1] - weapon[1]->AmmoCount();
			gSoundMgr->PlaySound (RESOURCESOUND, this);																// Generic Resource Pickup Sound
			weapon[1]->AddAmmoCount (number);
			return TRUE;																															//	return TRUE to indicate that the resource was collected
		case RESOURCE_FLECHETTE_GUN:
			if (!weapon[2])
			{
				Flechette	*flechette = new Flechette
				(	
					ParmReal (FLECHETTE_RATE),
					ParmReal (FLECHETTE_RANGE),
					this, 
					Point_2d (ParmReal (FLECHETTE_MUZZLE_X), ParmReal (FLECHETTE_MUZZLE_Y)), 
					ParmReal (FLECHETTE_FORCE),
					ParmReal (FLECHETTE_DAMAGE),
					ParmInteger (FLECHETTE_NUM_BLASTS),
					ParmReal (FLECHETTE_SPREAD),
					MakePixel (31, 15, 0),
					gSpriteMgr.Fetch (ParmString (FLECHETTE_BULLET)),
					gSpriteMgr.Fetch (ParmString (FLECHETTE_SPLUT)),
					3
				);																																			//	create the flechette
				flechette->AddAmmoCount (number);
				flechette->SetHeating (	ParmReal (FLECHETTE_HEAT_RATE), 
														ParmReal (FLECHETTE_COOL_RATE));
				SetWeapon (2, flechette);																								//	set the second weapon
				SelectWeapon (2);
				return TRUE;																														//	return TRUE to indicate that the resource was collected
			}
		case RESOURCE_FLECHETTE_AMMO:
			if (weapon[2])
			{
				if (number + weapon[2]->AmmoCount() > maxammo[2])
					number = maxammo[2] - weapon[2]->AmmoCount();
				gSoundMgr->PlaySound (RESOURCESOUND, this);															// Generic Resource Pickup Sound
				weapon[2]->AddAmmoCount (number);
				return TRUE;																														//	return TRUE to indicate that the resource was collected
			}
			break;
		case RESOURCE_LASER_GUN:
			if (!weapon[3])
			{
				LaserGun	*laser = new LaserGun
				(	
					ParmReal (LASER_RATE),
					ParmReal (LASER_RANGE),	
					this, 
					Point_2d (ParmReal (LASER_MUZZLE_X), ParmReal (LASER_MUZZLE_Y)), 
					ParmReal (LASER_DAMAGE),
					MakePixel (31, 8, 0),
					gSpriteMgr.Fetch (ParmString (LASER_SPLUT)),
					4
				);													//	create the laser gun
				laser->AddAmmoCount (number);
				laser->SetHeating (	ParmReal (LASER_HEAT_RATE), 
														ParmReal (LASER_COOL_RATE));
				SetWeapon (3, laser);																										//	set the third weapon
				SelectWeapon (3);
				return TRUE;																														//	return TRUE to indicate that the resource was collected
			}
		case RESOURCE_LASER_AMMO:
			if (weapon[3])
			{
				if (number + weapon[3]->AmmoCount() > maxammo[3])
					number = maxammo[3] - weapon[3]->AmmoCount();
				gSoundMgr->PlaySound (RESOURCESOUND, this);															// Generic Resource Pickup Sound
				weapon[3]->AddAmmoCount (number);
				return TRUE;																														//	return TRUE to indicate that the resource was collected
			}
			break;
		case RESOURCE_PLASMA_GUN:
			if (!weapon[4])
			{
				PlasmaGun	*plasma = new PlasmaGun
				(
					ParmReal (PLASMA_RANGE), 
					this, 
					Point_2d (ParmReal (PLASMA_MUZZLE_X), ParmReal (PLASMA_MUZZLE_Y)), 
					ParmReal (PLASMA_SPEED), 
					ParmReal (PLASMA_DAMAGE), 
					gSpriteMgr.Fetch (ParmString (PLASMA_SPRITE)),
					2
				);																																			//	create the plasma gun
				plasma->AddAmmoCount (number);
				plasma->SetHeating (	ParmReal (PLASMA_HEAT_RATE), 
															ParmReal (PLASMA_COOL_RATE));
				SetWeapon (4, plasma);																									//	set the fourth weapon
				SelectWeapon (4);
				return TRUE;																														//	return TRUE to indicate that the resource was collecte
			}
		case RESOURCE_PLASMA_AMMO:
			if (weapon[4])
			{
				if (number + weapon[4]->AmmoCount() > maxammo[4])
					number = maxammo[4] - weapon[4]->AmmoCount();
				gSoundMgr->PlaySound (RESOURCESOUND, this);															// Generic Resource Pickup Sound
				weapon[4]->AddAmmoCount (number);
				return TRUE;																														//	return TRUE to indicate that the resource was collected
			}
			break;
		case RESOURCE_BUSTER_GUN:
		case RESOURCE_BUSTER_AMMO:
			if (weapon[5])
			{
				gSoundMgr->PlaySound (RESOURCESOUND, this);															// Generic Resource Pickup Sound
				weapon[5]->AddAmmoCount (number);
				return TRUE;																														//	return TRUE to indicate that the resource was collected
			}
			break;
		case RESOURCE_RUPTOR_GUN:
		case RESOURCE_RUPTOR_AMMO:
			if (weapon[6])
			{
				if (number + weapon[6]->AmmoCount() > maxammo[6])
					number = maxammo[6] - weapon[6]->AmmoCount();
				gSoundMgr->PlaySound (RESOURCESOUND, this);															// Generic Resource Pickup Sound
				weapon[6]->AddAmmoCount (number);
				return TRUE;																														//	return TRUE to indicate that the resource was collected
			}
			break;
		case RESOURCE_ROCKET_GUN:
			if (!weapon[7])
			{
				RocketL		*rocket1 = new RocketL
				(
					this, 
					ParmReal (ROCKET_RATE), 
					ParmReal (ROCKET_RADIUS), 
					Point_2d (ParmReal (ROCKET_MUZZLE_X), ParmReal (ROCKET_MUZZLE_Y)), 
					ParmReal (ROCKET_DAMAGE), 
					gSpriteMgr.Fetch (ParmString (ROCKET_SPRITE)),
					ParmReal (ROCKET_T_THRUST),
					ParmReal (ROCKET_R_THRUST),
					16
				),																																			//	create the plasma gun
									*rocket2 = new RocketL
				(
					this, 
					ParmReal (ROCKET_RATE), 
					ParmReal (ROCKET_RADIUS), 
					Point_2d (-ParmReal (ROCKET_MUZZLE_X), ParmReal (ROCKET_MUZZLE_Y)), 
					ParmReal (ROCKET_DAMAGE), 
					gSpriteMgr.Fetch (ParmString (ROCKET_SPRITE)),
					ParmReal (ROCKET_T_THRUST),
					ParmReal (ROCKET_R_THRUST),
					16
				);																																			//	create the plasma gun
				rocket1->SetHeating (	ParmReal (ROCKET_HEAT_RATE), 
														ParmReal (ROCKET_COOL_RATE));
				rocket2->SetHeating (	ParmReal (ROCKET_HEAT_RATE), 
														ParmReal (ROCKET_COOL_RATE));
				Cluster	*cluster = new Cluster (this, 2, rocket1, rocket2);
				cluster->AddAmmoCount (number);
				SetWeapon (7, cluster);																									//	set the eighth weapon
				gSoundMgr->PlaySound (WEAPONPICKUP, this);															//	Weapon Pickup sound
				return TRUE;																														//	return TRUE to indicate that the resource was collected
			}
		case RESOURCE_ROCKET_AMMO:
			if (weapon[7])
			{
				if (number + weapon[7]->AmmoCount() > maxammo[7])
					number = maxammo[7] - weapon[7]->AmmoCount();
				gSoundMgr->PlaySound (RESOURCESOUND, this);															// Generic Resource Pickup Sound
				weapon[7]->AddAmmoCount (number);
				gSoundMgr->PlaySound (WEAPONPICKUP, this);															//	Weapon Pickup sound
				return TRUE;																														//	return TRUE to indicate that the resource was collected
			}
			break;
		case RESOURCE_TACMINE_GUN:
			if (!weapon[8])
			{
				MineTraik *tacmine = new MineTraik (ParmReal (TACMINE_RATE), 
					ParmReal (TACMINE_RANGE), this, 
					Point_2d (ParmReal (TACMINE_MUZZLE_X), ParmReal (TACMINE_MUZZLE_Y)), 
					ParmReal (TACMINE_EJECT_FORCE), 
					ParmReal (TACMINE_RADIUS), ParmReal (TACMINE_DAMAGE), 
					gSpriteMgr.Fetch (ParmString(TACMINE_SPRITE)), 
					gSpriteMgr.Fetch (ParmString(TACMINE_SPLUT)), 
					TACMINE_WEP_SOUND, ParmReal(TACMINE_RECOIL));
				tacmine->SetHeating(ParmReal(TACMINE_HEAT_RATE),ParmReal(TACMINE_COOL_RATE));
				SetWeapon (8, tacmine);
				tacmine->AddAmmoCount (number);
				return TRUE;
			}
		case RESOURCE_TACMINE_AMMO:
			if (weapon[8])
			{
				gSoundMgr->PlaySound (RESOURCESOUND, this);															// Generic Resource Pickup Sound
				weapon[8]->AddAmmoCount (number);
				return TRUE;																														//	return TRUE to indicate that the resource was collected
			}
			break;
		case RESOURCE_MINE_GUN:
			if (!weapon[9])
			{
				MineL		*mine = new MineL
				(
					ParmReal (MINE_RATE), 
					this, 
					Point_2d (ParmReal (MINE_MUZZLE_X), ParmReal (MINE_MUZZLE_Y)), 
					ParmReal (MINE_RADIUS), 
					ParmReal (MINE_DAMAGE), 
					gSpriteMgr.Fetch (ParmString (MINE_SPRITE)),
					5
				);																																			//	create the plasma gun
				mine->SetHeating (	ParmReal (MINE_HEAT_RATE), 
														ParmReal (MINE_COOL_RATE));
				mine->AddAmmoCount (number);
				SetWeapon (9, mine);																										//	set the ninth weapon
				gSoundMgr->PlaySound (WEAPONPICKUP, this);															// Weapon Pickup sound
				return TRUE;																														//	return TRUE to indicate that the resource was collected
			}
		case RESOURCE_MINE_AMMO:
			if (weapon[9])
			{
				if (number + weapon[9]->AmmoCount() > maxammo[9])
					number = maxammo[9] - weapon[9]->AmmoCount();
				gSoundMgr->PlaySound (RESOURCESOUND, this);															// Generic Resource Pickup Sound
				weapon[9]->AddAmmoCount (number);
				return TRUE;																														//	return TRUE to indicate that the resource was collected
			}
			break;
		case RESOURCE_GENERIC:
			{
				gSoundMgr->PlaySound (RESOURCESOUND, this);															// Generic Resource Pickup Sound
				resources += number;
				if (resources < 0) resources = 0;
				Real maxlife = Real(maxlevel) + R(1.0);
				life += Real(number) * R(0.01);
				if (life > maxlife) life = maxlife;
				return TRUE;	
		  }
			break;	

		case RESOURCE_AMMO_EXCHANGE:
		{
			Weapon *weap = GetCurrentWeapon ();
			if (weap)
			{
				int cost = ammocost[curWeapon] * number;
				if (cost <= resources)
				{
					int freespace = maxammo[curWeapon] - weap->AmmoCount ();
					if (freespace >= number)
					{
						resources -= cost;
						weap->AddAmmoCount (number);
						return TRUE;
					}
				}
			}
		}	break;

		case RESOURCE_HEALTH_EXCHANGE:
			if (resources >= number)
				if (AddResource (RESOURCE_HEALTH, int (number / ParmReal (RES_HEALTH_COST))))
				{
					resources -= number;
					return TRUE;
				}
			break;

		case RESOURCE_DEDUCTION:																										//	use this to subtract generic resources
			if (resources >= number)																									//	returns true if transaction successful
			{
				resources -= number;
				return TRUE;
			}
			break;
	}
	return FALSE;																																	//	return FALSE to indicate that the resource wa snot collected
}																																								//	end

//------------------------------------------------------------------------------
void	CrixaShip::DropStuff (void)																								//	drop everything we are carrying
{																																								//	begin
	static	int	types[10] = {
				0,
				0,
				RESOURCE_FLECHETTE_GUN,
				RESOURCE_LASER_GUN,
				RESOURCE_PLASMA_GUN,
				RESOURCE_BUSTER_GUN,
				RESOURCE_RUPTOR_GUN,
				RESOURCE_ROCKET_GUN,
				RESOURCE_TACMINE_GUN,
				RESOURCE_MINE_GUN
				};
	for (int i = 2; i < 10; i++)
		if (weapon[i])
		{
			while (weapon[i]->AmmoCount () > 250)
			{
				DropResource (types[i], 250);
				weapon[i]->AddAmmoCount (-250);
			}
			if (weapon[i]->AmmoCount () > 0)
			{
				DropResource (types[i], weapon[i]->AmmoCount ());
				weapon[i]->AddAmmoCount (-weapon[i]->AmmoCount ());
			}
		}
	WeaponShip::DropStuff ();
}																																								//	end

//------------------------------------------------------------------------------
void		CrixaShip::AddPower(Real addpow) 																				//	ds: add some amount of power (up to maxlevel * 20)
{																																								//	begin
	Real	maxp = R(1.0) + (Real) maxlevel;																				//	the maximum power is 1.0 + level of ship
	Real	newp = power + addpow;
	power = (newp > maxp) ? maxp : newp;																					//	clamp the added power to maxpower
}																																								//	end


//------------------------------------------------------------------------------
Bool		CrixaShip::UpgradeLevel(int newlevel) 																	//	ds: not as obvious as it seems...
{																																								//	begin
	int		increment = (newlevel > maxlevel);																			//	upgrade by 1 level only, only if the upgradelevel is higher
	maxlevel += increment;																												//	prevents people from getting SUPER UPGRADE right off
	return increment;																															//	return TRUE if increment happened
}																																								//	end

//------------------------------------------------------------------------------
