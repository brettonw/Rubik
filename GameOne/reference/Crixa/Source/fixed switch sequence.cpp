//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	02/28/97	JB		creation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "fixed switch sequence.h"

//------------------------------------------------------------------------------
FixedSwitchSeq::FixedSwitchSeq (CPoint where, SeqPtr seq1, SeqPtr seq2) :				//	constructor
SwitchSeq (where, seq1, seq2)
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
FixedSwitchSeq::~FixedSwitchSeq (void)																					//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Bool	FixedSwitchSeq::Step (CPoint where, int mode)															//	step along in the sequence
{																																								//	begin
	if (current != kSwitchOff)
		if (!sequences[current]->Step (where, mode))																//	if we've stepped to the end of the sequence
			if (current)																															//	if we're not at the default sequence
				Switch ();																															//	switch to the default sequence
	return TRUE;																																	//	return success
}																																								//	end

//------------------------------------------------------------------------------
