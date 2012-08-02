//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/29/96
//
//------------------------------------------------------------------------------

#ifndef	STAR_SEQUENCE
#define	STAR_SEQUENCE

//------------------------------------------------------------------------------
#include "sequence.h"

//------------------------------------------------------------------------------
class StarSeq : public Sequence																									//	starfield sequence
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				int				width;																												//	width of the star array
				int				height;																												//	height of the star array
				Point_2d	**star;																												//	star array
				Real			parallax;																											//	how much parallax compression to use
				Real			density;																											//	the size of the star grid
				Real			brightness;																										//	the brightness of each star in the grid
	public:																																				//	members available externally
				StarSeq (Real x, Real y, Real parallax, Real density, Real brightness);	//	constructor
virtual	~StarSeq (void);																												//	destructor
virtual	Bool			Step (CPoint where);																					//	draw the next frame
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	STAR_SEQUENCE