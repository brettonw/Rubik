//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	07/18/96
//
//------------------------------------------------------------------------------

#ifndef	FADE_PIXEL_SEQUENCE
#define	FADE_PIXEL_SEQUENCE

//------------------------------------------------------------------------------
#include "sequence.h"
#include "pixel.h"

//------------------------------------------------------------------------------
class FadeSeq : public Sequence																									//	a fading pixel sequence
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Pixel			pixel;																												//	the pixel to draw
				Real			scale;																												//	pixel scale factor
				Real			step;																													//	elapsed time since first step
	public:																																				//	members available externally
				FadeSeq (const Pixel &pixel, Real time = R(1.0));												//	constructor
virtual	~FadeSeq (void);																												//	destructor
virtual	Bool			Step (CPoint where);																					//	draw the next frame
virtual void			Reset	(void);																									//	return the sequence to its starting state
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	FADE_PIXEL_SEQUENCE