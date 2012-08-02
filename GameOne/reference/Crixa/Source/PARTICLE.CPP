//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	05/29/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "particle.h"
#include "circle.h"
#include "dot.h"
#include "pi.h"
#include "random.h"
#include "piece ptr.h"

//------------------------------------------------------------------------------
Particle::Particle (CPoint loc, Pixel color, Real dam, Real lif, Real mult) : Piece (0)		//	constructor
{																																								//	begin
	Real	mass = PI * mult;																										//	compute the mass as the area of the plate of radius 1 times one hundred
	body = new Body (loc, ZERO, mass, mass);																			//	allocate the body
	visual = new Dot (body, color);																								//	allocate the visual
	geometry = new Circle (R(1.0));																								//	allocate the geometry, a circle of unit radius
	damage = dam;																																	//	save the damage potential of this bullet
	life = lif;																																		//	copy the lifetime
}																																								//	end

//------------------------------------------------------------------------------
Particle::~Particle (void)																											//	destructor
{																																								//	begin
	delete geometry;																															//	release the geometry
	delete visual;																																//	release the visual
	delete body;																																	//	release the body
}																																								//	end

//------------------------------------------------------------------------------
Bool	Particle::Step (Real time)																								//	step the piece ahead in time
{																																								//	begin
	life -= time;																																	//	subtract the time from the life value
	if (life < ZERO)																															//	if the life is negative
		life = ZERO;																																//	set life to zero
	return Piece::Step (time);																										//	do a noremal step
}																																								//	end

//------------------------------------------------------------------------------
int		Particle::PreCollide (const PiecePtr &piece, Sect *sect)									//	do some work before a collision with another piece
{																																								//	begin
	piece->Damage (PiecePtr (0), damage);																					//	damage the piece we just hit
	return PIECE_ALIVE | PIECE_UPDATE;																						//	return alive, so we get the impact effect
}																																								//	end

//------------------------------------------------------------------------------
int		Particle::PostCollide (const PiecePtr &piece, Sect *sect)									//	do some work after a collision with another piece
{																																								//	begin
	return PIECE_DEAD;																														//	the particle goes away now, so return dead
}																																								//	end

//------------------------------------------------------------------------------
