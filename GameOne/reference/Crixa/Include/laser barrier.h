//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Last Revision:	08/13/96
//
//	Modified by:    Mark E. Kern
//                  02/13/97        Added a new Contains fucntion to fix bounding check bug
//																	which checks locations against a radius as large as the
//                                  the barrier is wide, instead of the actual rectangular area
//------------------------------------------------------------------------------

#ifndef LASER_BARRIER
#define LASER_BARRIER

//------------------------------------------------------------------------------
#include "piece.h"

//------------------------------------------------------------------------------
class LaserBarrier : public Piece
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Real damage;
	public:																																				//	members available externally
				LaserBarrier (Player *own, CPoint loc, Real w, Real h, Real dam);
virtual int		PostCollide (const PiecePtr &piece, Sect *sect);
virtual Real	TraceRay (CPoint or, CVector direction);
virtual Bool	Contains(CPoint location);																				//  Test override
};																																							//	end

//------------------------------------------------------------------------------

#endif // LASER_BARRIER