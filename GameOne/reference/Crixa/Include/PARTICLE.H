//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/02/96
//
//------------------------------------------------------------------------------

#ifndef	PARTICLE
#define	PARTICLE

//------------------------------------------------------------------------------
#include "piece.h"
#include "pixel.h"

//------------------------------------------------------------------------------
class Particle : public Piece																										//	a particle
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Real			damage;																												//	how much damage this bullet does
	public:																																				//	members available externally
				Particle (CPoint loc, Pixel color, Real dam, Real life, Real mult = R(1.0e2));			//	constructor
virtual	~Particle (void);																												//	destructor
virtual	Bool			Step (Real time);																							//	step the piece ahead in time
virtual	int				PreCollide (const PiecePtr &piece, Sect *sect);								//	do some work before a collision with another piece
virtual	int				PostCollide (const PiecePtr &piece, Sect *sect);							//	do some work after a collision with another piece
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	PARTICLE
