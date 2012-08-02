//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/09/97
//
//------------------------------------------------------------------------------

#ifndef	ROUND
#define	ROUND

//------------------------------------------------------------------------------
#include "piece.h"
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
class Jenny : public Piece																											//	a disc piece
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Real			deadTime;
				Real			lastPop;
	public:																																				//	members available externally
				Jenny (Real radius, CPoint tLoc, Real rLoc, SArrayPtr sprites);					//	constructor
virtual	~Jenny (void);																													//	destructor
virtual	Bool			Step (Real time);																							//	step the piece ahead in time
virtual	int				PostCollide (const PiecePtr &piece, Sect *sect);							//	do some work after a collision with another piece
virtual	void			Damage (const PiecePtr &piece, Real damage);									//	damage the piece
virtual	Real			TraceRay (CPoint origin, CVector direction);									//	return the time at which a ray intersects this piece
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	ROUND
