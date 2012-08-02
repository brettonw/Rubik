//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	02/04/97
//
//	02/11/97	MK		Added thrusting soundcalls & logic to DoCrixa()
//	02/17/97	MK		Added parameter for number of crixa ship lives
//	02/27/97	DS		Made Play() return ship status like it should, put in
//									end of level trigger for death.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "local human.h"
#include "referee.h"
#include "crixa ship.h"
#include "round.h"
#include "korves ship.h"
#include "sprite array mgr.h"
#include "floor trigger.h"
#include "get parameters.h"
#include "sound mgr.h"
#include "rotating.h"
#include "target lock.h"
#include "reporter.h"
#include "area.h"
#include "random.h"
#include "direction.h"
#include "level file.h"
#include "text buffer.h"
#include "math utility.h"
#include "spring.h"

#include "traik.h"
#include "view.h"
#include "stationary sequence.h"
#include "time sequence.h"
#include "tracking sequence.h"
#include "name sequence.h"
#include "network play.h"
#include "lightning sequence.h"
#include "trigger array.h"

// Definition section for sounds (MK)
#define THRUSTSTART 48
#define THRUSTLOOP  50
#define THRUSTSTOP  51

#define GUNEMPTY    54

//------------------------------------------------------------------------------
Bool	Human::NewCrixa (CPoint where, Real rLoc)																	//	allocate a new crixa ship
{																																								//	begin
	if (lives > 0)
	{
		lives--;
		crixa = new CrixaShip (	this, 
													ParmReal (CRIXA_RADIUS), 
													where, 
													rLoc, 
													gSpriteMgr.Fetch (ParmString (CRIXA_SPRITE)));				//	create the crixa ship
		gView->Install (new StatSeq (loc, new TimeSeq (gSpriteMgr.Fetch ("etel"))));
		return TRUE;
	}
	return FALSE;
}																																								//	end

//------------------------------------------------------------------------------
void	Human::GetLock (void)																											//	get a lock
{																																								//	begin
	Point_2d	loc = crixa->GetBody ()->TLocation ();
	CVector		norm = ((Rotating *) crixa->GetVisual ())->GetDirection ()[0];
	Real			limit = Real (SCRN_HEIGHT >> 1),
						best = ZERO;
	LockPtr		oldLock = lock;
	lock = LockPtr (0);
	Reporter	reporter (gReferee, FULL_INTERSECT);
	PiecePtr	piece = reporter.GetPiece ();	
	while (piece ())																															//	loop over all of the pieces
	{																																							//	begin
		if ((piece->GetOwner () != crixa->GetOwner ()) && (piece->GetGeometry ()->Type () == CIRCLE_GEOM))
		{
			Body			*body = piece->GetBody ();
			if (body)
			{
				Vector_2d	delta = body->TLocation () - loc;
				if ((FABS (delta[X]) < limit) && (FABS (delta[Y]) < limit))
				{
					delta.Normalize ();
					Real	dot = delta | norm,
								dot2 = dot * dot;
					if ((dot > ZERO) && (dot2 > best))
					{
						best = dot2;
						lock = new TLock (piece, dot2);
					}
				}
			}
		}
		reporter.Next ();																														//	advance to the next piece
		piece = reporter.GetPiece ();																								//	grab a pointer to the piece
	}																																							//	end
	if (!lock ())
		lock = oldLock;
}																																								//	end

//------------------------------------------------------------------------------
void	Human::ComputeCamera (void)																								//	compute the camera location for the crixa ship
{																																								//	begin
	if ((shakeFrame) && (++shakeFrame > ParmInteger (SHAKE_FRAMES)))
		shakeFrame = 0;

	if (crixa->GetLife () < lastLife)
	{
		Real	deltaLife = lastLife - crixa->GetLife (),
					distance = deltaLife * R(2.0e2),
					maxDistance = ParmReal (SHAKE_DISTANCE);
		if (distance > maxDistance)
			distance = maxDistance;
		shakeVector = gDirection[gRandom.RndInteger ()][0] * distance;
		shakeFrame = 1;
	}
	lastLife = crixa->GetLife ();
	Body			*body = crixa->GetBody ();
	Real			weight = ParmReal (CAMERA_DRIFT) * ParmReal (NEW_WEIGHT);
	Vector_2d	offset = body->TVelocity () * weight;
	camera = (camera * ParmReal (OLD_WEIGHT)) + offset;
}																																								//	end

//------------------------------------------------------------------------------
Human::Human (int ident) : Player ("\0")																				//	constructor
{																																								//	begin
	id = ident;
	loc (ZERO, ZERO);
	rloc = ZERO;
	lives = ParmInteger (CRIXA_LIVES);											
	name[0] = 0;
	stillPlaying = TRUE;
}																																								//	end

//------------------------------------------------------------------------------
Human::Human (cstr nam, int ident) : Player (nam)																//	constructor
{																																								//	begin
	id = ident;
	loc (ZERO, ZERO);
	rloc = ZERO;
	lives = ParmInteger (CRIXA_LIVES);																						// (MK) New Parameterized lives 
	stillPlaying = TRUE;
}																																								//	end

