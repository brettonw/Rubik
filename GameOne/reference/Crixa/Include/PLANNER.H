//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	07/29/96
//
//------------------------------------------------------------------------------

#ifndef	PLANNER
#define	PLANNER

//------------------------------------------------------------------------------
#include "path.h"
#include "int vector.h"
#include "priority.h"

//------------------------------------------------------------------------------
class Planner																																		//	motion planning class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				IVec_2d		moves[4];																											//	array of moves
static	IVec_2d		full;																													//	a vector that represents a full cell
static	IVec_2d		empty;																												//	a vector that represents an empty cell
				IVec_2d		**grid;																												//	two dimensional array of cells
				IVec_2d		start;																												//	the start point of the plan
				IVec_2d		stop;																													//	the stop point of the plan
				int				width;																												//	width of the grid
				int				height;																												//	height of the grid
				int				shift;																												//	scale shift value for converting grid space to world space
				int				halfSize;																											//	half the size of a cell in world space
				Real			overSize;																											//	scale factor for converting world space to grid space
				Priority	*queue;																												//	priority queue used for planning
 const	IVec_2d		&GetCell (IVec_2d cell);																			//	return the requested cell
				void			SetCell (IVec_2d cell, IVec_2d vec);													//	set the value of a cell
				Real			Weight (IVec_2d cell);																				//	return a weighting value for a cell position in the search
				Path			*OptimizePts (int cnt, Point_2d *pts);												//	optimize an array of points for a path
	public:																																				//	members available externally
  			Planner (int scaleShift = 2);																						//	constructor
				~Planner (void);																												//	destructor
				void			StartPlan (CPoint start, CPoint stop);												//	start up a new plan
				Bool			Plan (void);																									//	incremental plan
				Path			*BuildPath (void);																						//	build a path from a completed plan
				void			Clear (void);																									//	clear the current plan
				Real			TraceRay (CPoint origin, CVector direction);									//	trace the specified ray through the planning grid
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	PLANNER
