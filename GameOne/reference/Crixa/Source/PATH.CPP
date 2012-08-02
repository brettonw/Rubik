//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	07/30/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "path.h"

//------------------------------------------------------------------------------
Path::Path (Bool lp, int cnt, ...)																							//	constructor
{																																								//	begin
	count = cnt;																																	//	save the point count
	point = new Point_2d[count];																									//	allocate a new point array
	va_list var;																																	//	a variable argument list
	va_start (var, cnt);																													//	start the variable argument list after the count
	for (int i = 0; i < count; i++)																								//	loop over all of the points
		point[i] = va_arg (var, Point_2d);																					//	copying them from the argument list
	va_end (var);																																	//	end the variable argument list
	loop = lp;																																		//	save the loop flag
	current = 0;																																	//	start at the beginning
}																																								//	end

//------------------------------------------------------------------------------
Path::Path (Bool lp, int cnt, Point_2d *pts)																		//	constructor
{																																								//	begin
	count = cnt;																																	//	save the point count
	point = new Point_2d[count];																									//	allocate a new point array
	for (int i = 0; i < count; i++)																								//	loop over all of the points
		point[i] = pts[i];																													//	copying the points from the parameter array
	loop = lp;																																		//	save the loop flag
	current = 0;																																	//	start at the beginning
}																																								//	end

//------------------------------------------------------------------------------
Path::~Path (void)																															//	destructor
{																																								//	begin
	delete[] point;																																//	release the point array
}																																								//	end

//------------------------------------------------------------------------------
Bool	Path::Previous (void)																											//	step back to the previous point
{																																								//	begin
	current--;																																		//	decrement the current index
	if (current == -1)																														//	if it has dropped off the end of the array
		if (loop)																																		//	if the loop flag is set
			current += count;																													//	start again at the top
		else																																				//	otherwise
			return FALSE;																															//	return false to indicate the sequence termination
	return TRUE;																																	//	return true to indicate the path continues
}																																								//	end

//------------------------------------------------------------------------------
Bool	Path::Next (void)																													//	step forward to the next point
{																																								//	begin
	current++;																																		//	increment the current index
	if (current == count)																													//	if it has dropped off the end of the array
		if (loop)																																		//	if the loop flag is set
			current  = 0;																															//	start again at the bottom
		else																																				//	otherwise
			return FALSE;																															//	return false to indicate the sequence termination
	return TRUE;																																	//	return true to indicate the path continues
}																																								//	end

//------------------------------------------------------------------------------
