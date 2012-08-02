//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/06/97
//
//	02/24/97	DS		Added AltDrawMode
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "stationary sequence.h"

//------------------------------------------------------------------------------
StatSeq::StatSeq (CPoint where, SeqPtr seq)																			//	constructor
{																																								//	begin
	location = where;																															//	save the location of the sprite
	sequence = seq;																																//	copy the sequence pointer
	tag = 0;																																			//	clear the tag
}																																								//	end

//------------------------------------------------------------------------------
StatSeq::~StatSeq (void)																												//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Bool	StatSeq::Step (CPoint where)																							//	draw the next frame
{																																								//	begin
	return sequence->Step (location - where);																			//	pass on the event to the sequence
}																																								//	end

//------------------------------------------------------------------------------
Bool	StatSeq::Step (CPoint where, int mode)																		//	draw the next frame
{																																								//	begin
	return sequence->Step (location - where, mode);																//	pass on the event to the sequence
}																																								//	end

//------------------------------------------------------------------------------
int		StatSeq::Width (void)																											//	return the width of the sequence
{																																								//	begin
	return sequence->Width ();																										//	return the width
}																																								//	end

//------------------------------------------------------------------------------
int		StatSeq::Height (void)																										//	return the height of the sequence
{																																								//	begin
	return sequence->Height ();																										//	return the height
}																																								//	end

//------------------------------------------------------------------------------
