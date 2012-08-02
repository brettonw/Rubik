//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	07/18/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "fade pixel sequence.h"
#include "display.h"
#include "bound.h"
#include "referee.h"

//------------------------------------------------------------------------------
FadeSeq::FadeSeq (const Pixel &pix, Real tm)																		//	constructor
{																																								//	begin
	pixel = pix;																																	//	copy the pixel value
	step = gReferee->Interval () / tm;																						//	compute the stepping factor for scaling
	scale = R(1.0);																																//	start with the first scaling factor
}																																								//	end

//------------------------------------------------------------------------------
FadeSeq::~FadeSeq (void)																												//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Bool	FadeSeq::Step (CPoint where)																							//	draw the next frame
{																																								//	begin
	int		x = int (where[X]),																											//	compute the x component of the integer coordinate of the pixel
				y = int (where[Y]);																											//	compute the y component of the integer coordinate of the pixel
	if (gBound.TestClip (x, y) == INSIDE)																					//	if the	point is in the bound
		(*gDisplay) (x, y) += pixel.Scale (scale);																	//	set the pixel
	scale -= step;																																//	decrement the scale
	return (scale > ZERO) ? TRUE : FALSE;																					//	return whether or not the sequence is still in operation
}																																								//	end

//------------------------------------------------------------------------------
void	FadeSeq::Reset (void)																											//	return the sequence to its starting state
{																																								//	begin
	scale = R(1.0);																																//	start with the first scaling factor
}																																								//	end

//------------------------------------------------------------------------------
