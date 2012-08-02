//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/13/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "intersection node.h"
#include "link node.h"
#include "referee.h"
#include "quadratic.h"
#include "circle.h"
#include "polygon.h"
#include "grid.h"
#include "math utility.h"
#include "get parameters.h"

//------------------------------------------------------------------------------
Stack		INode::pool;																														//	the intersection node pool

//------------------------------------------------------------------------------
Real	INode::SectCircleCircle (void)																						//	compute an intersection of two circles
{																																								//	begin
	Body			*body0 = piece[0]->piece->GetBody (),																//	get the body from the first piece
						*body1 = piece[1]->piece->GetBody ();																//	get the body from the second piece
	Real			rad0 = ((Circle *) piece[0]->piece->GetGeometry ())->Radius (),			//	get the radius of the first body
						rad1 = ((Circle *) piece[1]->piece->GetGeometry ())->Radius (),			//	get the radius of the second body
						d = rad0 + rad1,																										//	compute the radius to test for intersection
						root1,																															//	place to store the first root of the intersection equation
						root2;																															//	place to store the second root of the intersection equation
	Vector_2d	C = body0->TLocation () - body1->TLocation (),											//	compute the vector between the two objects
						V = body0->TVelocity () - body1->TVelocity ();											//	compute the relative velocity of the two objects
	if (Dist (C, V, d, root1, root2))																							//	compute the times at which circle 1 is intersecting with circle 2
		if (root1 < ZERO)																														//	if that intersection has already occured
				if (root2 > -root1)																											//	if we are less than half way penetrating the object
				{																																				//	begin
					sect.plane = C;																												//	compute the intersection plane
					sect.plane.Normalize ();																							//	make the plane unit length
					sect.point = body0->TLocation () + (-rad0 * sect.plane);							//	compute the intersection point
					return ZERO;																													//	return a current intersection
				}																																				//	end
				else																																		//	otherwise
					return INFINITY;																											//	return no intersection
		else																																				//	end
		{																																						//	begin
			CPoint	pt0 = body0->TLocAt (root1),																			//	compute the location of the first body at the intersection time
							pt1 = body1->TLocAt (root1);																			//	compute the location of the second body at the intersection time
			sect.plane = pt0 - pt1;																										//	compute the intersection plane
			sect.plane.Normalize ();																									//	make the plane unit length
			sect.point = pt0 + (-rad0 * sect.plane);																	//	compute the intersection point
			return root1;																															//	return the first intersection
		}																																						//	end
	return INFINITY;																															//	the roots are complex numbers, no intersection occurs
}																																								//	end

