//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Last Revision:	07/31/96
//
//------------------------------------------------------------------------------

#ifndef SWITCH_SEQUENCE
#define SWITCH_SEQUENCE

//------------------------------------------------------------------------------
#include "sequence ptr.h"

//------------------------------------------------------------------------------
#define	kSwitchOff	-1

//------------------------------------------------------------------------------
class SwitchSeq : public Sequence																								//	a switchable sequence
{																																								//	begin
	private:																																			//	private members in the class
	protected:																																		//	protected members in the class
				Point_2d	location;																											//	the location where the sequence takes place
				SeqPtr		*sequences;																										//	pointer to an array of sequences
				int				count;																												//	the number of sequences involved
				int				current;																											//	the current sequence being played
	public:																																				//	public members in the class
				SwitchSeq (CPoint where, int num, ...);																	//	constructor
				SwitchSeq (CPoint where, SeqPtr seq1, SeqPtr seq2);
virtual	~SwitchSeq (void);																											//	destructor
virtual	Bool	Step (CPoint where);																							//	step through the current sequence
virtual	void	Switch (int which = 0, Bool reset = TRUE);												//	switch to a specified sequence
virtual void	Reset	(void);																											//	reset the current sequence
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	SWITCH_SEQUENCE
