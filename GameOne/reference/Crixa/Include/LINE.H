//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/03/96
//
//------------------------------------------------------------------------------

#ifndef	LINE
#define	LINE

//------------------------------------------------------------------------------
#include "geometry.h"
#include "space.h"
#include "sect.h"

//------------------------------------------------------------------------------
class Line : public Geometry																										//	a line geometry description for the physics model
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
  			Line (void);																														//	constructor
virtual	~Line (void);																														//	destructor
virtual	Real			Intersect (CPoint p, CVector v, Real r, Sect *s, int *d) = 0;	//	return the nearest intersection with a circle
virtual	Real			Classify (CPoint p) = 0;																			//	return which side of the line the piece is on
};																																							//	end

//------------------------------------------------------------------------------
inline	Line::Line (void) : Geometry (LINE_GEOM)																//	contructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	LINE
