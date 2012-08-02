//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/06/97
//
//------------------------------------------------------------------------------

#ifndef	PARTITION_SEQUENCE
#define	PARTITION_SEQUENCE

//------------------------------------------------------------------------------
#include "stationary sequence.h"
#include "list.h"

//------------------------------------------------------------------------------
class PartitionSeq : public Sequence																						//	partition sequence of visual effects
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				int				width;
				int				height;
				List			**grid;
	public:																																				//	members available externally
				PartitionSeq (int width, int height);																		//	constructor
virtual	~PartitionSeq (void);																										//	destructor
virtual	Bool			Step (CPoint where);																					//	draw the next frame
				void			Install (StatSeq *seq);																				//	add a sequence to the partitioning
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	PARTITION_SEQUENCE
