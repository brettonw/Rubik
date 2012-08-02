//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	12/23/96
//
//------------------------------------------------------------------------------

#ifndef	MOUSE_POINTER
#define	MOUSE_POINTER

//------------------------------------------------------------------------------
#include "pointer device.h"

//------------------------------------------------------------------------------
class MousePointer : public PointerDevice																				//	mouse input class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				tagPOINT	where;																												//	the xy location of the mouse
				RECT			oldClip;																											//	the old clipping coordinates of the mouse
				int				oldScale[3];																									//	the old mouse scaling
	public:																																				//	members available externally
				MousePointer (int t1 = 4, int t2 = 0, int s = 1);												//	constructor
				~MousePointer (void);																										//	destructor
virtual	Bool				Update (int count);																					//	update the pointer status
virtual	Vector_2d		GetPos (void);																							//	get the pointer position
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	MOUSE_POINTER