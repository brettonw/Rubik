//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/07/97
//
//------------------------------------------------------------------------------

#ifndef	PUMPING_SEQUENCE
#define	PUMPING_SEQUENCE

//------------------------------------------------------------------------------
#include "piece ptr.h"
#include "sequence ptr.h"
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
class PumpSeq : public Sequence																									//	sequence of visual effects that tracks an object's location and pumps moving sequences for it
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				PiecePtr	object;
				Vector_2d	loc;
				SArrayPtr	sprite;
				int				delay;
				int				elapsed;
	public:																																				//	members available externally
				PumpSeq (const SArrayPtr &spr, const PiecePtr &obj, CVector offset);//	constructor
virtual	~PumpSeq (void);																												//	destructor
virtual	Bool			Step (CPoint where);																					//	draw the next frame
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	PUMPING_SEQUENCE