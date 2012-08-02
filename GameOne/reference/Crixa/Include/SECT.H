//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/05/96
//
//------------------------------------------------------------------------------

#ifndef	SECT
#define	SECT

//------------------------------------------------------------------------------
#include "space.h"

//------------------------------------------------------------------------------
class Sect																																			//	intersection data
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
				Vector_2d	plane;																												//	normal to the intersection plane
				Point_2d	point;																												//	point of intersection
				int				data;																													//	any other data we may want to attach to the record
	public:																																				//	members available externally
				Sect (void) {}																													//	constructor
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	SECT