//------------------------------------------------------------------------------
Human::~Human (void)																														//	destructor
{																																								//	begin
	if (gReferee->Confirm (crixa))																								//	if the crixa ship is a valid piece
		gReferee->Remove (crixa);																										//	remove it
}																																								//	end

//------------------------------------------------------------------------------
Bool	Human::Play (void)																												//	play one frame
{																																								//	begin
	Code	*code = inputs.Dequeue ();																							//	get a control code off the queue
	if (stillPlaying)
		DoCrixa (code);	
	return TRUE;																																	//	always return true (network related? DS)
}																																								//	end

//------------------------------------------------------------------------------
Bool	Human::DoCrixa (Code *code)																								//	play the crixa ship
{																																								//	begin
	if (gReferee->Confirm (crixa))																								//	if the crixa ship is a valid piece
	{																																							//	begin
		Body	*body = ((Ship *) crixa ())->GetBody ();
		body->ApplyForce (body->TVelocity () * ParmReal (CRIXA_FRICTION));
		if (code)
		{
			Real	thrust = code->Thrust (),																						//	get the thrust value from the code
						strafe = code->Strafe (),																						//	get the strafe value from the code
						rotate = code->Rotate ();																						//	get the rotate value from the code
			
																																								// (MK)
			if ((thrust || strafe) && !(thrusting)) {																	//  if starting to thrust or strafe
					thrusting = TRUE;																											//  mark flag indicating we are thrusting
					if ((thrust > .99999))
						gSoundMgr->PlaySound(THRUSTSTART,gLocal->GetCrixa(), FALSE);				//  trigger the thrust "start" sound
					thrustsoundID = gSoundMgr->InsertEMSound(THRUSTLOOP,crixa);						//  insert the thrusting sound emitter
			}
			else if (thrusting && ((thrust == 0) && (strafe == 0))){									//  otherwise, check all thrust is stopped
					thrusting = FALSE;																										//  reset thrusting flag to not thrusting
				  gSoundMgr->PlaySound(THRUSTSTOP, crixa, FALSE);												//  play the "Stop," trailing thrust sound
					gSoundMgr->RemoveEMSound(THRUSTLOOP, thrustsoundID);									//  remove the emitter 
			}		
			else if (thrusting && ((strafe > 1.999) || (strafe < -1.999)))
					gSoundMgr->PlaySound(THRUSTSTART, gLocal->GetCrixa(), FALSE);// (MK)
	


			((Ship *) crixa ())->Thrust (thrust);																			//	thrust the ship
			((Ship *) crixa ())->Strafe (strafe);																			//	strafe the ship
			((Ship *) crixa ())->Rotate (rotate);																			//	rotate the ship
			switch (code->lock)
			{
				case LOCK_POSITION:
					lock = new Lock (body->TLocation ());
					break;
				case LOCK_OBJECT:
					GetLock ();
					break;
				case LOCK_CLEAR:
					lock = LockPtr (0);
					break;
				default:
					if (lock () && (lock->Confirm () == FALSE))
						lock = LockPtr (0);
					break;
			}
			if (code->weapon < 10)
				((WeaponShip *) crixa ())->SelectWeapon (code->weapon);									//	select the coded weapon
			if (code->fire)																														//	if the fire code is set
			{																																					//	begin
				Weapon	*weapon = ((WeaponShip *) crixa ())->GetCurrentWeapon ();
				weapon->SetLock (lock);
				if (!((WeaponShip *) crixa ())->Fire ())																//	fire the ship's weapon
				{																																				//  if weapon didn't fire
								gSoundMgr->PlaySound(GUNEMPTY, crixa, FALSE);									  //  if so, play empty gun sound
				}
			}																																					//	end
		}
		gReferee->Update (crixa);																										//	update the ship's collision records
		gFloor->Trigger (crixa);																										//	check the floor for triggers
		ComputeCamera ();
	}																																							//	end
	else // da ship was kilt
	{
		if (thrusting)
				gSoundMgr->RemoveEMSound(THRUSTLOOP, thrustsoundID);
		if (code)																																		//	otherwise, time to create a new piece
		{
			if (NewCrixa (loc, rloc))
			{																																					//	create a new crixa ship
				gReferee->Install (crixa);																							//	add the ship to the world
				//	punch in some running lights
				TimeSeq		*seq = new TimeSeq (gSpriteMgr.Fetch ("redfl"), TRUE, 3, 0, ZERO, 1, R(0.75), 1, R(0.25));
				TrackSeq	*track = new TrackSeq (seq, crixa, Vector_2d (R(15.0), R(-8.0)));
				gView->Install (track, PIECE_LAYER);
				seq = new TimeSeq (gSpriteMgr.Fetch ("greenfl"), TRUE, 3, 1, R(0.25), 0, ZERO, 1, R(0.75));
				track = new TrackSeq (seq, crixa, Vector_2d (R(-15.0), R(-8.0)));
				gView->Install (track, PIECE_LAYER);
				if (gClient)
				{
					//	punch in the name sequence
					NameSeq	*tseq = new NameSeq (this, gSpriteMgr.Fetch ("text7bg"));
					track = new TrackSeq (tseq, crixa, Vector_2d (ZERO, ZERO), Vector_2d (R(-15.0), R(30.0)));
					gView->Install (track, HUD_LAYER_2);
				}

				shakeFrame = 0;
				lastLife = R(1.0);
				lock = LockPtr (0);
			}
			else	// unable to make a new ship! Interface handled in Local
			{
				gTrigger->Trigger(1, PiecePtr(gLocal->GetCrixa()),DEV_END_LEVEL,0);			// we can't make new ships, so end the level
				return FALSE;																														// and report that we're not playing
			}

		}
	}
	return TRUE;																																	//	always return true (ds: not sure why)
}																																								//	end

