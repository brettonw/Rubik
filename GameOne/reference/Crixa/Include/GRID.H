//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/23/96
//
//------------------------------------------------------------------------------

#ifndef	GRID
#define	GRID

//------------------------------------------------------------------------------
#include "geometry.h"
#include "sect.h"

//------------------------------------------------------------------------------
class Grid : public Geometry																										//	world class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
  			Grid (void);																														//	constructor
virtual	~Grid (void);																														//	destructor
virtual	Real			Bound (void);																									//	return the radius of a bounding circle for this geometry
virtual	Real			TraceRay (CPoint origin, CVector direction) = 0;							//	trace a ray through the grid
virtual	Real			Intersect (CPoint p, CVector v, Real r, Sect *s) = 0;					//	return the nearest intersection time for the grid elements
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	GRID
