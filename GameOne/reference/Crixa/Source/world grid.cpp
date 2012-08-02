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
#include "world grid.h"
#include "quadratic.h"
#include "math utility.h"
#include "level file.h"
#include "text buffer.h"

//------------------------------------------------------------------------------
inline	WGrid::Cell	WGrid::GetCell (int x, int y)																//	return the specified cell
{																																								//	begin
	if ((x >= 0) && (x < width) && (y >= 0) && (y < height))											//	if the request is in the playable grid
		return grid[y][x];																													//	return the cell
	else																																					//	otherwise
		return border;																															//	return the border space
}																																								//	end

//------------------------------------------------------------------------------
WGrid::WGrid (void)																															//	constructor
{																																								//	begin
	LevFile		lev ("Grid");
	TxtBuffer	txt (&lev);																													//	load the grid file
	txt >> width >> height;																												//	get the width and height
	grid = new Cell*[height];																											//	allocate the indexing array
	grid[0] = new Cell[height * width];																						//	allocate the grid map
	for (int i = 0; i < height; i++)																							//	loop over all of the rows
		grid[i] = grid[0] + (i * width);																						//	set the row pointer
	for (int y = 0; y < height; y++)																							//	loop over all of the rows
	{																																							//	begin
		cstr	line;																																	//	place to store a line pointer
		txt >> line;																																//	get the line from the grid file
		for (int x = 0; x < width; x++)																							//	loop over all of the columns in this row
		{																																						//	begin
			grid[y][x].occupied = (line[x] - '0') & 0x01;															//	extract the occupied data
			grid[y][x].convex = ((line[x] - '0') & 0x02) >> 1;												//	extract the convex data
		}																																						//	end
	}																																							//	end
	border.convex = 0;																														//	the border cell is not a convex corner
	border.occupied = 1;																													//	the border cell is always occupied
}																																								//	end

//------------------------------------------------------------------------------
WGrid::~WGrid (void)																														//	destructor
{																																								//	begin
	delete[] grid[0];																															//	release the grid map
	delete[] grid;																																//	release the indexing array
}																																								//	end

//------------------------------------------------------------------------------
Real	WGrid::TraceRay (CPoint pt, CVector vel)																	//	trace a ray through the grid
{																																								//	begin
	Point_2d	origin (pt * OVER_CELL_SIZE);																				//	compute the origin point in grid space
	Vector_2d	velocity (vel * OVER_CELL_SIZE),																		//	compute the direction vector in grid space
						inter,
						delta,
						sign (Sign (velocity[X]), Sign (velocity[Y])),
						time;
	if (FABS (velocity[X]) > EPSILON)
	{
		inter[X] = FLOOR (origin[X]) + Positive (velocity[X]);
		delta[X] = R(1.0) / velocity[X];
		time[X] = (inter[X] - origin[X]) * delta[X];
	}
	else
		time[X] = INFINITY;
	if (FABS (velocity[Y]) > EPSILON)
	{
		inter[Y] = FLOOR (origin[Y]) + Positive (velocity[Y]);
		delta[Y] = R(1.0) / velocity[Y];
		time[Y] = (inter[Y] - origin[Y]) * delta[Y];
	}
	else
		time[Y] = INFINITY;
	if ((time[X] == INFINITY) && (time[Y] == INFINITY))
		return INFINITY;
	if (GetCell (int (origin[X]), int (origin[Y])).occupied)
		return ZERO;
	while (1)
		if (time[X] < time[Y])
		{
			Real	y = origin[Y] + (time[X] * velocity[Y]);
			int		ix = int (inter[X] + Negative (velocity[X])),
						iy = int (y);
			if (GetCell (ix, iy).occupied)
				return time[X];
			else
			{
				inter[X] += sign [X];
				time[X] += sign[X] * delta[X];
			}
		}
		else if (time[X] == time[Y])
		{
			int		ix = int (inter[X] + Negative (velocity[X])),
						iy = int (inter[Y] + Negative (velocity[Y]));
			if (GetCell (ix, iy).occupied)
				return time[X];
			else
			{
				inter[X] += sign[X];
				time[X] += sign[X]* delta[X];
				inter[Y] += sign[Y];
				time[Y] += sign[Y] * delta[Y];
			}
		}
		else
		{
			Real	x = origin[X] + (time[Y] * velocity[X]);
			int		ix = int (x),
						iy = int (inter[Y] + Negative (velocity[Y]));
			if (GetCell (ix, iy).occupied)
				return time[Y];
			else
			{
				inter[Y] += sign[Y];
				time[Y] += sign[Y] * delta[Y];
			}
		}
}																																								//	end