//------------------------------------------------------------------------------
Vector_2d	Human::Camera (void)																									//	return the location of the player's camera
{																																								//	begin
	Vector_2d	cam = crixa->GetBody ()->TLocation ();
	if (gReferee->Confirm (crixa))																								//	if the crixa ship is a valid piece
	{
		if (shakeFrame)																															//	if the crixa ship is being shaken
		{
			Real	mul = ((shakeFrame & 0x01) ? R(1.0) : R(-1.0)) / Real (shakeFrame);
			cam += (shakeVector * mul);
		}
	}
	else
		camera *= ParmReal (OLD_WEIGHT);
	return cam + camera;
}																																								//	end

//------------------------------------------------------------------------------
void	Human::Reset (void)																												//	reset the local human player
{																																								//	begin
	LevFile		lev ("Crixa");
	TxtBuffer	buffer (&lev);
	cstr	crx;
	Real	x, y;
	for (int i = 0; i <= id; i++)
		buffer >> crx >> rloc >> x >> y;
	rloc = DegreesToRadians (rloc);																								//	convert the orientation from degrees to radians
	loc (x, y);																																		//	build the location form the components

	if (crixa ())
	{
		Body	*body = crixa->GetBody ();
		body->SetLocation (loc, rloc);
		body->AddVelocity (-body->TVelocity (), -body->RVelocity ());
	}
	else
		
		NewCrixa (loc, rloc);																												//	create a new crixa ship
	
	gReferee->Install (crixa);																										//	add the ship to the world
	//	punch in some running lights
	TimeSeq		*seq = new TimeSeq (gSpriteMgr.Fetch ("greenfl"), TRUE, 3, 0, ZERO, 1, R(0.75), 1, R(0.25));
	TrackSeq	*track = new TrackSeq (seq, crixa, Vector_2d (R(15.0), R(-8.0)));
	gView->Install (track, PIECE_LAYER);
	seq = new TimeSeq (gSpriteMgr.Fetch ("redfl"), TRUE, 3, 1, R(0.25), 0, ZERO, 1, R(0.75));
	track = new TrackSeq (seq, crixa, Vector_2d (R(-15.0), R(-8.0)));
	gView->Install (track, PIECE_LAYER);
	//	punch in the name sequence
	if (gClient)
	{
		NameSeq	*tseq = new NameSeq (this, gSpriteMgr.Fetch ("text7bg"));
		track = new TrackSeq (tseq, crixa, Vector_2d (ZERO, ZERO), Vector_2d (R(-15.0), R(30.0)));
		gView->Install (track, HUD_LAYER_2);
	}
	/*
	//	temporary lightning sequence
	LightSeq	*lseq = new LightSeq (Point_2d (R(960.0), R(485.0)), Vector_2d (ZERO, R(1.0)), 300);
	gView->Install (lseq);
	lseq = new LightSeq (Point_2d (R(960.0), R(985.0)), Vector_2d (ZERO, R(-1.0)), 300);
	gView->Install (lseq);
	*/

	camera (ZERO, ZERO);
	shakeFrame = 0;
	lastLife = crixa->GetLife ();
	lock = LockPtr (0);
}																																								//	end

//------------------------------------------------------------------------------
void	Human::Quit (void)																												//	have this player stop playing
{																																								//	begin
	stillPlaying = FALSE;
	if (gReferee->Confirm (crixa))																								//	if the crixa ship is a valid piece
		gReferee->Remove (crixa);																										//	remove it
}																																								//	end

//------------------------------------------------------------------------------
bool	Human::AddLife (void)																											//	return lives
{																																								//	begin
	if (lives++ < ParmInteger(CRIXA_MAX_LIVES))																		//	maxlife check (arbitrarily set to 1000)
		return true;																																//	return the number of lives
	lives = ParmInteger(CRIXA_MAX_LIVES);																					//	set the maximum
	return FALSE;																																	//	report failure
}																																								//	end
//------------------------------------------------------------------------------
