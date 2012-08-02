//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/29/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "piece.h"
#include "get parameters.h"

//------------------------------------------------------------------------------
int		Piece::idCounter = 0;																											//	the piece id assignment counter

//------------------------------------------------------------------------------
Piece::Piece (Player *own)																											//	constructor
{																																								//	begin
	owner = own;																																	//	copy the owner
	life = R(1.0);																																//	set the life to one
	armor = R(1.0);																																//	set the armor to one
	hardness = ParmReal (DEFAULT_HARDNESS);																				//	set the default hardness
	refcount = 0;																																	//	initialize the reference count
	id = idCounter++;																															//	assign the piece id and increment the id counter
	value = 100;																																	//	default score value of 100 points
}																																								//	end

//------------------------------------------------------------------------------
Piece::Piece (Player *own, Visual *vis, Body *bod, Geometry *geom)							//	constructor
{																																								//	begin
	body = bod;																																		//	copy the body
	visual = vis;																																	//	copy the visual
	geometry = geom;																															//	copy the geometry
	life = R(1.0);																																//	set the life to one
	armor = R(1.0);																																//	set the armor to one
	hardness = ParmReal (DEFAULT_HARDNESS);																				//	set the default hardness
	refcount = 0;																																	//	initialize the reference count
	id = idCounter++;																															//	assign the piece id and increment the id counter
	value = 100;																																	//	default score value of 100 points
}																																								//	end

//------------------------------------------------------------------------------
Piece::~Piece (void)																														//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	Piece::Draw (CPoint where)																								//	draw the piece
{																																								//	begin
	if (life > ZERO)																															//	if the piece is alive
		visual->Draw (where);																												//	draw the visual
}																																								//	end

//------------------------------------------------------------------------------
Bool	Piece::Step (Real time)																										//	step the piece ahead in time
{																																								//	begin
	if (body)																																			//	if there is a body
		body->Step (time);																													//	step the body ahead in time
	return life > ZERO ? TRUE : FALSE;																						//	return true if the piece is still alive
}																																								//	end

//------------------------------------------------------------------------------
int		Piece::PreCollide (const PiecePtr &piece, Sect *sect)											//	do some work before a collision with another piece
{																																								//	begin
	return PIECE_ALIVE | PIECE_UPDATE;																						//	return flags to indicate object is alive and should be updated
}																																								//	end

//------------------------------------------------------------------------------
int		Piece::PostCollide (const PiecePtr &piece, Sect *sect)										//	do some work after a collision with another piece
{																																								//	begin
	if (life > ZERO)																															//	if the piece still has life
		return PIECE_ALIVE | PIECE_UPDATE;																					//	return flags to indicate object is alive and should be updated
	return PIECE_DEAD;																														//	return flags to indicate that the piece is dead
}																																								//	end

//------------------------------------------------------------------------------
void	Piece::Damage (const PiecePtr &piece, Real damage)												//	damage the piece
{																																								//	begin
	if (life > ZERO)																															//	if the object is alive
	{																																							//	begin
		life -= damage * armor;																											//	subtract the damage multiplied by the armor from the piece life
		if (life < ZERO)																														//	if the life has expired
			life = ZERO;																															//	zero the life
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
Real	Piece::TraceRay (CPoint origin, CVector direction)												//	return the time at which a ray intersects this piece
{																																								//	begin
	return INFINITY;																															//	no intersection unless specifically desired
}																																								//	end

//------------------------------------------------------------------------------
// Broken for polygonal pieces such as laser barriers. Returns TRUE even if
// point is located outside of the polygonal area. the geometry->Bound() call
// is the culprit, since it returns a huge radius of a circle,
// equal to the distance of the farthest vertex in the piece from its center.
// Quick fix for laser barriers....laserbarriers return a radius of 0 always 
// by overiding this virtual contains function in laser barrier.h & .cpp
//
Bool	Piece::Contains (CPoint location)																					//	return whether or not the location is contained by the piece
{																																								//	begin
	if (body)																																			
	{
		Real	bound = geometry->Bound ();
		bound *= bound;
		Vector_2d	delta = body->TLocation () - location;
		Real	dist = delta | delta;
		if (dist < bound)
			return TRUE;
	}
	return FALSE;
}																																								//	end

//------------------------------------------------------------------------------
Bool	Piece::AddResource (int type, int number)																	//	add a resource to the piece
{																																								//	begin
	return FALSE;																																	//	return FALSE to indicate that the resource is not collected
}																																								//	end

//------------------------------------------------------------------------------
void	Piece::Stun (Real time)																										//	stun the piece
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