//------------------------------------------------------------------------------
Real	INode::SectCirclePolygon (void)																						//	compute an intersection of a circle and a polygon
{																																								//	begin
	Body			*body0 = piece[0]->piece->GetBody (),																//	get the body from the first piece
						*body1 = piece[1]->piece->GetBody ();																//	get the body from the second piece
	Poly			*poly = (Poly *) piece[1]->piece->GetGeometry ();										//	get a pointer to the polygon geometry
	Real			rad0 = ((Circle *) piece[0]->piece->GetGeometry ())->Radius (),			//	get the radius of the first body
						rad1 = poly->Bound (),																							//	get the bounding radius of the second body
						d = rad0 + rad1,																										//	compute the radius to test for intersection
						root1,																															//	place to store the first root of the intersection equation
						root2,																															//	place to store the second root of the intersection equation
						time = INFINITY;																										//	the intersection time
	Vector_2d	C = body0->TLocation () - body1->TLocation (),											//	compute the vector between the two objects
						V = body0->TVelocity () - body1->TVelocity ();											//	compute the relative velocity of the two objects
	if ((Dist (C, V, d, root1, root2)) && (root2 >= ZERO))												//	if we pass the bounding circle test
	{																																							//	begin
		for (int i = 0; i < poly->Count (); i++)																		//	loop over all of the vertices and edges
		{																																						//	begin
			Point_2d	vertex = body1->TLocation () + poly->Vertex (i);								//	compute the actual vertex location
			C = body0->TLocation () - vertex;																					//	compute the relative vector from the circle to the vertex
			if (Dist (C, V, rad0, root1, root2))																			//	if the circle intersects with the vertex
			{																																					//	begin
				if (root1 < ZERO)																												//	if the circle has already intersected the vertex
					if (root2 > -root1)																										//	and it is less than half way penetrated
						root1 = ZERO;																												//	use zero as the root time
					else																																	//	otherwise
						root1 = INFINITY;																										//	set no intersection with this vertex
				if (root1 < time)																												//	if the intersection time is less than the saved time
				{																																				//	begin
					time = root1;																													//	save the intersection time
					CPoint	pt0 = body0->TLocAt (root1),																	//	compute the location of the first body at the intersection time
									pt1 = body1->TLocAt (root1) + poly->Vertex (i);								//	compute the location of the second body at the intersection time
					sect.plane = pt0 - pt1;																								//	compute the intersection plane
					sect.plane.Normalize ();																							//	make the plane unit length
					sect.point = pt0 - (rad0 * sect.plane);																//	compute the intersection point
				}																																				//	end
			}																																					//	end
			CVector		edge = poly->Edge (i);																					//	get the current edge
			Vector_2d	N (edge[Y], -edge[X]);																					//	compute its normal vector
			Real			vel = V | N;																										//	compute the closin velocity along the normal
			if (vel < ZERO)																														//	if the circle is actually converging on this edge
			{																																					//	begin
				Point_2d	P = vertex + (N * rad0);																			//	compute the line location for convolution with the circle
				Real			itime = ((P - body0->TLocation ()) | N) / vel;								//	compute the actual intersection time from the circle's point of view
				if ((itime >= ZERO) && (itime < time))																	//	if the intersection time is positive, and it is less than our previously computed values
				{																																				//	begin
					Point_2d	ipt = body0->TLocation () + (body0->TVelocity () * itime);	//	compute the intersection point of the circle's center point and the convolved edge
					Real			etime = (ipt - P) | edge;																		//	compute the intersection time from the edge's point of view
					if ((etime >= ZERO) && (etime <= poly->Length (i)))										//	if the intersection occurs in the segment defined by the edge
					{																																			//	begin
						time = itime;																												//	save the intersection time
						sect.plane = N;																											//	save the edge normal
						sect.point = ipt - (N * rad0);																			//	figure the actual intersection point
					}																																			//	end
				}																																				//	end
			}																																					//	end
		}																																						//	end
	}																																							//	end
	return time;																																	//	return the found intersection time
}																																								//	end

//------------------------------------------------------------------------------
Real	INode::SectCircleGrid (void)																							//	compute an intersection of a circle and a grid
{																																								//	begin
	Real	rad = ((Circle *) piece[0]->piece->GetGeometry ())->Radius ();					//	fetch the radius of the circle
	Grid	*grid = (Grid *) piece[1]->piece->GetGeometry ();												//	get the geometry from the second piece
	Body	*body = piece[0]->piece->GetBody ();																		//	get the body from the first piece
	return grid->Intersect (body->TLocation (), body->TVelocity (), rad, &sect);	//	return the time of intersection of the circle and the grid
}																																								//	end

//------------------------------------------------------------------------------
Real	INode::SectPolygonPolygon (void)																					//	compute an intersection of two polygons
{																																								//	begin
	Body			*body0 = piece[0]->piece->GetBody (),																//	get the body from the first piece
						*body1 = piece[1]->piece->GetBody ();																//	get the body from the second piece
	Real			rad0 = piece[0]->piece->GetGeometry ()->Bound (),										//	get the bounding radius of the first body
						rad1 = piece[1]->piece->GetGeometry ()->Bound (),										//	get the bounding radius of the second body
						d = rad0 + rad1,																										//	compute the radius to test for intersection
						root1,																															//	place to store the first root of the intersection equation
						root2;																															//	place to store the second root of the intersection equation
	Vector_2d	C = body0->TLocation () - body1->TLocation (),											//	compute the vector between the two objects
						V = body0->TVelocity () - body1->TVelocity ();											//	compute the relative velocity of the two objects
	if (Dist (C, V, d, root1, root2))																							//	compute the times at which circle 1 is intersecting with circle 2
	{																																							//	begin
		if (root2 < ZERO)																														//	if that intersection has already occured
			return INFINITY;																													//	we are moving away from the object so don't compute intersection
		//	test vertices from one against the edges of the other
		//	then do the same from the other point of view
	}																																							//	end
	return INFINITY;																															//	no intersection occurs
}																																								//	end

