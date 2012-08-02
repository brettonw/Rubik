//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/07/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "explosion sequence.h"
#include "referee.h"
#include "world.h"

//------------------------------------------------------------------------------
ExplSeq::ExplSeq (CPoint pt, CVector vel, Real time, Real rad, int pop)					//	constructor
{																																								//	begin
	location = pt;
	velocity = vel;
	timeLeft = time;
	radius = rad;
	pops = pop;
}																																								//	end

//------------------------------------------------------------------------------
ExplSeq::~ExplSeq (void)																												//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Bool	ExplSeq::Step (CPoint where)																							//	draw the next frame
{																																								//	begin
	Real	interval = gReferee->Interval () / Real (pops);
	for (int i = 0; i < pops; i++)
	{
		timeLeft -= interval;
		Point_2d	newLocation = location + (velocity * interval);
		PiecePtr	piece;
		if (!gReferee->Contains (newLocation, piece))
			location = newLocation;
		Pop ();
	}
	return (timeLeft > ZERO) ? TRUE : FALSE;
}																																								//	end

//------------------------------------------------------------------------------
