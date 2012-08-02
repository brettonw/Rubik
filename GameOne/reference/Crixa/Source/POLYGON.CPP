//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/19/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "polygon.h"

//------------------------------------------------------------------------------
Poly::Poly (int c, ...) : Geometry (POLYGON_GEOM)																//	constructor
{																																								//	begin
	count = c;																																		//	copy the number of vertices
	vertex = new Vector_2d[count];																								//	allocate the array of vertices for the polygon
	edge = new Vector_2d[count];																									//	allocate the array of edges for the polygon
	length = new Real[count];																											//	allocate the array of edge lengths
	bound = ZERO;																																	//	start with a zero size bound
	va_list	var;																																	//	variable argument list
	va_start (var, c);																														//	set up the variable argument list
	int		i;																																			//	loop counter
	for (i = 0; i < count; i++)																										//	loop over all of the vertices
	{																																							//	begin
		vertex[i] = va_arg (var, Vector_2d);																				//	copying them from the argument list
		Real	lensq = vertex[i] | vertex[i];																				//	compute the squared length of the vector from the origin to this vertex
		if (lensq > bound)																													//	if that value is larger than the current bound
			bound = lensq;																														//	save it
	}																																							//	end
	va_end (var);																																	//	clean up the argument list
	for (i = 0; i < count; i++)																										//	loop over all of the vertices
	{																																							//	begin
		int		next = (i + 1) % count;																								//	compute the index of the next vertex
		edge[i] = vertex[next] - vertex[i];																					//	compute the ith edge
		length[i] = edge[i].Normalize ();																						//	normalize the edge vector and save the length
	}																																							//	end
	bound = SQRT (bound) + EPSILON;																								//	take the square root of the squared length and add a buffer
}																																								//	end

//------------------------------------------------------------------------------
Poly::~Poly (void)																															//	destructor
{																																								//	begin
	delete[] vertex;																															//	free up the vertex array
	delete[] edge;																																//	free up the edge array
	delete[] length;																															//	free up the length list
}																																								//	end

//------------------------------------------------------------------------------
Poly	*Poly::Rectangle (Real width, Real height)																//	return a regular rectangular polygon centered at the origin
{																																								//	begin
	Real	halfW = width * R(0.5),																									//	compute the half width
				halfH = height * R(0.5);																								//	compute the half height
	return new Poly (4, Point_2d (halfW, halfH), Point_2d (-halfW, halfH),				//	return the rectangle centered at the origin
									Point_2d (-halfW, -halfH), Point_2d (halfW, -halfH));					//	return the rectangle centered at the origin (continued)
}																																								//	end

//------------------------------------------------------------------------------
Real	Poly::Bound (void)																												//	return the radius of a bounding circle for this geometry
{																																								//	begin
	return bound;																																	//	return the bounding radius
}																																								//	end

//------------------------------------------------------------------------------
