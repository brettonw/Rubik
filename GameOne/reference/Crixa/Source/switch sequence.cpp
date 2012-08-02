//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Last Revision:	07/23/96
//	Revisions:			02/19/97:	allowed the switch sequence to be turned off.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "switch sequence.h"

//------------------------------------------------------------------------------
SwitchSeq::SwitchSeq 	(CPoint where, SeqPtr seq1, SeqPtr seq2)									//	constructor
{																																								//	begin
	location = where;																															//	set the location
	count = 2;																																		//	there are two sequences
	sequences = new SeqPtr [count];																								//	create the array of sequences
	sequences[0] = seq1;																													//	set the first sequence
	sequences[1] = seq2;																													//	set the second sequence
	current = 0;																																	//	start at the beginning sequence
}																																								//	end

//------------------------------------------------------------------------------
SwitchSeq::SwitchSeq	(CPoint where, int num, ...)															//	default constructor for a switchable sequence
{																																								//	begin
	SeqPtr 	next_seq;																															//	used for grabbing the sequences from the var arg list
	va_list	ap;																																		//	variable argument list (of sequence pointers)
	location = where;																															//	set the location
	count = num;																																	//	save the number of sequences in the list
	sequences = new SeqPtr [num];																									//	create an array of sequence pointers
	va_start (ap, num);																														//	start the variable argument list
	for (int i = 0; i < num; i++)																									//	for the number of arguments
		sequences[i] = SeqPtr (va_arg (ap, Sequence *));														//	load the arguments into the array
	va_end (ap);																																	//	enough with the variable arguments
	current = 0;																																	//	0 will be the starting/default position
}																																								//	end

//------------------------------------------------------------------------------
SwitchSeq::~SwitchSeq (void)																										//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Bool	SwitchSeq::Step (CPoint where)																						//	step along in the sequence
{																																								//	begin
	if (current != kSwitchOff)
		if (!sequences[current]->Step (location - where))															//	if we've stepped to the end of the sequence
		{
			if (current)																																//	if we're not at the default sequence
				Switch ();																																//	switch to the default sequence
		}
	return TRUE;																																	//	return success
}																																								//	end

//------------------------------------------------------------------------------
void	SwitchSeq::Switch (int which, Bool reset)																	//	switch to a specified sequence
{																																								//	begin
	if (reset)																																		//	if the reset flag is set
		Reset ();																																		//	reset the current sequence
	current = which;																															//	set the current sequence to the specified sequence
}																																								//	end

//------------------------------------------------------------------------------
void	SwitchSeq::Reset (void)																										//	reset the sequence
{																																								//	begin
	if (current != kSwitchOff)
		sequences[current]->Reset ();																									//	reset the current sequence
}																																								//	end

//------------------------------------------------------------------------------
