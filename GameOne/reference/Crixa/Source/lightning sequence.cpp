//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	02/04/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "lightning sequence.h"
#include "referee.h"
#include "display.h"
#include "bound.h"
#include "direction.h"
#include "random.h"
#include "sprite array mgr.h"

//------------------------------------------------------------------------------
class	NodeNode : public ListNode																								//	list node class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
				NodeNode	*parent;
				Point_2d	location;
				Vector_2d	direction;
				int				count;
				int				total;
	public:																																				//	members available externally
				NodeNode (NodeNode *p, CPoint loc, CVector dir, int c, int t);					//	constructor
virtual	~NodeNode (void);																												//	destructor
};																																							//	end

//------------------------------------------------------------------------------
inline	NodeNode::NodeNode (NodeNode *p, CPoint loc, CVector dir, int c, int t)	//	constructor
{																																								//	begin
	parent = p;
	location = loc;
	direction = dir;
	count = c;
	total = t;
}																																								//	end

//------------------------------------------------------------------------------
NodeNode::~NodeNode (void)																											//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	LightSeq::DrawLine (int count, Point_2d o, CVector d, const Pixel &color)	//	draw a line from a point in a general direction
{																																								//	begin
	for (int i = 0; i < count; i++)
	{
		int		x = int (o[X]),																												//	compute the x component of the integer coordinate of the pixel
					y = int (o[Y]);																												//	compute the y component of the integer coordinate of the pixel
		if (gBound.TestClip (x, y) == INSIDE)																				//	if the	point is in the bound
			(*gDisplay) (x, y) += color;																							//	set the pixel
		o += d;
	}
}																																								//	end

//------------------------------------------------------------------------------
void	LightSeq::Branch (CPoint where, CVector direction)
{																																								//	begin
	List			queue;
	int				count;
	Vector_2d	dir;
	do
		dir = gDirection[gRandom.RndInteger ()][0];
	while ((dir | direction) < R(0.75));
	count = 7 + (gRandom.RndInteger () & 15);
	queue.InsertBefore (new NodeNode (0, where, dir, count, count));
	NodeNode	*node = (NodeNode *) queue.Head ();
	while (node)
	{
		queue.Remove (node);
		nodes.InsertBefore (node);
		nodeCount++;
		if (node->total < length)
		{
			do
				dir = gDirection[gRandom.RndInteger ()][0];
			while ((dir | direction) < R(0.75));	//	lower for more branching, really controls spread
			count = 7 + (gRandom.RndInteger () & 15);
			queue.InsertBefore (new NodeNode (node, node->location + (node->direction * node->count), dir, count, node->total + count));
			if ((dir | node->direction) < R(0.55))	//	higher for more branching
			{
				dir += node->direction;
				dir.Normalize ();
				count = 7 + (gRandom.RndInteger () & 15);
				queue.InsertBefore (new NodeNode (node, node->location + (node->direction * node->count), dir, count, node->total + count));
			}
		}
		node = (NodeNode *) queue.Head ();
	}
}																																								//	end

//------------------------------------------------------------------------------
LightSeq::LightSeq (CPoint o, CVector d, int len)																//	constructor
{																																								//	begin
	nodeCount = 0;
	origin = o;
	direction = d;
	length = len;
	Branch (origin, direction);
	step = 0;
}																																								//	end

//------------------------------------------------------------------------------
LightSeq::~LightSeq (void)																											//destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Bool	LightSeq::Step (CPoint where)																							//	step to the next event
{																																								//	begin
	Pixel			blue = MakePixel (0, 0, 12);
	NodeNode	*node = (NodeNode *) nodes.Head ();
	for (int i = 0; i < step; i++)
	{
		DrawLine (node->count, node->location - where, node->direction, blue);
		PiecePtr	unused;
		if (gReferee->Contains (node->location, unused))
		{
			SArrayPtr	sprite = gSpriteMgr.Fetch ("eksthr");
			while (node->parent)
			{
				for (int i = 0; i < node->parent->count; i += 3)
					sprite[3]->Draw ((node->location + (i * -node->parent->direction)) - where);
				node = node->parent;
			}
			step = nodeCount;
			break;
		}
		node = (NodeNode *) nodes.Next (node);
	}
	step = (step + 13) % nodeCount;
	if (step <= 13)
	{
		nodes.Reset ();
		nodeCount = 0;
		Branch (origin, direction);
	}
	return TRUE;
}																																								//	end

//------------------------------------------------------------------------------
