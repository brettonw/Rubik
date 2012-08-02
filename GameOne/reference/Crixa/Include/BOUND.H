//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/21/96
//
//------------------------------------------------------------------------------

#ifndef	BOUND
#define	BOUND

//------------------------------------------------------------------------------
enum	ClipCode	{OUTSIDE = -1, INTERSECTING = 0, INSIDE = 1};										//	different clip codes

//------------------------------------------------------------------------------
class Bound																																			//	a rectangular bound
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
				short	top;																															//	top of the rectangle
				short	left;																															//	left of the rectangle
				short	bottom;																														//	bottom of the rectangle
				short	right;																														//	right of the rectangle
	public:																																				//	members available externally
				Bound (void) {}																													//	constructor
				Bound (int top, int left, int bottom, int right);												//	constructor
				ClipCode	TestClip (const Bound &bound);																//	test this bound with an offset against another for clipping
				ClipCode	TestClip (int x, int y);																			//	test a point against the bound for containment
};																																							//	end

//------------------------------------------------------------------------------
inline	Bound::Bound (int t, int l, int b, int r)																//	constructor
{																																								//	begin
	top = t;																																			//	copy the top edge
	left = l;																																			//	copy the left edge
	bottom = b;																																		//	copy the bottom edge
	right = r;																																		//	copy the right edge
}																																								//	end

//------------------------------------------------------------------------------
extern	Bound	gBound;																														//	global play area bound

//------------------------------------------------------------------------------

#endif	//	BOUND
