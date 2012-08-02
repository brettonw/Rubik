//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	02/03/97
//	03/12/97	DS		Modified Resources to print names
//	03/18/97	DS		Modified PostCollide to set life = ZERO on absorption
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "resource.h"
#include "sequence visual.h"
#include "fireball visual.h"
#include "sprite sequence.h"
#include "circle.h"
#include "pi.h"
#include "piece ptr.h"
#include "quadratic.h"

// stuff for labeled resource pickup
#include "view.h"
#include "text sequence.h"
#include "stationary sequence.h"
#include "moving sequence.h"
#include "sprite array mgr.h"

//------------------------------------------------------------------------------
Resource::Resource (Real rad, CPoint loc, Real rloc, SArrayPtr spr, int typ,		//	constructor
										int num, Real tm) :																					//	constructor (continued)
Piece (0)																																				//	constructor (continued)
{																																								//	begin
	Real	mass = PI * rad * rad;																									//	compute the mass as the area of the plate times one hundred
	body = new Body (loc, rloc, mass, INFINITY);																	//	allocate the body, non-rotating
	if (typ == RESOURCE_GENERIC)																									//	if this is a generic resource
		visual = new FireBall (body, spr);																					//	allocate the fireball visual
	else																																					//	otherwise
		visual = new SeqVisual (body, new SpriteSeq (spr, typ));										//	allocate the sequence visual
	geometry = new Circle (rad);																									//	allocate the geometry, a circle of specified radius
	type = typ;																																		//	copy the resource type
	number = num;																																	//	copy the resource number
	armor = ZERO;
	life = tm;																																		//	copy the lifetime of the resource
}																																								//	end

//------------------------------------------------------------------------------
Resource::~Resource (void)																											//	destructor
{																																								//	begin
	delete geometry;																															//	release the geometry
	delete visual;																																//	release the visual
	delete body;																																	//	release the body
}																																								//	end

//------------------------------------------------------------------------------
Bool	Resource::Step (Real time)																								//	step the piece ahead in time
{																																								//	begin
	life -= time;																																	//	subtract the time from the life value
	if (life < ZERO)																															//	if the life is negative
		life = ZERO;																																//	set life to zero
	return Piece::Step (time);																										//	do a noremal step
}																																								//	end

//------------------------------------------------------------------------------
int		Resource::PostCollide (const PiecePtr &piece, Sect *sect)									//	do some work after a collision with another piece
{																																								//	begin
	if (piece->AddResource (type, number))																				//	if the piece collects the resource
	{																																							//	begin
		Player	*powner = piece->GetOwner ();																				//	get the owner of the collecting piece
		if (powner)																																	//	if the owner is a valid player
		{
			powner->AddScore (0, 0, 0, 1, 0, value);																	//	add the score to the player
			PrintName (type);
		}
		life = ZERO;																																//	ds: change to allow that piece is no longer alive
		return PIECE_DEAD;																													//	the resource goes away now, so return dead
	}																																							//	end
	return Piece::PostCollide (piece, sect);																			//	return the parental behavior
}																																								//	end

//------------------------------------------------------------------------------
void	Resource::PrintName (int type)
{
	cstr desc;
	SArrayPtr font = gSpriteMgr.Fetch("scaps5bgs"),
						bold = gSpriteMgr.Fetch("scaps5bbs");
	int	layer = PIECE_LAYER, time = 30;
	SArrayPtr temp;

	switch (type)
	{
		case RESOURCE_HEALTH:
			desc = "Health";
			break;
		case RESOURCE_TRAIK_AMMO:
			desc = "trk ammo";
			break;
		case RESOURCE_FLECHETTE_GUN:
			desc = "Flechette";
			temp = font; font = bold; bold = temp; layer = HUD_LAYER_2; time = 90; 
			break;
		case RESOURCE_FLECHETTE_AMMO:
			desc = "flc ammo";
			break;
		case RESOURCE_LASER_GUN:
			desc = "Laser Gun";
			temp = font; font = bold; bold = temp; layer = HUD_LAYER_2; time = 90;
			break;
		case RESOURCE_LASER_AMMO:
			desc = "las ammo";
			break;
		case RESOURCE_PLASMA_GUN:
			desc = "Plasma Gun";
			temp = font; font = bold; bold = temp; layer = HUD_LAYER_2; time = 90;
			break;
		case RESOURCE_PLASMA_AMMO:
			desc = "pls ammo";
			break;
		case RESOURCE_BUSTER_GUN:
			desc = "Buster Gun";
			temp = font; font = bold; bold = temp; layer = HUD_LAYER_2; time = 90;
			break;
		case RESOURCE_BUSTER_AMMO:
			desc = "bus ammo";
			break;
		case RESOURCE_RUPTOR_GUN:
			desc = "Ruptor Gun";
			temp = font; font = bold; bold = temp; layer = HUD_LAYER_2; time = 90;
			break;
		case RESOURCE_RUPTOR_AMMO:
			desc = "rup ammo";
			break;
		case RESOURCE_ROCKET_GUN:
			desc = "Rockets";
			temp = font; font = bold; bold = temp; layer = HUD_LAYER_2; time = 90;
			break;
		case RESOURCE_ROCKET_AMMO:
			desc = "rkt pak";
			break;
		case RESOURCE_TACMINE_GUN:
			desc = "Torpedos";
			temp = font; font = bold; bold = temp; layer = HUD_LAYER_2; time = 90;
			break;
		case RESOURCE_TACMINE_AMMO:
			desc = "torp pak";
			break;
		case RESOURCE_MINE_GUN:
			desc = "Mines";
			temp = font; font = bold; bold = temp; layer = HUD_LAYER_2; time = 90;
			break;
		case RESOURCE_MINE_AMMO:
			desc = "mine pak";
			break;
		case RESOURCE_GENERIC:
			desc = "Resource";
			break;
		default:
			break;
	}
	Point_2d loc = body->TLocation();
	SeqPtr text = new TextSeq(font,bold,desc,128,time);
	loc[X]-=strlen(desc)/2;
	// pick one of the below!
	gView->Install(new MoveSeq(text,loc,TRUE,2,Vector_2d(ZERO,R(50.0)),R(0.3),Vector_2d(ZERO,R(-50.0)),R(0.3)), layer);
	//gView->Install(new StatSeq(loc,text));
}

//------------------------------------------------------------------------------
Real	Resource::TraceRay (CPoint origin, CVector direction)											//	return the time at which a ray intersects this piece
{																																								//	begin
	Real			radius = ((Circle *) geometry)->Radius (),													//	get the radius of the resource
						root1, root2;																												//	place to fetch the roots
	if (Dist (origin - body->TLocation (), direction, radius, root1, root2))			//	if the ray intersects the resource
		if (root1 > ZERO)																														//	if the ray intersects the resource at some later time
			return root1;																															//	return that time
	return INFINITY;																															//	return no intersection for all unhandled cases
}																																								//	end

//------------------------------------------------------------------------------
