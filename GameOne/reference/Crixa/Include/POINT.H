//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	03/25/96
//
//------------------------------------------------------------------------------

#ifndef	POINT2D
#define	POINT2D

//------------------------------------------------------------------------------
#include "tuple.h"
#include "vector.h"

//------------------------------------------------------------------------------
class Point_2d : public Tuple_2d																								//	point class for physics model
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
  			Point_2d (void) {}																											//	constructor (empty)
  			Point_2d (Real x, Real y) : Tuple_2d (x, y) {}													//	constructor
  			Point_2d (const Tuple_2d &tuple) : Tuple_2d (tuple) {}									//	constructor
  			void		operator += (const Vector_2d &v);																//	add a vector to this point
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	POINT2D