//------------------------------------------------------------------------------
Real	INode::SectPolygonGrid (void)																							//	compute an intersection of a polygon and a grid
{																																								//	begin
	//	test the edges of the polygon against the convex vertices
	//	then test the vertices of the polygon against the edges of the world
	return INFINITY;																															//	no intersection occurs
}																																								//	end

//------------------------------------------------------------------------------
void	INode::CollideCircleCircle (void)																					//	compute collision forces for two impacting circles
{																																								//	begin
	PiecePtr	p0 = piece[0]->piece,																								//	get the first piece
						p1 = piece[1]->piece;																								//	get the second piece
	Body			*body0 = p0->GetBody (),																						//	get the body from the first piece
						*body1 = p1->GetBody ();																						//	get the body from the second piece
	Real			r0 = ((Circle *) p0->GetGeometry ())->Radius (),										//	get the radius of the first body
						r1 = ((Circle *) p1->GetGeometry ())->Radius ();										//	get the radius of the second body
	Vector_2d	M (sect.plane[Y], -sect.plane[X]),																	//	compute the vector perpendicular to the normal
						Vr = body0->TVelocity () - body1->TVelocity ();											//	compute the relative velocity of the two objects
	Real			m1_m2 = body0->OverTInertia () + body1->OverTInertia (),						//	sum the transaltional inertia reciprocals
						w0 = body0->RVelocity (),																						//	get the rotational velocity from the first body
						w1 = body1->RVelocity (),																						//	get the rotational velocity from the second body
						OverI0 = body0->OverRInertia (),																		//	get the reciprocal of the rotational inertia from the first body
						OverI1 = body1->OverRInertia (),																		//	get the reciprocal of the rotational inertai from the second body
						Vn = Vr | sect.plane,																								//	compute the normal component of the relative velocity
						elastic = (p0->GetHardness () * p1->GetHardness ()) + R(1.0),				//	compute the elasticity of the collision from the hardness of the two pieces
						Pn = (-Vn / m1_m2) * elastic,																				//	compute the normal component of the impulse
						Vm = (Vr | M) + (w0 * r0) + (w1 * r1),															//	compute the relative horizontal velocity of the two bodies
						Pm = -Vm / (m1_m2 + (r0 * r0 * OverI0) + (r1 * r1 * OverI1));				//	compute the horizontal component of the impulse
	if (FABS (Pm) > ParmReal (STATIC_FRICTION) * Pn)															//	if the horizontal component exceeds the maximum static friction force
		Pm = ParmReal (KINETIC_FRICTION) * Pn * Sign (Pm);													//	use kinetic friction instead
	body0->ApplyForce ((sect.plane * Pn) + (M * Pm), sect.point);									//	apply the reaction force to object one
	body1->ApplyForce ((sect.plane * -Pn) + (M * -Pm), sect.point);								//	apply the reaction force to object two
}																																								//	end

