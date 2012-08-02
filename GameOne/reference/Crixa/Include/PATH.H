//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	07/25/96
//
//------------------------------------------------------------------------------

#ifndef	PATH
#define	PATH

//------------------------------------------------------------------------------
#include "space.h"

//------------------------------------------------------------------------------
class Path																																			//	a path of points
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Point_2d		*point;																											//	array of points
				int					count;																											//	how many points in the path
				Bool				loop;																												//	whether or not to loop the path in traversal
				int					current;																										//	current point in the path
	public:																																				//	members available externally
				Path (Bool loop, int count, ...);																				//	constructor
				Path (Bool loop, int count, Point_2d *pts);															//	constructor
				~Path (void);																														//	destructor
				CPoint			operator [] (int i);																				//	array indexing operator
				CPoint			GetCurrent (void);																					//	return the current point
				Bool				Previous (void);																						//	step back to the previous point
				Bool				Next (void);																								//	step forward to the next point
				int					Count (void);																								//	return the number of waypoints in the path
};																																							//	end

//------------------------------------------------------------------------------
inline	CPoint	Path::operator [] (int i)																				//	array indexing operator
{																																								//	begin
	return point[i];																															//	return the indexed point
}																																								//	end

//------------------------------------------------------------------------------
inline	CPoint	Path::GetCurrent (void)																					//	return the current point
{																																								//	begin
	return point[current];																												//	return the currently indexed point
}																																								//	end

//------------------------------------------------------------------------------
inline	int		Path::Count (void)																								//	return the number of waypoints in the path
{																																								//	begin
	return count;																																	//	return the count
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	PATH