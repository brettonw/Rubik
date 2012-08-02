//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:		Bretton Wade
//	Last Revision:	07/17/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "plasma bubble.h"
#include "time sequence.h"
#include "referee.h"

//------------------------------------------------------------------------------
PBubble::PBubble (SArrayPtr spr, CPoint pt, CVector vel, Real dam, PiecePtr own,//	constructor
									Real speed) :																									//	constructor (continued)
MoveSeq (new TimeSeq (spr), pt, FALSE, 3, 																			//	constructor (continued)
					Vector_2d (ZERO, ZERO), gReferee->Interval (), 												//	constructor (continued)
					vel, speed, 																													//	constructor (continued)
					Vector_2d (ZERO, ZERO))																								//	constructor (continued)
{																																								//	begin
	damage = dam;																																	//	copy the damage
	owner = own;																																	//	copy the owner
}																																								//	end

//------------------------------------------------------------------------------
PBubble::~PBubble (void)																												//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Bool	PBubble::Step (CPoint where)																							//	move it move it
{																																								//	begin
	PiecePtr	victim;																															//	place to get the victim pointer
	if (gReferee->Contains (location, victim))																		//	find who is inside the plasma bubble
		if (victim () != owner ())																									//	if the victim is not the shooter
			victim->Damage (owner, damage);																						//	damage them
	return MoveSeq::Step (where);																									//	do the parental thing
}																																								//	end

//------------------------------------------------------------------------------
