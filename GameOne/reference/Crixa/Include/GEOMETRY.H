//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/13/97
//
//------------------------------------------------------------------------------

#ifndef	GEOMETRY
#define	GEOMETRY

//------------------------------------------------------------------------------
enum	{CIRCLE_GEOM = 0, POLYGON_GEOM, GRID_GEOM};																//	geometry types

//------------------------------------------------------------------------------
class Geometry																																	//	a geometry description for the physics model
{																																								//	begin
	private:																																			//	members internal to this class only
				int				type;																													//	the type of geometry this is (circle, polygon, or grid)
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
  			Geometry (int type);																										//	constructor
virtual	~Geometry (void);																												//	destructor
virtual	Real			Bound (void) = 0;																							//	return the radius of a bounding circle for this geometry
				int				Type (void);																									//	return the type
};																																							//	end

//------------------------------------------------------------------------------
inline	Geometry::Geometry (int typ)																						//	contructor
{																																								//	begin
	type = typ;																																		//	save the type
}																																								//	end

//------------------------------------------------------------------------------
inline	int		Geometry::Type (void)																							//	return the type
{																																								//	begin
	return type;																																	//	return the geometry type
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	GEOMETRY
