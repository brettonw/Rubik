//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/12/96
//
//------------------------------------------------------------------------------

#ifndef	POINTER
#define	POINTER

//------------------------------------------------------------------------------
#include "space.h"

//------------------------------------------------------------------------------
class Pointer																																		//	user input pointer class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
static	Point_2d		mid;																												//	center point of the screen
	public:																																				//	members available externally
				Pointer (void);																													//	constructor
virtual	~Pointer (void);																												//	destructor
virtual	void				Update (void) = 0;																					//	update the pointer status
virtual	Vector_2d		GetPos (void) = 0;																					//	get the pointer position
virtual	Bool				Button (int button) = 0;																		//	get a button status
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	POINTER