//------------------------------------------------------------------------------
Real	WGrid::Intersect (CPoint pt, CVector vel, Real radius, Sect *sect)				//	return the nearest intersection time for both edges and corners
{																																								//	begin
	Real	time = Edges (pt, vel, radius, sect);																		//	compute the edge intersection time
	if (time < INFINITY)																													//	if the object is not stationary
		return Corners (pt, vel, radius * R(1.1), sect, time);											//	return the result of intersecting with the corners
	return time;																																	//	otherwise, just return the time
}																																								//	end

//------------------------------------------------------------------------------
Real	WGrid::Edges (CPoint pt, CVector v, Real r, Sect *sect)										//	return the nearest intersection time for edges only
{																																								//	begin
#if	1
	Point_2d	ptA = pt + Vector_2d (r * Sign (v[X]), ZERO),
						ptB = pt + Vector_2d (ZERO, r * Sign (v[Y]));
	Real			timeA = (Sign (v[X]) == ZERO) ? INFINITY : TraceRay (ptA, v),
						timeB = (Sign (v[Y]) == ZERO) ? INFINITY : TraceRay (ptB, v);
	if (timeA < timeB)
	{
		sect->plane (Sign (-v[X]), ZERO);																						//	set the plane of intersection
		sect->point = ptA + (timeA * v);																						//	compute the intersection point
		return timeA;
	}
	else
	{
		sect->plane (ZERO, Sign (-v[Y]));																						//	set the plane of intersection
		sect->point = ptB + (timeB * v);																						//	compute the intersection point
		return timeB;
	}
#else
	Real			rad = -r * OVER_CELL_SIZE;																					//	compute the radius in grid space
	Vector_2d	vel = v * OVER_CELL_SIZE;																						//	compute the velocity in grid space
	Point_2d	pos = pt * OVER_CELL_SIZE;																					//	compute the position in grid space
	Vector_2d	radius,																															//	how to offset lines for intersection
						delta,																															//	how much the time changes to the next line crossing
						Xp,																																	//	the horizontal lines to cross
						Yp,																																	//	the vertical lines to cross
						time;																																//	the times to the lines
	if (FABS (vel[X]) < EPSILON)																									//	if the object is not moving appreciably along the x axis
		time[X] = INFINITY;																													//	don't even test for intersections in the x direction
	else																																					//	otherwise
	{																																							//	begin
		radius[X] = rad * Sign (vel[X]);																						//	compute the radius offset
		Xp[X] = FLOOR (pos[X]) + Positive (vel[X]);																	//	compute the first line to cross
		//Xp[X] = FLOOR (pos[X]) - Negative (vel[X]);																	//	compute the first line to cross
		time[X] = (Xp[X] + radius[X] - pos[X]) / vel[X];														//	compute the time of crossing the first line
		delta[X] = Sign (vel[X]) / vel[X];																					//	compute the delta time between line crossings
		while (time[X] < ZERO)																											//	while the time is less than zero
		{																																						//	begin
			Xp[X] += Sign (vel[X]);																										//	advance to the next vertical line
			time[X] += delta[X];																											//	advance the time
		}																																						//	end
		Xp[Y] = pos[Y] + (time[X] * vel[Y]);																				//	compute the y coordinate of the current intersection
	}																																							//	end
	if (FABS (vel[Y]) < EPSILON)																									//	if the object is not moving appreciably along the y axis
	{																																							//	begin
		if (time[X] == INFINITY)																										//	if the object is essentially not moving
			return INFINITY;																													//	it will never intersect the grid
		time[Y] = INFINITY;																													//	don't even test for intersections in the y direction
	}																																							//	end
	else																																					//	otherwise
	{																																							//	begin
		radius[Y] = rad * Sign (vel[Y]);																						//	compute the radius offset
		Yp[Y] = FLOOR (pos[Y]) + Positive (vel[Y]);																	//	compute the first line to cross
		//Yp[Y] = FLOOR (pos[Y]) - Negative (vel[Y]);																	//	compute the first line to cross
		time[Y] = (Yp[Y] + radius[Y] - pos[Y]) / vel[Y];														//	compute the time of crossing
		delta[Y] = Sign (vel[Y]) / vel[Y];																					//	compute the delta time between line crossings
		while (time[Y] < ZERO)																											//	while the time is less than zero
		{																																						//	begin
			Yp[Y] += Sign (vel[Y]);																										//	advance to the next horizontal line
			time[Y] += delta[Y];																											//	advance the time
		}																																						//	end
		Yp[X] = pos[X] + (time[Y] * vel[X]);																				//	compute the x coordinate of the current intersection
	}																																							//	end
	while (TRUE)																																	//	loop until an intersection is found
		if (time[X] <= time[Y])																											//	if the vertical crossing happens before the horizontal crossing
		{																																						//	begin
			if (GetCell (int (Xp[X] + Negative (vel[X])), int (Xp[Y])).occupied)			//	if the cell we are travelling into is occupied
			{																																					//	begin
				sect->plane (Sign (-vel[X]), ZERO);																			//	set the plane of intersection
				sect->point = (pt + (time[X] * v)) - (r * sect->plane);									//	compute the intersection point
				return time[X];																													//	return the intersection time
			}																																					//	end
			Xp[X] += Sign (vel[X]);																										//	advance to the next vertical line
			time[X] += delta[X];																											//	advance the time
			Xp[Y] = pos[Y] + (time[X] * vel[Y]);																			//	compute the y coordinate
		}																																						//	end
		else																																				//	otherwise, the horizontal crossing happens before the vertical crossing
		{																																						//	begin
			if (GetCell (int (Yp[X]), int (Yp[Y] + Negative (vel[Y]))).occupied)			//	if the cell we are travelling into is occupied
			{																																					//	begin
				sect->plane (ZERO, Sign (-vel[Y]));																			//	set the plane of intersection
				sect->point = (pt + (time[Y] * v)) - (r * sect->plane);									//	compute the intersection point
				return time[Y];																													//	return the intersection time
			}																																					//	end
			Yp[Y] += Sign (vel[Y]);																										//	advance to the next horizontal line
			time[Y] += delta[Y];																											//	advance the time
			Yp[X] = pos[X] + (time[Y] * vel[X]);																			//	compute the x coordinate
		}																																						//	end
#endif
}																																								//	end

