//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/06/97
//
//	02/24/97	DS		Added alternative step function
//
//------------------------------------------------------------------------------

#ifndef	SEQUENCE
#define	SEQUENCE

//------------------------------------------------------------------------------
#include "space.h"

//------------------------------------------------------------------------------
class Sequence																																	//	sequence of visual effects
{																																								//	begin
	friend	class	SeqPtr;																													//	the sequence pointer class can play with my private parts
	private:																																			//	members internal to this class only
  			int				refcount;																											//	reference count for the sequence
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
				Sequence (void);																												//	constructor
virtual	~Sequence (void);																												//	destructor
virtual	Bool			Step (CPoint where) = 0;																			//	draw the next frame
virtual	Bool			Step (CPoint where, int mode);																//	special draw the next frame
virtual void			Reset (void);																									//	return the sequence to its starting state
				int				RefCount (void);																							//	return the reference count
virtual	int				Width (void);																									//	return the width of the sequence
virtual	int				Height (void);																								//	return the height of the sequence
};																																							//	end

//------------------------------------------------------------------------------
inline	Sequence::Sequence (void)																								//	constructor
{																																								//	begin
	refcount = 0;																																	//	initialize the reference count
}																																								//	end

//------------------------------------------------------------------------------
inline	int	Sequence::RefCount (void)																						//	return the reference count
{																																								//	begin
	return refcount;																															//	return the count
}																																								//	end

//------------------------------------------------------------------------------
inline Bool	Sequence::Step (CPoint where, int mode)															//	default alternative Step
{																																								//	begin
	return Step(where);																														//	call regular step
}																																								//	end

//------------------------------------------------------------------------------
#endif	//	SEQUENCE