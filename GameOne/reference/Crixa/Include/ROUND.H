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
class Round : public Piece																											//	a disc piece
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
				Round (Real radius, CPoint tLoc, Real rLoc, SArrayPtr sprites);					//	constructor
virtual	~Round (void);																													//	destructor
virtual	Real			TraceRay (CPoint origin, CVector direction);									//	return the time at which a ray intersects this piece
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	ROUND
