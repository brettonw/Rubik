//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/06/97
//
//	02/24/97	DS		Added AltDrawMode
//
//------------------------------------------------------------------------------

#ifndef	STATIONARY_SEQUENCE
#define	STATIONARY_SEQUENCE

//------------------------------------------------------------------------------
#include "sequence ptr.h"

//------------------------------------------------------------------------------
class StatSeq : public Sequence																									//	stationary sequence of visual effects
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Point_2d	location;																											//	the location of the stationary sequence
				SeqPtr		sequence;																											//	the actual sequence to draw
	public:																																				//	members available externally
				int				tag;																													//	tag to identify whether or not this sequence has already been drawn
	public:																																				//	members available externally
				StatSeq (CPoint where, SeqPtr sequence);																//	constructor
virtual	~StatSeq (void);																												//	destructor
virtual	Bool			Step (CPoint where);																					//	draw the next frame
virtual	Bool			Step (CPoint where, int mode);																//	draw the next frame
virtual	int				Width (void);																									//	return the width of the sequence
virtual	int				Height (void);																								//	return the height of the sequence
				Point_2d	Location (void);																							//	return the location of the stationary sequence
};																																							//	end

//------------------------------------------------------------------------------
inline	Point_2d	StatSeq::Location (void)																			//	return the location of the stationary sequence
{																																								//	begin
	return location;																															//	return the location
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	STATIONARY_SEQUENCE