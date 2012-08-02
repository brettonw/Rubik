//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	07/21/96
//
//------------------------------------------------------------------------------

#ifndef	WORLD
#define	WORLD

//------------------------------------------------------------------------------
#include "piece ptr.h"

//------------------------------------------------------------------------------
class World : public Piece																											//	a piece representing the world map
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
				World (void);																														//	constructor
virtual	~World (void);																													//	destructor
virtual	Real			TraceRay (CPoint origin, CVector direction);									//	return the time at which a ray intersects this piece
virtual	Bool			Contains (CPoint location);																		//	return whether or not the location is contained by the piece
};																																							//	end

//------------------------------------------------------------------------------
extern	PiecePtr	gWorld;																												//	global world pointer

//------------------------------------------------------------------------------

#endif	//	WORLD
