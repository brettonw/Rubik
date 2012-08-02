//----------------------------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------

#ifdef	_MSC_VER
#include "precompile.h"
#else
#include <timer.h>
#endif

//----------------------------------------------------------------------------------------------------
#include "timer.h"

//----------------------------------------------------------------------------------------------------
Timer::Timer (void)																																										//	constructor
{																																																			//	begin
#ifdef	_MSC_VER
	__int64	freq;																																												//	place to fetch the timer frequency
	QueryPerformanceFrequency ((LARGE_INTEGER *) &freq);																								//	fetch the timer frequency
	frequency = 1.0 / (double) freq;																																		//	store the timer frequency as a reciprocal
	start = 0;																																													//	zero the timer
#else
	start.hi = 0; start.lo = 0;																																					//	zero the timer
#endif
}																																																			//	end

//----------------------------------------------------------------------------------------------------
void	Timer::Start (void)																																							//	start the timer
{																																																			//	begin
#ifdef	_MSC_VER
	QueryPerformanceCounter ((LARGE_INTEGER *) &start);																									//	get the start time
#else
	Microseconds (&start);																																							//	get the start time
#endif
}																																																			//	end

//----------------------------------------------------------------------------------------------------
double	Timer::Elapsed (void)																																					//	report the time so far
{																																																			//	begin
#ifdef	_MSC_VER
	__int64	stop;																																												//	place to fetch the stop time
	QueryPerformanceCounter ((LARGE_INTEGER *) &stop);																									//	get the stop time
	double	elapsed = (stop - start) * frequency;																												//	compute the elapsed time
#else
	UnsignedWide	stop;																																									//	the stop time
	Microseconds (&stop);																																								//	get the stop time
	double	elapsed = ((stop.lo-start.lo) + (double) 0x10000 * 0x10000 * (stop.hi - start.hi)) * 1.0e-6;//	compute the elapsed time in millionths of a second
	elapsed *= 1.0e-6;																																									//	multiply the elapsed time by one millionth
#endif
	return elapsed;																																											//	return the elapsed time in seconds
}																																																			//	end

//----------------------------------------------------------------------------------------------------
double	Timer::Interval (void)																																				//	report the time so far and reset the timer
{																																																			//	begin
#ifdef	_MSC_VER
	__int64	stop;																																												//	place to fetch the stop time
	QueryPerformanceCounter ((LARGE_INTEGER *) &stop);																									//	get the stop time
	double	elapsed = (stop - start) * frequency;																												//	compute the elapsed time
#else
	UnsignedWide	stop;																																									//	the stop time
	Microseconds (&stop);																																								//	get the stop time
	double	elapsed = ((stop.lo-start.lo) + (double) 0x10000 * 0x10000 * (stop.hi - start.hi)) * 1.0e-6;//	compute the elapsed time in millionths of a second
#endif
	start = stop;																																												//	rekey the start time
	return elapsed;																																											//	return the elapsed time in seconds
}																																																			//	end

//----------------------------------------------------------------------------------------------------
