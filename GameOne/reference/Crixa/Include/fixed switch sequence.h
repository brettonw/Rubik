//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	02/28/97	JB		creation.
//
//------------------------------------------------------------------------------

#ifndef FIXED_SWITCH_SEQUENCE
#define FIXED_SWITCH_SEQUENCE

//------------------------------------------------------------------------------
#include "switch sequence.h"

//------------------------------------------------------------------------------
class FixedSwitchSeq : public SwitchSeq																					//	a fixed switchable sequence
{																																								//	begin
	private:																																			//	private members in the class
	protected:																																		//	protected members in the class
	public:																																				//	public members in the class
				FixedSwitchSeq (CPoint where, SeqPtr seq1, SeqPtr seq2);								//	constructor
virtual	~FixedSwitchSeq (void);																									//	destructor
virtual	Bool	Step (CPoint where, int mode);																		//	step through the current sequence
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	SWITCH_SEQUENCE
