//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/02/96
//
//------------------------------------------------------------------------------

#ifndef	HLINE
#define	HLINE

//------------------------------------------------------------------------------
#include "line.h"

//------------------------------------------------------------------------------
class HLine : public Line																												//	a horizontal line geometry description for the physics model
{																																								//	begin
	private:																																			//	members internal to this class only
				Real			y;																														//	the y coordinate of the horizontal line
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
  			HLine (Real y);																													//	constructor
virtual	~HLine (void);																													//	destructor
virtual	Real			Intersect (CPoint p, CVector v, Real r, Sect *s, int *d);			//	return the nearest intersection with a circle
virtual	Real			Classify (CPoint p);																					//	return which side of the line the point is on
				Real			GetY (void);																									//	return the y coordinate of the line
};																																							//	end

//------------------------------------------------------------------------------
inline	HLine::HLine (Real yp)																									//	constructor
{																																								//	begin
	y = yp;																																				//	copy the y paramater
}																																								//	end

//------------------------------------------------------------------------------
inline	Real	HLine::GetY (void)																								//	return the y coordinate of the line
{																																								//	begin
	return y;																																			//	return the coordinate
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	HLINE
