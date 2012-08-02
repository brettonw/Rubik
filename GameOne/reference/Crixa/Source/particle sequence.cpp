//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:		Bretton Wade
//	Last Revision:	07/18/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "particle sequence.h"
#include "moving sequence.h"
#include "fade pixel sequence.h"
#include "referee.h"

//------------------------------------------------------------------------------
PartSeq::PartSeq (CPoint pt, CVector vel, Pixel color, Real time, int cnt)			//	constructor
{																																								//	begin
	count = cnt;																																	//	copy the count
	Vector_2d	zeroVec (ZERO, ZERO);
	dots = new SeqPtr[count];
	for (int i = 0; i < count; i++)
		dots[i] = new MoveSeq (new FadeSeq (color, time), pt, FALSE, 3, 
							zeroVec, gReferee->Interval () * (i + 1), vel, time, zeroVec);
}																																								//	end

//------------------------------------------------------------------------------
PartSeq::~PartSeq (void)																												//	destructor
{																																								//	begin
	delete[] dots;
}																																								//	end

//------------------------------------------------------------------------------
Bool	PartSeq::Step (CPoint where)																							//	move it move it
{																																								//	begin
	for (int i = count - 1; i > 0; i--)
		dots[i]->Step (where);
	return dots[0]->Step (where);
}																																								//	end

//------------------------------------------------------------------------------
