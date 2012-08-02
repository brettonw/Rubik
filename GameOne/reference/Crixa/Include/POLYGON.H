//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/19/96
//
//------------------------------------------------------------------------------

#ifndef	POLYGONAL
#define	POLYGONAL

//------------------------------------------------------------------------------
#include "geometry.h"
#include "space.h"

//------------------------------------------------------------------------------
class Poly : public Geometry																										//	a polygonal geometry description for the physics model
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				int				count;																												//	number of vertices in the polygon
				Vector_2d	*vertex;																											//	pointer to an array of vertices
				Vector_2d	*edge;																												//	pointer to an array of edges
				Real			*length;																											//	pointer to an array of edge lengths
				Real			bound;																												//	the bounding radius
	public:																																				//	members available externally
  			Poly (int count, ...);																									//	constructor
virtual	~Poly (void);																														//	destructor
static	Poly			*Rectangle (Real width, Real height);													//	return a regular rectangular polygon centered at the origin
virtual	Real			Bound (void);																									//	return the radius of a bounding circle for this geometry
				int				Count (void);																									//	return the number of vertices
				CVector		Vertex (int index) const;																			//	return a requested vertex
				CVector		Edge (int index) const;																				//	return a requested edge
				Real			Length (int index) const;																			//	return the length of the requested edge
};																																							//	end

//------------------------------------------------------------------------------
inline	int		Poly::Count (void)																								//	return the number of vertices
{																																								//	begin
	return count;																																	//	return the count
}																																								//	end

//------------------------------------------------------------------------------
inline	CVector	Poly::Vertex (int index) const																	//	return a requested vertex
{																																								//	begin
	return vertex[index];																													//	return the vertex
}																																								//	end

//------------------------------------------------------------------------------
inline	CVector	Poly::Edge (int index) const																		//	return a requested edge
{																																								//	begin
	return edge[index];																														//	return the edge
}																																								//	end

//------------------------------------------------------------------------------
inline	Real	Poly::Length (int index) const																		//	return the length of the requested edge
{																																								//	begin
	return length[index];																													//	return the edge length
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	POLYGONAL
