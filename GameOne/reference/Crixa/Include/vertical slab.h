//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	09/24/96
//
//------------------------------------------------------------------------------

#ifndef	PARTITION_SLAB
#define	PARTITION_SLAB

//------------------------------------------------------------------------------
class PSlab																																			//	partition slab
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
  			PSlab (void) {}																													//	constructor
virtual	~PSlab (void);																													//	destructor
virtual	Real			Intersect (CPoint p, CVector v, Sect *s);											//	return the nearest intersection time for the grid, and which axis the intersection occurs on
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	PARTITION_SLAB
