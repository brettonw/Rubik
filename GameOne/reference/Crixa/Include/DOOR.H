//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/24/96
//
//------------------------------------------------------------------------------

#ifndef	DOOR
#define	DOOR

//------------------------------------------------------------------------------
#include "piece.h"
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
class Door : public Piece																												//	a door piece
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
				Door (Player *own, CPoint loc, Real width, Real height, SArrayPtr spr,	//	constructor
							CVector axis);																										//	constructor (continued)
virtual	~Door (void);																														//	destructor
virtual	Real			TraceRay (CPoint origin, CVector direction);									//	return the time at which a ray intersects this piece
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	DOOR
