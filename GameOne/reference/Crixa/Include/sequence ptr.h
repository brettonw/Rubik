//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/20/96
//
//------------------------------------------------------------------------------

#ifndef	SEQUENCE_PTR
#define	SEQUENCE_PTR

//------------------------------------------------------------------------------
#include "sequence.h"

//------------------------------------------------------------------------------
class SeqPtr																																		//	sequence pointer class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
  			Sequence		*ptr;																												//	pointer to the sequence
	public:																																				//	members available externally
  			SeqPtr (void);																													//	constructor
  			SeqPtr (Sequence *sequence);																						//	constructor
  			SeqPtr (const SeqPtr &ptr);																							//	constructor
  			~SeqPtr (void);																													//	destructor
				Sequence		*operator -> (void) const;																	//	pointer dereference operator
				Sequence		*operator () (void) const;																	//	function call operator
				SeqPtr			&operator = (const SeqPtr &ptr);														//	assignment operator
};																																							//	end

//------------------------------------------------------------------------------
inline	Sequence	*SeqPtr::operator -> (void) const															//	pointer dereference operator
{																																								//	begin
	return ptr;																																		//	return the pointer
}																																								//	end

//------------------------------------------------------------------------------
inline	Sequence	*SeqPtr::operator () (void) const															//	function call operator
{																																								//	begin
	return ptr;																																		//	return the pointer
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	SEQUENCE_PTR