//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/29/97
//
//------------------------------------------------------------------------------

#ifndef	PIECE
#define	PIECE

//------------------------------------------------------------------------------
#include "visual.h"
#include "geometry.h"
#include "sect.h"
#include "player.h"

//------------------------------------------------------------------------------
enum	{PIECE_DEAD = 0, PIECE_NO_UPDATE = 0, PIECE_ALIVE = 1, PIECE_UPDATE = 2};	//	flags for pre and post collide return values

//------------------------------------------------------------------------------
class Piece																																			//	a playing piece, an object in the world
{																																								//	begin
	friend	class	PiecePtr;																												//	the piece pointer class can play with my private parts
	private:																																			//	members internal to this class only
  			int				refcount;																											//	reference count for the sequence
				int				id;																														//	the piece id
static	int				idCounter;																										//	the piece id assignment counter
	protected:																																		//	members internal to this class hierarchy
				Visual		*visual;																											//	pointer to the visual representation of the piece
				Body			*body;																												//	pointer to a body for the piece
				Geometry	*geometry;																										//	pointer to the geometry description of the piece
				Real			life;																													//	life remaining to the piece
				Real			armor;																												//	multiplier for damage
				Real			hardness;																											//	how hard the object is
				Player		*owner;																												//	pointer to the owner of this piece
				int				value;																												//	the score value of the ship
	public:																																				//	members available externally
				Piece (Player *owner);																									//	constructor
				Piece (Player *owner, Visual *visual, Body *body, Geometry *geometry);	//	constructor
virtual	~Piece (void);																													//	destructor
virtual	void			Draw (CPoint where);																					//	draw the piece
virtual	Bool			Step (Real time);																							//	step the piece ahead in time
virtual	int				PreCollide (const PiecePtr &piece, Sect *sect);								//	do some work before a collision with another piece
virtual	int				PostCollide (const PiecePtr &piece, Sect *sect);							//	do some work after a collision with another piece
virtual	void			Damage (const PiecePtr &piece, Real damage);									//	damage the piece
virtual	Real			TraceRay (CPoint origin, CVector direction);									//	return the time at which a ray intersects this piece
virtual	Bool			Contains (CPoint location);																		//	return whether or not the location is contained by the piece
virtual	Bool			AddResource (int type, int number);														//	add a resource to the piece
virtual	void			Stun (Real time);																							//	stun the piece
				Visual		*GetVisual (void);																						//	return the visual
				Body			*GetBody (void);																							//	return the body
				Geometry	*GetGeometry (void);																					//	return the geometry
				Real			GetLife (void);																								//	return the life
				void			SetArmor (Real armor);																				//	set the armor value for the piece
				Real			GetHardness (void);																						//	return the piece hardness
				void			SetHardness (Real hardness);																	//	set the piece hardness
				Player		*GetOwner (void);																							//	return the owner of the piece
const		int				*GetId (void);																								//	return the id of the piece
				void			SetValue (int value);																					//	set the score value of the piece
};																																							//	end

//------------------------------------------------------------------------------
inline	Visual	*Piece::GetVisual (void)																				//	return the visual
{																																								//	begin
	return visual;																																//	return the pointer
}																																								//	end

//------------------------------------------------------------------------------
inline	Body	*Piece::GetBody (void)																						//	return the body
{																																								//	begin
	return body;																																	//	return the pointer
}																																								//	end

//------------------------------------------------------------------------------
inline	Geometry	*Piece::GetGeometry (void)																		//	return the geometry
{																																								//	begin
	return geometry;																															//	return the pointer
}																																								//	end

//------------------------------------------------------------------------------
inline	Real	Piece::GetLife (void)																							//	return the life
{																																								//	begin
	return life;																																	//	return the value
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Piece::SetArmor (Real ar)																					//	set the armor value for the piece
{																																								//	begin
	if (ar > ZERO)																																//	if the new value is greater than zero
		armor = R(1.0) / ar;																												//	set the armor to the reciprocal of the value
	else																																					//	otherwise
		armor = ZERO;																																//	make the piece invulnerable
}																																								//	end

//------------------------------------------------------------------------------
inline	Real	Piece::GetHardness (void)																					//	return the piece hardness
{																																								//	begin
	return hardness;																															//	return the hardness value
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Piece::SetHardness (Real hard)																		//	set the piece hardness
{																																								//	begin
	hardness = hard;																															//	set the hardness value
}																																								//	end

//------------------------------------------------------------------------------
inline	Player	*Piece::GetOwner (void)																					//	return the owner of the piece
{																																								//	begin
	return owner;																																	//	return the owning player
}																																								//	end

//------------------------------------------------------------------------------
inline	const int		*Piece::GetId (void)																				//	return the id of the piece
{																																								//	begin
	return &id;																																		//	return the id
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Piece::SetValue (int val)																					//	set the score value of the piece
{																																								//	begin
	value = val;																																	//	copy the new value
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	PIECE
