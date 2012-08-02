//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	and:						Bretton Wade
//	Last Revision:	08/01/96
//
//------------------------------------------------------------------------------

#ifndef	BUMPER
#define BUMPER

//------------------------------------------------------------------------------
#include "piece ptr.h"
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
class Bumper : public Piece																											//	a physical bumper that toggles an object
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Bool			bumped;
				Bool			damaged;
				PiecePtr	bumpPiece;
	public:																																				//	members available externally
				Bumper (Player *own, CPoint loc, Real width, Real height, SArrayPtr spr);//	constructor (continued)
				~Bumper (void);																													//	destructor
virtual	int				PostCollide (const PiecePtr &piece, Sect *sect);							//	mleh.
virtual	void			Damage (const PiecePtr &piece, Real damage);									//	damage the piece
virtual Real			TraceRay (CPoint origin, CVector direction);									//	return the time at which a ray intersects this piece
virtual Bool			Bumped (void);
virtual void			UnBump (void);
				PiecePtr	BumpedBy (void);
};																																							//	end

//------------------------------------------------------------------------------
inline	PiecePtr	Bumper::BumpedBy (void)
{																																								//	begin
	return bumpPiece;
}																																								//	end

//------------------------------------------------------------------------------

#endif // POWER_BUMPER