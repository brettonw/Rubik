//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/02/96
//
//------------------------------------------------------------------------------

#ifndef	CIRCLE
#define	CIRCLE

//------------------------------------------------------------------------------
#include "geometry.h"
#include "space.h"

//------------------------------------------------------------------------------
class Circle : public Geometry																									//	a circular geometry description for the physics model
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Real			radius;																												//	circle radius
	public:																																				//	members available externally
  			Circle (Real radius);																										//	constructor
virtual	~Circle (void);																													//	destructor
virtual	Real			Bound (void);																									//	return the radius of a bounding circle for this geometry
				Real			Radius (void);																								//	return the radius of the circle
				void			SetRadius (Real rad);																					//	set the circle radius
};																																							//	end

//------------------------------------------------------------------------------
inline	Circle::Circle (Real r) : Geometry (CIRCLE_GEOM)												//	constructor
{																																								//	begin
	radius = r;																																		//	set the radius
}																																								//	end

//------------------------------------------------------------------------------
inline	Real	Circle::Radius (void)																							//	return the radius of the circle
{																																								//	begin
	return radius;																																//	return the radius
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Circle::SetRadius (Real rad)																			//	set the circle radius
{																																								//	begin
	radius = rad;																																	//	set the radius
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	CIRCLE
