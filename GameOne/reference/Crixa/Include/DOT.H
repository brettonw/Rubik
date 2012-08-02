//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/02/96
//
//------------------------------------------------------------------------------

#ifndef	DOT
#define	DOT

//------------------------------------------------------------------------------
#include "visual.h"
#include "pixel.h"

//------------------------------------------------------------------------------
class Dot : public Visual																												//	a dot on the screen
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Pixel			color;																												//	the color of the dot
				Pixel			fadedColor;																										//	the color of the dot faded a bit
				Vector_2d	last[2];																											//	the last two locations of the dot
	public:																																				//	members available externally
				Dot (Body *body, Pixel color);																					//	constructor
virtual	~Dot (void);																														//	destructor
virtual	void			Draw (CPoint where);																					//	draw the object
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	DOT