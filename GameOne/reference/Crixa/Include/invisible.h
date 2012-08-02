//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	05/13/96
//
//------------------------------------------------------------------------------

#ifndef	INVISIBLE
#define	INVISIBLE

//------------------------------------------------------------------------------
#include "visual.h"

//------------------------------------------------------------------------------
class Invisible : public Visual																									//	invisible object
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
				Invisible (Body *body) : Visual (body) {}																//	constructor
virtual	~Invisible (void);																											//	destructor
virtual	void			Draw (CPoint where);																					//	draw the object
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	INVISIBLE