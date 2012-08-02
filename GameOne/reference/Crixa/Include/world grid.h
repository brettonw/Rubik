//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	12/19/96
//
//------------------------------------------------------------------------------

#ifndef	WORLD_GRID
#define	WORLD_GRID

//------------------------------------------------------------------------------
#include "grid.h"

//------------------------------------------------------------------------------
class WGrid : public Grid																												//	world grid class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				struct		Cell																													//	grid cell
				{																																				//	begin
					char		convex:1;																											//	flag indicating that the bottom left corner of this cell is a convex corner
					char		occupied:1;																										//	flag indicating that this cell is occupied
				};																																			//	end
				Cell			**grid;																												//	the grid array
				Cell			border;																												//	a special cell representing the area outside the border
				int				width;																												//	the width of the world
				int				height;																												//	the height of the world
  			Cell			GetCell (int x, int y);																				//	return the requested cell
	public:																																				//	members available externally
  			WGrid (void);																														//	constructor
virtual	~WGrid (void);																													//	destructor
virtual	Real			TraceRay (CPoint origin, CVector direction);									//	trace a ray through the grid
virtual	Real			Intersect (CPoint p, CVector v, Real r, Sect *s);							//	return the nearest intersection time for both edges and corners
  			Real			Edges (CPoint p, CVector v, Real r, Sect *s);									//	return the nearest intersection time for edges only
  			Real			Corners (CPoint p, CVector v, Real r, Sect *s, Real limit);		//	return the nearest intersection time for corners only
				Bool			IsOccupied (int x, int y, Bool safe = FALSE);									//	return true if the cell is occupied
};																																							//	end

//------------------------------------------------------------------------------
#define	CELL_SIZE				R(16.0)																									//	the size of each cell in the grid
#define	OVER_CELL_SIZE	R(0.0625)																								//	one over the size of each cell in the grid
#define	CELL_SHIFT			4																												//	the amount to shift an integer by to get real bounds

//------------------------------------------------------------------------------
inline	Bool	WGrid::IsOccupied (int x, int y, Bool safe)												//	return true if the specified cell is occupied
{																																								//	begin
	if ((x >= 0) && (x < width) && (y >= 0) && (y < height))											//	if the request is in the playable grid
		return grid[y][x].occupied;																									//	return the cell
	else																																					//	otherwise
		return safe;																																//	return the border space
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	WORLD_GRID
