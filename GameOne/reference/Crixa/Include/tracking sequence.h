//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/20/97
//
//------------------------------------------------------------------------------

#ifndef	TRACKING_SEQUENCE
#define	TRACKING_SEQUENCE

//------------------------------------------------------------------------------
#include "piece ptr.h"
#include "sequence ptr.h"

//------------------------------------------------------------------------------
class TrackSeq : public Sequence																								//	sequence of visual effects that tracks an object's location
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				PiecePtr	object;
				Vector_2d	loc;
				Vector_2d	loc2;
				SeqPtr		sequence;
	public:																																				//	members available externally
				TrackSeq (const SeqPtr &seq, const PiecePtr &obj, CVector offset);																												//	constructor
				TrackSeq (const SeqPtr &seq, const PiecePtr &obj, CVector offset, CVector offset2);																												//	constructor
virtual	~TrackSeq (void);																												//	destructor
virtual	Bool			Step (CPoint where);																					//	draw the next frame
virtual void			Reset (void);																									//	return the sequence to its starting state
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	TRACKING_SEQUENCE