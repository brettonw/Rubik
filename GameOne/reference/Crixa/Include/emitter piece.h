//------------------------------------------------------------------------------
//	Copyright ©1997 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Revisions:			02/14/97:	creation (JB)
//
//------------------------------------------------------------------------------

#ifndef EMITTER_PIECE
#define EMITTER_PIECE

//------------------------------------------------------------------------------
#include "piece.h"

//------------------------------------------------------------------------------
class EmitterPiece : public Piece																								//	a type of piece expressly used for invisble emitters
{																																								//	begin
	private:																																			//	private members
	protected:																																		//	protected members
	public:																																				//	public members
				EmitterPiece (Point_2d loc);																						//	constructor
virtual	~EmitterPiece (void);																										//	destructor
};																																							//	end

//------------------------------------------------------------------------------

#endif // EMITTER_PIECE
