//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	07/29/96
//
//------------------------------------------------------------------------------

#ifndef	INT_VECTOR
#define	INT_VECTOR

//------------------------------------------------------------------------------
#include "point.h"

//------------------------------------------------------------------------------
class IVec_2d																																		//	integer vector
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				short			xy[2];																												//	x and y components
	public:																																				//	members available externally
  			IVec_2d (void) {}																												//	constructor (empty)
  			IVec_2d (short x, short y);																							//	constructor
  			IVec_2d (const IVec_2d &tuple);																					//	constructor
  			IVec_2d (const Point_2d &tuple);																				//	constructor
  			short			operator [] (TupleComponent c) const;													//	array indexing operator
  			short			&operator [] (TupleComponent c);															//	array indexing operator
  			void			operator () (short x, short y);																//	function call operator to mimic constructor
  			IVec_2d		&operator = (const IVec_2d &tuple);														//	assignment operator to mimic constructor
  			int				operator == (const IVec_2d &tuple) const;											//	equality operator
  			int				operator != (const IVec_2d &tuple) const;											//	inequality operator
};																																							//	end

//------------------------------------------------------------------------------
inline	short	IVec_2d::operator [] (TupleComponent c) const											//	array indexing operator
{																																								//	begin
	return xy[c];																																	//	return the requested component of the tuple
}																																								//	end

//------------------------------------------------------------------------------
inline	short	&IVec_2d::operator [] (TupleComponent c)													//	array indexing operator
{																																								//	begin
	return xy[c];																																	//	return the requested component of the tuple
}																																								//	end

//------------------------------------------------------------------------------
inline	IVec_2d::IVec_2d (short x, short y)																			//	constructor
{																																								//	begin
	xy[X] = x; xy[Y] = y;																													//	copy the values in
}																																								//	end

//------------------------------------------------------------------------------
inline	IVec_2d::IVec_2d (const IVec_2d &tuple)																	//	constructor
{																																								//	begin
	*(long *) xy = *(long *) tuple.xy;																						//	copy the tuple as a long word
}																																								//	end

//------------------------------------------------------------------------------
inline	IVec_2d::IVec_2d (const Point_2d &tuple)																//	constructor
{																																								//	begin
	xy[X] = int (tuple[X]); xy[Y] = int (tuple[Y]);																//	copy the tuple values
}																																								//	end

//------------------------------------------------------------------------------
inline	void	IVec_2d::operator () (short x, short y)														//	function call operator to mimic constructor
{																																								//	begin
	xy[X] = x; xy[Y] = y;																													//	copy the values in
}																																								//	end

//------------------------------------------------------------------------------
inline	IVec_2d	&IVec_2d::operator = (const IVec_2d &tuple)											//	assignment operator to mimic constructor
{																																								//	begin
	*(long *) xy = *(long *) tuple.xy;																						//	copy the tuple as a long word
	return *this;																																	//	return a reference to this for other usage
}																																								//	end

//------------------------------------------------------------------------------
inline	int		IVec_2d::operator == (const IVec_2d &tuple) const									//	equality operator
{																																								//	begin
	return *(long *) xy == *(long *) tuple.xy;																		//	compare the two vectors as a long word
}																																								//	end

//------------------------------------------------------------------------------
inline	int		IVec_2d::operator != (const IVec_2d &tuple) const									//	inequality operator
{																																								//	begin
	return *(long *) xy != *(long *) tuple.xy;																		//	compare the two vectors as a long word
}																																								//	end

//------------------------------------------------------------------------------
inline	IVec_2d	operator + (const IVec_2d &t1, const IVec_2d &t2)								//	addition of two vectors
{																																								//	begin
	return IVec_2d (t1[X] + t2[X], t1[Y] + t2[Y]);																//	compute and return the sum of the two vectors
}																																								//	end

//------------------------------------------------------------------------------
inline	IVec_2d	operator - (const IVec_2d &t1, const IVec_2d &t2)								//	difference of two vectors
{																																								//	begin
	return IVec_2d (t1[X] - t2[X], t1[Y] - t2[Y]);																//	compute and return the difference of the two vectors
}																																								//	end

//------------------------------------------------------------------------------
inline	int	operator | (const IVec_2d &t1, const IVec_2d &t2)										//	dot product of two vectors
{																																								//	begin
	return (t1[X] * t2[X]) + (t1[Y] * t2[Y]);																			//	compute and return the inner product of the two mathematical vectors
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	INT_VECTOR