//------------------------------------------------------------------------------
void	INode::CollideCirclePolygon (void)																				//	compute collision forces for a cirlce impacting a polygon
{																																								//	begin
	PiecePtr	p0 = piece[0]->piece,																								//	get the first piece
						p1 = piece[1]->piece;																								//	get the second piece
	Body			*body0 = p0->GetBody (),																						//	get the body from the first piece
						*body1 = p1->GetBody ();																						//	get the body from the second piece
	Real			r0 = ((Circle *) p0->GetGeometry ())->Radius ();										//	get the radius of the first body
	Vector_2d	M (sect.plane[Y], -sect.plane[X]),																	//	compute the vector perpendicular to the normal
						Vr = body0->TVelocity () - body1->TVelocity ();											//	compute the relative velocity of the two objects
	Real			m1_m2 = body0->OverTInertia () + body1->OverTInertia (),						//	sum the transaltional inertia reciprocals
						w0 = body0->RVelocity (),																						//	get the rotational velocity from the first body
						OverI0 = body0->OverRInertia (),																		//	get the reciprocal of the rotational inertia from the first body
						Vn = Vr | sect.plane,																								//	compute the normal component of the relative velocity
						elastic = (p0->GetHardness () * p1->GetHardness ()) + R(1.0),				//	compute the elasticity of the collision from the hardness of the two pieces
						Pn = (-Vn / m1_m2) * elastic,																				//	compute the normal component of the impulse
						Vm = (Vr | M) + (w0 * r0),																					//	compute the relative horizontal velocity of the two bodies
						Pm = -Vm / (m1_m2 + (r0 * r0 * OverI0));														//	compute the horizontal component of the impulse
	if (FABS (Pm) > ParmReal (STATIC_FRICTION) * Pn)															//	if the horizontal component exceeds the maximum static friction force
		Pm = ParmReal (KINETIC_FRICTION) * Pn * Sign (Pm);													//	use kinetic friction instead
	body0->ApplyForce ((sect.plane * Pn) + (M * Pm), sect.point);									//	apply the reaction force to object one
	body1->ApplyForce ((sect.plane * -Pn) + (M * -Pm), sect.point);								//	apply the reaction force to object two
}																																								//	end

//------------------------------------------------------------------------------
void	INode::CollideCircleGrid (void)																						//	compute collision forces for a circle impacting a grid
{																																								//	begin
	Vector_2d	M (sect.plane[Y], -sect.plane[X]);																	//	compute the vector perpendicular to the normal
	PiecePtr	p0 = piece[0]->piece,																								//	get the first piece
						p1 = piece[1]->piece;																								//	get the second piece
	Body	*body = p0->GetBody ();																									//	get the body from the first piece
	Real	r = ((Circle*) p0->GetGeometry ())->Radius (),													//	get the radius of the circle
				Vn = body->TVelocity () | sect.plane,																		//	compute the component of the velocity in the normal direction
				elastic = (p0->GetHardness () * p1->GetHardness ()) + R(1.0),						//	compute the elasticity of the collision from the hardness of the two pieces
				Pn = -Vn * body->TInertia () * elastic,																	//	compute the normal component of the impulse
				Vm = (body->TVelocity () | M) + (body->RVelocity () * r),								//	compute the relative velocity of the surfaces in the horizontal direction
				Pm = -Vm / (body->OverTInertia () + ((r * r) * body->OverRInertia ()));	//	compute the horizontal component of the impulse
	if (FABS (Pm) > ParmReal (STATIC_FRICTION) * Pn)															//	if the horizontal component exceeds the maximum static friction force
		Pm = ParmReal (KINETIC_FRICTION) * Pn * Sign (Pm);													//	use kinetic friction instead
	body->ApplyForce ((sect.plane * Pn) + (M * Pm), sect.point);									//	apply the reaction force to the object
}																																								//	end

//------------------------------------------------------------------------------
void	INode::CollidePolygonPolygon (void)																				//	compute collision forces for two impacting polygons
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	INode::CollidePolygonGrid (void)																					//	compute collision forces for a polygon impacting a grid
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
inline	void	INode::Swap (PieceNode *&a, PieceNode *&b)												//	swap two piece pointers
{																																								//	begin
	PieceNode	*tmp = a;																														//	save a in a temporary variable
	a = b;																																				//	move b to a
	b = tmp;																																			//	move the old a to b
}																																								//	end