//------------------------------------------------------------------------------
Real	WGrid::Corners (CPoint pt, CVector v, Real r, Sect *sect, Real limit)			//	return the nearest intersection time for corners only
{																																								//	begin
	Real			delta,																															//	the time between crossings
						time,																																//	the current time
						itime = limit,																											//	the intersection time, starts at the limit time
						radius = (r * OVER_CELL_SIZE) * R(1.5),															//	compute the radius in grid space (multiplied by an approximation to SQRT[2] to cover angles)
						root, root2;																												//	roots for the circle intersections
	Vector_2d	velocity = v * OVER_CELL_SIZE;																			//	compute the velocity in grid space
	Point_2d	position = pt * OVER_CELL_SIZE;																			//	compute the position in grid space
	limit *= R(2.0);
	if (FABS (velocity[X]) > FABS (velocity[Y]))																	//	if the dominant motion is along the x axis
	{																																							//	begin
		Vector_2d	Xp;																																//	the horizontal lines to cross
		Xp[X] = FLOOR (position[X]) - Negative (velocity[X]);												//	compute the first line to cross
		time = (Xp[X] - position[X]) / velocity[X];																	//	compute the time of crossing the first line
		delta = Sign (velocity[X]) / velocity[X];																		//	compute the delta time between line crossings
		Xp[Y] = position[Y] + (time * velocity[Y]);																	//	compute the y coordinate of the current intersection
		while (time < limit)																												//	loop until all of the row crossings within a limited time span are crossed
		{																																						//	begin
			int		y_start = int (CEIL (Xp[Y] - radius)),															//	compute the starting point in the vertical loop
						y_stop = int (FLOOR (Xp[Y] + radius)),															//	compute the stopping point in the vertical loop
						x = int (Xp[X]);																										//	pre-integerize the x location
			for (int y = y_start; y <= y_stop; y++)																		//	loop over all of the vertices in the vertical loop
				if (GetCell (x, y).convex)																							//	if the cell we are travelling into is a convex corner
				{																																				//	begin
					Point_2d	circle (Real (x << CELL_SHIFT), Real (y << CELL_SHIFT));		//	set up the origin of the circle to test for intersection
					if (Dist (pt - circle, v, r, root, root2))														//	if there is an intersection with the circle
					{																																			//	begin
						if (root < ZERO)																										//	if the intersection has already occurred
							if (root2 > -root)																								//	if the object has not penetrated more than half way
								root = ZERO;																										//	indicate a current intersection
							else																															//	otherwise
								continue;																												//	no intersection
						if (root < itime)																										//	if the intersection is before any previously found intersection
						{																																		//	begin
							itime = root;																											//	save the intersection time
							sect->plane = ((pt + (root * v)) - circle);												//	set the plane of intersection
							sect->plane.Normalize ();																					//	make the plane unit length
							sect->point = circle;																							//	set the point of intersection
						}																																		//	end
					}																																			//	end
				}																																				//	end
			Xp[X] += Sign (velocity[X]);																							//	advance to the next vertical line
			time += delta;																														//	advance the time
			Xp[Y] = position[Y] + (time * velocity[Y]);																//	compute the y coordinate
		}																																						//	end
		return itime;																																//	return the intersection time
	}																																							//	end
	else																																					//	otherwise the dominant motion is along the y axis
	{																																							//	begin
		Vector_2d	Yp;																																//	the vertical lines to cross
		Yp[Y] = FLOOR (position[Y]) - Negative (velocity[Y]);												//	compute the first line to cross
		time = (Yp[Y] - position[Y]) / velocity[Y];																	//	compute the time of crossing
		delta = Sign (velocity[Y]) / velocity[Y];																		//	compute the delta time between line crossings
		Yp[X] = position[X] + (time * velocity[X]);																	//	compute the x coordinate of the current intersection
		while (time < limit)																												//	loop until the time has surpassed the limit
		{																																						//	begin
			int		x_start = int (CEIL (Yp[X] - radius)),															//	compute the starting point in the horizontal loop
						x_stop = int (FLOOR (Yp[X] + radius)),															//	compute the stopping point in the horizontal loop
						y = int (Yp[Y]);																										//	pre-integerize the y location
			for (int x = x_start; x <= x_stop; x++)																		//	loop over all of the vertices in the horizontal loop
				if (GetCell (x, y).convex)																							//	if the cell we are travelling into is a convex corner
				{																																				//	begin
					Point_2d	circle (Real (x << CELL_SHIFT), Real (y << CELL_SHIFT));		//	set up the origin of the circle
					if (Dist (pt - circle, v, r, root, root2))														//	if there is an intersection
					{																																			//	begin
						if (root < ZERO)																										//	if the intersection has already occurred
							if (root2 > -root)																								//	if the object has not penetrated more than half way
								root = ZERO;																										//	indicate a current intersection
							else																															//	otherwise
								continue;																												//	no intersection
						if (root < itime)																										//	if the intersection occurs before any other we have found
						{																																		//	begin
							itime = root;																											//	save the intersection time
							sect->plane = ((pt + (root * v)) - circle);												//	set the plane of intersection
							sect->plane.Normalize ();																					//	make the plane unit length
							sect->point = circle;																							//	set the point of intersection
						}																																		//	end
					}																																			//	end
				}																																				//	end
			Yp[Y] += Sign (velocity[Y]);																							//	advance to the next horizontal line
			time += delta;																														//	advance the time
			Yp[X] = position[X] + (time * velocity[X]);																//	compute the x coordinate
		}																																						//	end
		return itime;																																//	return the collision time
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
