//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	07/25/96
//
//------------------------------------------------------------------------------

#ifndef	PARTICLE_SEQUENCE
#define	PARTICLE_SEQUENCE

//------------------------------------------------------------------------------
#include "sequence ptr.h"
#include "pixel.h"

//------------------------------------------------------------------------------
class PartSeq : public Sequence																									//	a particle sequence
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				SeqPtr		*dots;																												//	array of sequences for the effect
				int				count;																												//	how many dots to draw
	public:																																				//	members available externally
				PartSeq (CPoint pt, CVector vel, Pixel color, Real time, int count);		//	constructor
virtual	~PartSeq (void);																												//	destructor
virtual Bool			Step (CPoint where);																					//	move it move it
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	PARTICLE