//------------------------------------------------------------------------------
INode::INode (PieceNode *a, PieceNode *b)																				//	constructor
{																																								//	begin
	Geometry	*geom2 = b->piece->GetGeometry ();																	//	store the geometry of b
	switch (a->piece->GetGeometry ()->Type ())																		//	depending on the geometry type of a
	{																																							//	begin
		case CIRCLE_GEOM:																														//	a is a circle
			switch (geom2->Type ())																										//	depending on the geometry type of b
			{																																					//	begin
				case CIRCLE_GEOM:																												//	b is a circle
					Intersect = &INode::SectCircleCircle;																	//	set the intersection routine to circle/circle
					Collide = &INode::CollideCircleCircle;																//	set the collision routine
					break;																																//	end case
				case POLYGON_GEOM:																											//	b is a polygon
					Intersect = &INode::SectCirclePolygon;																//	set the intersection routine to circle/polygon
					Collide = &INode::CollideCirclePolygon;																//	set the collision routine
					break;																																//	end case
				case GRID_GEOM:																													//	b is a grid
					Intersect = &INode::SectCircleGrid;																		//	set the intersection routine to circle/grid
					Collide = &INode::CollideCircleGrid;																	//	set the collision routine
					break;																																//	end case
			}																																					//	end
			break;																																		//	end case
		case POLYGON_GEOM:																													//	a is a polygon
			switch (geom2->Type ())																										//	depending on the geometry type of b
			{																																					//	begin
				case CIRCLE_GEOM:																												//	b is a circle
					Swap (a, b);																													//	switch the pointers so a is the circle
					Intersect = &INode::SectCirclePolygon;																//	set the intersection routine to circle/polygon
					Collide = &INode::CollideCirclePolygon;																//	set the collision routine
					break;																																//	end case
				case POLYGON_GEOM:																											//	b is a polygon
					Intersect = &INode::SectPolygonPolygon;																//	set the intersection routine to polygon/polygon
					Collide = &INode::CollidePolygonPolygon;															//	set the collision routine
					break;																																//	end case
				case GRID_GEOM:																													//	b is a grid
					Intersect = &INode::SectPolygonGrid;																	//	set the intersection routine to polygon/grid
					Collide = &INode::CollidePolygonGrid;																	//	set the collision routine
					break;																																//	end case
			}																																					//	end
			break;																																		//	end case
		case GRID_GEOM:																															//	a is a grid
			Swap (a, b);																															//	switch the pointers so b is the grid
			switch (geom2->Type ())																										//	depending on the geometry type of b
			{																																					//	begin
				case CIRCLE_GEOM:																												//	b is a circle
					Intersect = &INode::SectCircleGrid;																		//	set the intersection routine to circle/grid
					Collide = &INode::CollideCircleGrid;																	//	set the collision routine
					break;																																//	end case
				case POLYGON_GEOM:																											//	b is a polygon
					Intersect = &INode::SectPolygonGrid;																	//	set the intersection routine to polygon/grid
					Collide = &INode::CollidePolygonGrid;																	//	set the collision routine
					break;																																//	end case
			}																																					//	end
			break;																																		//	end case
	}																																							//	end
	piece[0] = a;																																	//	copy the first object
	piece[1] = b;																																	//	copy the second object
	update = TRUE;																																//	set the update flag
}																																								//	end

//------------------------------------------------------------------------------
INode::~INode (void)																														//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	INode::Update (void)																											//	update the intersection time for this record
{																																								//	begin
	value = (this->*Intersect) ();																								//	compute the intersection time of two objects
	if (value != INFINITY)																												//	if an intersection occurs
		value += gReferee->Clock ();																								//	add in the current clock time
	update = FALSE;																																//	clear the update flag
}																																								//	end

//------------------------------------------------------------------------------
void	*INode::operator new (size_t size)																				//	free store allocation
{																																								//	begin
	ListNode	*node = pool.Pop ();
	if (!node)
		return ::operator new (size);
	return node;
}																																								//	end

//------------------------------------------------------------------------------
void	INode::operator delete (void *ptr, size_t size)														//	free store recovery
{																																								//	begin
	pool.Push ((INode *) ptr);
}																																								//	end

//------------------------------------------------------------------------------
void	INode::PreAllocate (int number)																						//	pre-allocate intersection nodes
{																																								//	begin
	INode	**table = new INode*[number];
	for (int i = 0; i < number; i++)
		table[i] = new INode;
	for (i = 0; i < number; i++)
		delete table[i];
	delete[] table;
}																																								//	end

//------------------------------------------------------------------------------
