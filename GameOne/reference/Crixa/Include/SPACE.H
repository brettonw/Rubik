//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	03/25/96
//
//------------------------------------------------------------------------------

#ifndef	SPACE
#define	SPACE

//------------------------------------------------------------------------------
#include "point.h"
#include "vector.h"

//------------------------------------------------------------------------------
#define	CPoint	const Point_2d &																								//	expression to be used for constant references to a point
#define	CVector	const Vector_2d &																								//	expression to be used for constant references to a vector

//------------------------------------------------------------------------------
Vector_2d		Reflect (CVector vec, CVector axis);																//	reflect a source vector around an axis
Vector_2d		Rotate (CVector vec, Real angle);																		//	rotate a source vector some angle
Vector_2d		Rotate (CVector vec, CVector axis);																	//	rotate a source vector to an axis

//------------------------------------------------------------------------------
inline	Vector_2d	operator - (CPoint p1, CPoint p2)															//	subtraction of two points
{																																								//	begin
	return Vector_2d (p1[X] - p2[X], p1[Y] - p2[Y]);															//	return a vector as the result
}																																								//	end

//------------------------------------------------------------------------------
inline	Point_2d	operator + (CPoint p, CVector v)															//	addition of a point and a vector
{																																								//	begin
	return Point_2d (p[X] + v[X], p[Y] + v[Y]);																		//	return a point as the result
}																																								//	end

//------------------------------------------------------------------------------
inline	Point_2d	operator + (CVector v, CPoint p)															//	addition of a point and a vector
{																																								//	begin
	return Point_2d (p[X] + v[X], p[Y] + v[Y]);																		//	return a point as the result
}																																								//	end

//------------------------------------------------------------------------------
inline	Vector_2d	operator + (CVector v1, CVector v2)														//	addition of two vectors
{																																								//	begin
	return Vector_2d (v1[X] + v2[X], v1[Y] + v2[Y]);															//	return a vector as the result
}																																								//	end

//------------------------------------------------------------------------------
inline	Vector_2d	operator * (CVector v, Real c)																//	multiplication of a constant and a vector
{																																								//	begin
	return Vector_2d (v[X] * c, v[Y] * c);																				//	return a vector as the result
}																																								//	end

//------------------------------------------------------------------------------
inline	Vector_2d	operator / (CVector v, Real c)																//	division of a vector by a constant
{																																								//	begin
	return Vector_2d (v[X] / c, v[Y] / c);																				//	return a vector as the result
}																																								//	end

//------------------------------------------------------------------------------
inline	Vector_2d	operator * (Real c, CVector v)																//	multiplication of a constant and a vector
{																																								//	begin
	return Vector_2d (v[X] * c, v[Y] * c);																				//	return a vector as the result
}																																								//	end

//------------------------------------------------------------------------------
inline	Vector_2d	operator - (CVector v)																				//	unary negation of a vector
{																																								//	begin
	return Vector_2d (-v[X], -v[Y]);																							//	return a vector as the result
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	SPACE