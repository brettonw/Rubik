//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	07/02/96
//
//------------------------------------------------------------------------------

#ifndef	VECTOR
#define	VECTOR

//------------------------------------------------------------------------------
#include "tuple.h"

//------------------------------------------------------------------------------
class Vector_2d : public Tuple_2d																								//	vector class for physics model
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
  			Vector_2d (void) {}																											//	constructor (empty)
  			Vector_2d (Real x, Real y) : Tuple_2d (x, y) {}													//	constructor
  			Vector_2d (const Tuple_2d &tuple) : Tuple_2d (tuple) {}									//	constructor
  			Vector_2d (Real angle) : Tuple_2d (COS (angle), SIN (angle)) {}					//	constructor
  			Real			Normalize (void);																							//	reduce the vector to length 1
  			Real			Norm (void) const;																						//	return the length of the vector
  			void			operator += (const Vector_2d &v);															//	add another vector into this one
  			void			operator -= (const Vector_2d &v);															//	subtract another vector from this one
  			void			operator *= (Real s);																					//	multiply this vector by a scalar
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	VECTOR