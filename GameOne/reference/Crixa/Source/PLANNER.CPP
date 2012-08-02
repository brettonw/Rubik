//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/08/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "planner.h"
#include "level file.h"
#include "text buffer.h"
#include "naive.h"
#include "math utility.h"

//------------------------------------------------------------------------------
class	PlanNode : public PriorityNode																						//	cell plan priority node class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
				IVec_2d			cell;																												//	the cell that this node is for
				PlanNode (Real value, IVec_2d cell);																		//	constructor
virtual	~PlanNode (void);																												//	destructor
};																																							//	end

//------------------------------------------------------------------------------
PlanNode::PlanNode (Real val, IVec_2d cel)																			//	constructor
{																																								//	begin
	value = val;																																	//	copy the value
	cell = cel;																																		//	copy the cell
}																																								//	end

//------------------------------------------------------------------------------
PlanNode::~PlanNode (void)																											//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
IVec_2d	Planner::full (-1, -1);																									//	vector representing a full cell
IVec_2d	Planner::empty (-2, -2);																								//	vector representing an empty cell

//------------------------------------------------------------------------------
inline	const IVec_2d	&Planner::GetCell (IVec_2d vec)														//	return the requested cell
{																																								//	begin
	if ((vec[X] < width) && (vec[X] > 0) && (vec[Y] < height) && (vec[Y] > 0))		//	if the requested cell is in the grid
		return grid[vec[Y]][vec[X]];																								//	return the requested cell
	return full;																																	//	otherwise, return a full cell
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Planner::SetCell (IVec_2d cell, IVec_2d vec)											//	set the value of a cell
{																																								//	begin
	grid[cell[Y]][cell[X]] = vec;																									//	assign the value to the requested cell
}																																								//	end

//------------------------------------------------------------------------------
Real	Planner::Weight (IVec_2d cell)																						//	return a weighting value for a cell position in the search
{																																								//	begin
	IVec_2d	delta = cell - start,																									//	compute the vector from the start point
					delta2 = cell - stop;																									//	compute the vector from the stop point
	Real		distance = SQRT (delta | delta),																			//	compute the distance to the start point
					deviation = distance + SQRT (delta2 | delta2);												//	compute the sum of the two distances as a deviation function
	return distance * deviation;																									//	return the product of the distance and the deviation
}																																								//	end

//------------------------------------------------------------------------------
Path	*Planner::OptimizePts (int count, Point_2d *pts)													//	optimize an array of points for a path
{																																								//	begin
	//	this routine is VERY expensive, and needs to be made into an incremental call somehow
	if (count < 2)																																//	if there are less than two points in the planned path
		return 0;																																		//	there is no need to plan, so return an empty path
	int		cur = count - 1,																												//	start with the last point in the path
				step;																																		//	a step index for optimizing the point set
	do																																						//	loop
	{																																							//	begin
		step = 0;																																		//	set step to the first point
		while (step < cur)																													//	loop up to the current point
			if (TraceRay (pts[cur], pts[step] - pts[cur]) > R(1.0))										//	if the step point it visible from the current point
				break;																																	//	stop looping, we will use this point
			else																																			//	otherwise
				step++;																																	//	advance to the next point
		pts[--cur] = pts[step];																											//	store the found point and decrement the current counter
	}																																							//	end
	while (step);																																	//	loop until the found visible point is the first point
	cur++;																																				//	increment the current index
	count = (count - cur) - 1;																										//	compute the number of points used in the optimal plan
	return new Path (FALSE, count, &pts[cur]);																		//	return the new path created from the optimized point array
}																																								//	end

//------------------------------------------------------------------------------
Planner::Planner (int shif)																											//	constructor
{																																								//	begin
	LevFile		lev ("Grid");
	TxtBuffer	buffer (&lev);																											//	load the grid file
	int		size = 1 << shif;																												//	compute the resolution of the planning grid
	buffer >> width >> height;																										//	read the width and height
	width >>= shif;																																//	reduce the width to our resolution
	height >>= shif;																															//	reduce the height to our resolution
	grid = new IVec_2d*[height];																									//	allocate an indexing array for the grid
	grid[0] = new IVec_2d[width * height];																				//	allocate the grid memory
	cstr	*line = new cstr[size];																									//	allocate an array of lines for our resolution
	for (int i = 0; i < height; i++)																							//	loop over all of the rows in the grid
	{																																							//	begin
		grid[i] = grid[0] + (width * i);																						//	assign the row pointer in the indexing array
		for (int j = 0; j < size; j++)																							//	loop over our resolution
			buffer >> line[j];																												//	reading a line from the grid file
		for (j = 0; j < width; j++)																									//	loop over all of the cells in the row
		{																																						//	begin
			int		sum = 0;																														//	start by assuming the cell is empty
			for (int y = 0; y < size; y++)																						//	loop vertically over our resolution
				for (int x = 0; x < size; x++)																					//	loop horizontally over our resolution
					sum += (line[y][(j * size) + x] - '0') & 0x01;												//	accumulating the occupied cell count in this region
			grid[i][j] = sum ? full : empty;																					//	if any of the sub-cells were occupied, this cell is occupied
		}																																						//	end
	}																																							//	end
	delete[] line;																																//	release the line array
	queue = new Naive;																														//	allocate the priority queue
	moves[0] (1, 0);																															//	move to the right
	moves[1] (-1, 0);																															//	move to the left
	moves[2] (0, 1);																															//	move up
	moves[3] (0, -1);																															//	move down
	shift = shif + 4;																															//	compute the scale shift value for converting grid space to real space
	halfSize = 1 << (shift - 1);																									//	compute the half size of the cell
	overSize = R(1.0) / Real (1 << shift);																				//	compute the scale factor for converting world space to grid space
}																																								//	end

//------------------------------------------------------------------------------
Planner::~Planner (void)																												//	destructor
{																																								//	begin
	delete[] grid[0];																															//	free the grid memory
	delete[] grid;																																//	free the grid indexing array
	delete queue;																																	//	free the priority queue
}																																								//	end

//------------------------------------------------------------------------------
void	Planner::StartPlan (CPoint startPt, CPoint stopPt)												//	start up a new plan
{																																								//	begin
	Clear ();																																			//	clear the planner
	start = IVec_2d (startPt * overSize);																					//	compute the start point in grid space
	stop = IVec_2d (stopPt * overSize);																						//	compute the stop point in grid space
	if ((GetCell (stop) == empty) && (GetCell (start) == empty))									//	if the origin and target cells are not occupied
	{																																							//	begin
		SetCell (stop, stop);																												//	set the stop point value to itself
		queue->Insert (new PlanNode (ZERO, stop));																	//	queue up the target cell for the search
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
Bool	Planner::Plan (void)																											//	incremental plan
{																																								//	begin
	PlanNode *node = (PlanNode *) queue->Peek ();																	//	look at the queue
	if (node)																																			//	if there is a node to pull
	{																																							//	begin
		if (node->cell == start)																										//	if the current cell is the start cell
			return FALSE;																															//	return false to indicate that there is nothing left to do
		queue->Remove (node);																												//	remove the node
		for (int i = 0; i < 4; i++)																									//	loop over the four potential moves from here
		{																																						//	begin
			IVec_2d	next = node->cell + moves[i];																			//	compute the next cell location to test
			if (GetCell (next) == empty)																							//	if that cell is empty
			{																																					//	begin
				SetCell (next, node->cell);																							//	make it reference back to me
				queue->Insert (new PlanNode (Weight (next), next));											//	queue it up for planning
			}																																					//	end
		}																																						//	end
		delete node;																																//	free the node memory
		return TRUE;																																//	return true to indicate that there is still work to do
	}																																							//	end
	return FALSE;																																	//	no path was available, return false to terminate search
}																																								//	end

//------------------------------------------------------------------------------
Path	*Planner::BuildPath (void)																								//	build a path from a completed plan
{																																								//	begin
	if ((GetCell (start) != empty) && (GetCell (start) != full))									//	if the start cell is not empty and is not a full cell
	{																																							//	begin
		int			count = 0;																													//	always start with one point
		for (IVec_2d vec (start); vec != stop; vec = GetCell (vec))									//	loop throught the planned path cells
			count++;																																	//	increment the count
		if (count)																																	//	if there are planned points in the path
		{																																						//	begin
			Point_2d	*pts = new Point_2d[count];																			//	allocate an array of points
			count = 0;																																//	reset the count
			for (vec = start; vec != stop; vec = GetCell (vec))												//	loop over all of the points in the path while incrementing the count
				pts[count++] (Real ((vec[X] << shift) + halfSize),											//	assign the x component of a path point
											Real ((vec[Y] << shift) + halfSize));											//	assign the y component of a path point
				return OptimizePts (count, pts);																				//	return the new path created from an optimal point array
			delete[] pts;																															//	release the points
		}																																						//	end
	}																																							//	end
	return 0;																																			//	return null pointer to indicate that no path was found
}																																								//	end

//------------------------------------------------------------------------------
void	Planner::Clear (void)																											//	clear the current plan
{																																								//	begin
	for (int i = 0; i < height; i++)																							//	loop over all of the rows in the grid
		for (int j = 0; j < width; j++)																							//	loop over all of the cells in the row
			if (grid[i][j] != full)																										//	if the cell is not a fully occupied cell
				grid[i][j] = empty;																											//	clear it
	queue->Reset ();																															//	reset the priority queue
}																																								//	end

//------------------------------------------------------------------------------
Real	Planner::TraceRay (CPoint pt, CVector vel)																//	trace the specified ray through the planning grid
{																																								//	begin
	Point_2d	origin (pt * overSize);																							//	compute the origin point in grid space
	Vector_2d	velocity (vel * overSize),																					//	compute the direction vector in grid space
						inter,																															//	the intersection position
						delta,																															//	the time between intersection positions
						sign (Sign (velocity[X]), Sign (velocity[Y])),											//	the quadrant projection vector
						time;																																//	the intersection time vector
	if (FABS (velocity[X]) > EPSILON)																							//	if the horizontal velocity is not too small
	{																																							//	begin
		inter[X] = FLOOR (origin[X]) + Positive (velocity[X]);											//	compute the intersection position
		delta[X] = R(1.0) / velocity[X];																						//	compute the time interval between intersections
		time[X] = (inter[X] - origin[X]) * delta[X];																//	compute the time of the current intersection
	}																																							//	end
	else																																					//	otherwise, not moving
		time[X] = INFINITY;																													//	so no intersection
	if (FABS (velocity[Y]) > EPSILON)																							//	if the vertical velocity is not too small
	{																																							//	begin
		inter[Y] = FLOOR (origin[Y]) + Positive (velocity[Y]);											//	compute the intersection position
		delta[Y] = R(1.0) / velocity[Y];																						//	compute the time interval between intersections
		time[Y] = (inter[Y] - origin[Y]) * delta[Y];																//	compute the time of the first intersection
	}																																							//	end
	else																																					//	otherwise, not movin
		time[Y] = INFINITY;																													//	so no intersection
	if ((time[X] == INFINITY) && (time[Y] == INFINITY))														//	if the ray is not moving in any direction
		return INFINITY;																														//	return no intersection
	while (1)																																			//	loop until we find an intersection
		if (time[X] < time[Y])																											//	if the horizontal intersection occurs before the vertical one
		{																																						//	begin
			int			ix = int (inter[X] + Negative (velocity[X])),											//	compute the x coordinate of the cell to check
							iy = int (origin[Y] + (time[X] * velocity[Y]));										//	compute the y coordinate of the cell to check
			if (GetCell (IVec_2d (ix, iy)) == full)																		//	if the cell is occupied
				return time[X];																													//	return the intersection time
			else																																			//	otherwise
			{																																					//	begin
				inter[X] += sign[X];																										//	get the next intersection
				time[X] += sign[X] * delta[X];																					//	compute the time of the next intersection
			}																																					//	end
		}																																						//	end
		else if (time[X] == time[Y])																								//	if the x intersection happens simultaneous with the y intersection
		{																																						//	begin
			int			ix = int (inter[X] + Negative (velocity[X])),											//	compute the x coordinate of the cell to check
							iy = int (inter[Y] + Negative (velocity[Y]));											//	compute the y coordinate of the cell to check
			if (GetCell (IVec_2d (ix, iy)) == full)																		//	if the cell is occupied
				return time[X];																													//	return the intersection time
			else																																			//	otherwise
			{																																					//	begin
				inter[X] += sign[X];																										//	get the next intersection
				time[X] += sign[X]* delta[X];																						//	compute the time of the next intersection
				inter[Y] += sign[Y];																										//	get the next intersection
				time[Y] += sign[Y] * delta[Y];																					//	compute the time of the next intersection
			}																																					//	end
		}																																						//	end
		else																																				//	otherwise, the y intersection is first
		{																																						//	begin
			int			ix = int (origin[X] + (time[Y] * velocity[X])),										//	compute the x coordinate of the cell to check
							iy = int (inter[Y] + Negative (velocity[Y]));											//	compute the y coordinate of the cell to check
			if (GetCell (IVec_2d (ix, iy)) == full)																		//	if the cell is occupied
				return time[Y];																													//	return the intersection time
			else																																			//	otherwise
			{																																					//	begin
				inter[Y] += sign[Y];																										//	get the next intersection
				time[Y] += sign[Y] * delta[Y];																					//	compute the time of the next intersection
			}																																					//	end
		}																																						//	end
}																																								//	end

//------------------------------------------------------------------------------
