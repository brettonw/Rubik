//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/02/96
//
//------------------------------------------------------------------------------

#ifndef	VLINE
#define	VLINE

//------------------------------------------------------------------------------
#include "line.h"

//------------------------------------------------------------------------------
class VLine : public Line																												//	a vertical line geometry description for the physics model
{																																								//	begin
	private:																																			//	members internal to this class only
				Real			x;																														//	the x coordinate of the horizontal line
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
  			VLine (Real x);																													//	constructor
virtual	~VLine (void);																													//	destructor
virtual	Real			Intersect (CPoint p, CVector v, Real r, Sect *s, int *d);			//	return the nearest intersection with a circle
virtual	Real			Classify (CPoint p);																					//	return which side of the line the point is on
				Real			GetX (void);																									//	return the x coordinate of the line
};																																							//	end

//------------------------------------------------------------------------------
inline	VLine::VLine (Real xp)																									//	constructor
{																																								//	begin
	x = xp;																																				//	copy the x paramater
}																																								//	end

//------------------------------------------------------------------------------
inline	Real	VLine::GetX (void)																								//	return the y coordinate of the line
{																																								//	begin
	return x;																																			//	return the coordinate
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	VLINE
