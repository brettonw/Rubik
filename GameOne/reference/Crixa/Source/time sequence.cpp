//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/14/97
//
//	02/24/97	DS		Added AltDrawMode
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "time sequence.h"
#include "referee.h"

//------------------------------------------------------------------------------
TimeSeq::TimeSeq (SArrayPtr sprts, Bool lp, Real t) : SpriteSeq (sprts)					//	constructor
{																																								//	begin
	loop = lp;																																		//	copy the desired loop behavior
	count = sprites->Count ();																										//	get the count from the sprite array
	frames = new Frame[count];																										//	allocate the frames array
	for (int i = 0; i < count; i++)																								//	loop over all of the frames
	{																																							//	begin
		frames[i].sprite = i;																												//	set the frame sprite
		frames[i].time = t;																													//	set the frame time
	}																																							//	end
	time = ZERO;																																	//	the display time is zero
	frame = -1;																																		//	the frame is set to the beginning
}																																								//	end

//------------------------------------------------------------------------------
TimeSeq::TimeSeq (SArrayPtr sprts, Bool lp, int c, ...) : SpriteSeq (sprts)			//	constructor
{																																								//	begin
	loop = lp;																																		//	copy the desired loop behavior
	count = c;																																		//	copy the frame count
	frames = new Frame[count];																										//	allocate the frames array
	va_list	var;																																	//	variable argument list pointer
	va_start (var, c);																														//	start with the first variablce argument
	for (int i = 0; i < count; i++)																								//	loop over all of the frames
	{																																							//	begin
		frames[i].sprite = va_arg (var, int);																				//	set the frame sprite
		frames[i].time = Real (va_arg (var, double));																//	set the frame time
	}																																							//	end
	va_end (var);																																	//	cleanup the variable argument list
	time = ZERO;																																	//	the display time is zero
	frame = -1;																																		//	the frame is set to the beginning
}																																								//	end

//------------------------------------------------------------------------------
TimeSeq::~TimeSeq (void)																												//	destructor
{																																								//	begin
	delete[] frames;																															//	free up the frames array
}																																								//	end

//------------------------------------------------------------------------------
Bool	TimeSeq::Step (CPoint where)																							//	draw the next frame
{																																								//	begin
	while (time <= ZERO)																													//	while the time for this frame has expired
	{																																							//	begin
		if (++frame >= count)																												//	if the frame counter has gone past the end of the sequence
			if (!loop)																																//	if the sequence is not looping
				return FALSE;																														//	return false to signify the end of the sequence
			else																																			//	otherwise the sequence is looping
				frame = 0;																															//	start again at the beginning
		SetCurrent (frames[frame].sprite);																					//	set the current sprite
		time += frames[frame].time;																									//	set the time until this frame expires
		if (frames[frame].time <= ZERO)																							//	a frame time of zero is intended to be displayed once regardless of elapsed time
			break;																																		//	so end the loop if that happens
	}																																							//	end
	time -= gReferee->Interval ();																								//	subtract the update time from the frame time
	return SpriteSeq::Step (where);																								//	do the parent thing and return the result
}																																								//	end

//------------------------------------------------------------------------------
Bool	TimeSeq::Step (CPoint where, int mode)																		//	draw the next frame
{																																								//	begin
	while (time <= ZERO)																													//	while the time for this frame has expired
	{																																							//	begin
		if (++frame >= count)																												//	if the frame counter has gone past the end of the sequence
			if (!loop)																																//	if the sequence is not looping
				return FALSE;																														//	return false to signify the end of the sequence
			else																																			//	otherwise the sequence is looping
				frame = 0;																															//	start again at the beginning
		SetCurrent (frames[frame].sprite);																					//	set the current sprite
		time += frames[frame].time;																									//	set the time until this frame expires
		if (frames[frame].time <= ZERO)																							//	a frame time of zero is intended to be displayed once regardless of elapsed time
			break;																																		//	so end the loop if that happens
	}																																							//	end
	time -= gReferee->Interval ();																								//	subtract the update time from the frame time
	return SpriteSeq::Step (where,mode);																					//	do the parent thing and return the result
}																																								//	end

//------------------------------------------------------------------------------
void	TimeSeq::Reset (void)																											//	return the sequence to its starting state
{																																								//	begin
	time = ZERO;																																	//	the display time is zero
	frame = -1;																																		//	the frame is set to the beginning
}																																								//	end

//------------------------------------------------------------------------------
