//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/17/97
//
//------------------------------------------------------------------------------

#ifndef	INTERSECTION_NODE
#define	INTERSECTION_NODE

//------------------------------------------------------------------------------
#include "priority node.h"
#include "piece node.h"
#include "sect.h"

//------------------------------------------------------------------------------
class INode : public PriorityNode																								//	an intersection node
{																																								//	begin
	private:																																			//	members internal to this class only
static	Stack			pool;																													//	the intersection node pool
	protected:																																		//	members internal to this class hierarchy
				Real			(INode::*Intersect) (void);																		//	pointer to the intersection function for these two geometries
				void			(INode::*Collide) (void);																			//	pointer to the intersection function for these two geometries
	protected:																																		//	members internal to this class hierarchy
				Real			SectCircleCircle (void);																			//	compute an intersection of two circles
				Real			SectCirclePolygon (void);																			//	compute an intersection of a circle and a polygon
				Real			SectCircleGrid (void);																				//	compute an intersection of a circle and a grid
				Real			SectPolygonPolygon (void);																		//	compute an intersection of two polygons
				Real			SectPolygonGrid (void);																				//	compute an intersection of a polygon and a grid
				void			CollideCircleCircle (void);																		//	compute collision forces for two impacting circles
				void			CollideCirclePolygon (void);																	//	compute collision forces for a cirlce impacting a polygon
				void			CollideCircleGrid (void);																			//	compute collision forces for a circle impacting a grid
				void			CollidePolygonPolygon (void);																	//	compute collision forces for two impacting polygons
				void			CollidePolygonGrid (void);																		//	compute collision forces for a polygon impacting a grid
				void			Swap (PieceNode *&a, PieceNode *&b);													//	swap two piece pointers
	public:																																				//	members available externally
				Sect			sect;																													//	intersection location data
				Bool			update;																												//	whether or not to update the node
				PieceNode	*piece[2];																										//	the two pieces this intersection records
	public:																																				//	members available externally
				INode (void) {}																													//	constructor
				INode (PieceNode *a, PieceNode *b);																			//	constructor
				~INode (void);																													//	destructor
				void			Update (void);																								//	update the intersection time for this record
				void			DoCollide (void);																							//	collide the two bodies
				void			*operator new (size_t size);																	//	free store allocation
				void			operator delete (void *ptr, size_t size);											//	free store recovery
static	void			PreAllocate (int number = 2500);															//	pre-allocate intersection nodes
};																																							//	end

//------------------------------------------------------------------------------
inline	void	INode::DoCollide (void)																						//	collide the two bodies
{																																								//	begin
	(this->*Collide) ();																													//	call the collide function appropriate to this node
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	INTERSECTION_NODE
