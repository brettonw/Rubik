//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	12/23/96
//
//------------------------------------------------------------------------------

#ifndef	POINTER_DEVICE
#define	POINTER_DEVICE

//------------------------------------------------------------------------------
#include "space.h"

//------------------------------------------------------------------------------
class PointerDevice																															//	user input pointer class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
static	Point_2d		mid;																												//	center point of the screen
				int					update;																											//	the update counter
				int					stamp;																											//	the time of the last change
				void				TimeStamp (void);																						//	set the time stamp for a change
	public:																																				//	members available externally
				PointerDevice (void);																										//	constructor
virtual	~PointerDevice (void);																									//	destructor
virtual	Bool				Update (int count);																					//	update the pointer status
virtual	Vector_2d		GetPos (void) = 0;																					//	get the pointer position
				int					GetTimeStamp (void);																				//	return the time stamp of the last update
};																																							//	end

//------------------------------------------------------------------------------
inline	void	PointerDevice::TimeStamp (void)																		//	set the time stamp for a change
{																																								//	begin
	stamp = GetTickCount ();																											//	get the current time
}																																								//	end

//------------------------------------------------------------------------------
inline	int		PointerDevice::GetTimeStamp (void)																//	return the time stamp of the last update
{																																								//	begin
	return stamp;																																	//	return the time stamp
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	POINTER_DEVICE