//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Modified by:		Bretton Wade
//	Last Revision:	07/16/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "moving sequence.h"
#include "referee.h"

//------------------------------------------------------------------------------
MoveSeq::MoveSeq (SeqPtr seq, CPoint loc, Bool lp, int c, ...)									//	constructor
{																																								//	begin
	sequence = seq;																																//	save the sequence to play
	count = c;																																		//	set the number of frames
	loop = lp;																																		//	copy the loop flag
	location = loc;																																//	copy the starting location
	frames = new Frame[count];																										//	create the frame array
	va_list var;																																	//	a variable argument list
	va_start (var, c);																														//	start the variable argument list after the count
	if (loop)																																			//	if the loop flag is set
	{																																							//	begin
		for (int i = 0; i < count; i++)																							//	loop for the number of arguments
		{																																						//	begin
			frames[i].velocity = va_arg (var, Vector_2d);															//	get a velocity vector from the argument list
			frames[i].time = Real (va_arg (var, double));															//	get the time off the var arg list
		}																																						//	end
		for (i = 0; i < count; i++)																									//	loop for the number of arguments
		{																																						//	begin
			int	next = i + 1;																													//	compute the index of the next waypoint in the list
			if (next == count)																												//	if the next one is the last one
					next = 0;																															//	wrap around to the beginning
			frames[i].delta = frames[next].velocity - frames[i].velocity;							//	compute the delta vector
		}																																						//	end
	}																																							//	end
	else																																					//	otherwise
	{																																							//	begin
		int	max = count - 1;																												//	figure the index of the last velocity
		for (int i = 0; i < max; i++)																								//	loop over all of the velocities
		{																																						//	begin
			frames[i].velocity = va_arg (var, Vector_2d);															//	get a velocity vector from the argument list
			frames[i].time = Real (va_arg (var, double));															//	get the time off the var arg list
		}																																						//	end
		frames[max].velocity = va_arg (var, Vector_2d);															//	get the last velocity vector from the argument list
		frames[max].time = R(1.0);																									//	set the last frame time to some useless value
		frames[max].delta (ZERO, ZERO);																							//	zero the last delta vector
		for (i = 0; i < max; i++)																										//	loop for the number of arguments
		{																																						//	begin
			int	next = i + 1;																													//	compute the index of the next waypoint in the list
			frames[i].delta = frames[next].velocity - frames[i].velocity;							//	compute the delta vector
		}																																						//	end
	}																																							//	end
	va_end (var);																																	//	end the variable argument list
	time = ZERO;																																	//	start the time at zero
	frame = -1;																																		//	just before the 0th frame
}																																								//	end

//------------------------------------------------------------------------------
MoveSeq::~MoveSeq (void)																												//	destructor
{																																								//	begin
	delete[] frames;																															//	delete the frame data
}																																								//	end

//------------------------------------------------------------------------------
Bool	MoveSeq::Step (CPoint where)																							//	step to the next event
{																																								//	begin
	if (time <= ZERO)																															//	if we used up the time for this frame
	{																																							//	begin
		frame++;																																		//	advance the frame
		if (frame == count)																													//	if the frame is past the end of the sequence
			if (loop)																																	//	if the loop flag is set
				frame = 0;																															//	start over at the beginning of the sequence
			else																																			//	otherwise
				frame = count - 1;																											//	stop at the end
		time += frames[frame].time;																									//	add to the current frame time
	}																																							//	end
	Real			interpolant = R(1.0) - (time / frames[frame].time);									//	compute the fraction of time that has passed for the frame
	Vector_2d	vel = frames[frame].velocity + (interpolant * frames[frame].delta);	//	compute the current location of the sequence
	location += vel * gReferee->Interval ();																			//	add the velocity to the location
	time -= gReferee->Interval ();																								//	subtract the interval's duration
	return sequence->Step (location - where);																			//	step the sequence itself
}																																								//	end

//------------------------------------------------------------------------------
void	MoveSeq::Reset (void)																											//	reset the sequence
{																																								//	begin
	frame = -1;																																		//	go back to right before the 0th frame
	time = ZERO;																																	//	and reset the timer
}																																								//	end

//------------------------------------------------------------------------------
