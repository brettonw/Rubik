//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	04/25/96
//
//	02/24/97	DS		Added SetMode function
//
//------------------------------------------------------------------------------

#ifndef	VISUAL
#define	VISUAL

//------------------------------------------------------------------------------
#include "body.h"

//------------------------------------------------------------------------------
enum { VISNORMAL = 0, VISFLASH = 1, VISADD = 2 };																//	drawing mode for visual

//------------------------------------------------------------------------------
class Visual																																		//	visual effect for a body
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Body			*body;																												//	pointer to the body that defines the location of the visual
				int				mode;																													//	drawing mode to use
				int				lastmode;																											//	check for last mode
	public:																																				//	members available externally
				Visual (Body *body, int mode = VISNORMAL);															//	constructor
virtual	~Visual (void);																													//	destructor
virtual	void			Draw (CPoint where) = 0;																			//	draw the object
				void			SetMode (int num) { mode = num; }															//	set the drawing mode
};																																							//	end

//------------------------------------------------------------------------------
#endif	//	VISUAL