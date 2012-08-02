//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	03/25/96
//
//------------------------------------------------------------------------------

#ifndef	TUPLE
#define	TUPLE

//------------------------------------------------------------------------------
enum	TupleComponent {X = 0, Y = 1};																						//	indexing parameter for the tuple class

//------------------------------------------------------------------------------
class Tuple_2d																																	//	tuple class for physics model
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
  			Real			xy[2];																												//	x and y components
	public:																																				//	members available externally
  			Tuple_2d (void) {}																											//	constructor (empty)
  			Tuple_2d (Real x, Real y);																							//	constructor
  			Tuple_2d (const Tuple_2d &tuple);																				//	constructor
				void			Floor (void);																									//	floor the values of the tuple
  			Real			operator [] (TupleComponent c) const;													//	array indexing operator
  			Real			&operator [] (TupleComponent c);															//	array indexing operator
  			void			operator () (Real x, Real y);																	//	function call operator to mimic constructor
  			Tuple_2d	&operator = (const Tuple_2d &tuple);													//	assignment operator to mimic constructor
};																																							//	end

//------------------------------------------------------------------------------
inline	Real	Tuple_2d::operator [] (TupleComponent c) const										//	array indexing operator
{																																								//	begin
	return xy[c];																																	//	return the requested component of the tuple
}																																								//	end

//------------------------------------------------------------------------------
inline	Real	&Tuple_2d::operator [] (TupleComponent c)													//	array indexing operator
{																																								//	begin
	return xy[c];																																	//	return the requested component of the tuple
}																																								//	end

//------------------------------------------------------------------------------
inline	Tuple_2d::Tuple_2d (Real x, Real y)																			//	constructor
{																																								//	begin
	xy[X] = x; xy[Y] = y;																													//	copy the values in
}																																								//	end

//------------------------------------------------------------------------------
inline	Tuple_2d::Tuple_2d (const Tuple_2d &tuple)															//	constructor
{																																								//	begin
	xy[X] = tuple.xy[X]; xy[Y] = tuple.xy[Y];																			//	copy the tuple values
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Tuple_2d::Floor (void)																						//	floor the values of the tuple
{																																								//	begin
	xy[X] = FLOOR (xy[X]);																												//	floor the x coordinate
	xy[Y] = FLOOR (xy[Y]);																												//	floor the y coordinate
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Tuple_2d::operator () (Real x, Real y)														//	function call operator to mimic constructor
{																																								//	begin
	xy[X] = x; xy[Y] = y;																													//	copy the values in
}																																								//	end

//------------------------------------------------------------------------------
inline	Tuple_2d	&Tuple_2d::operator = (const Tuple_2d &tuple)									//	assignment operator to mimic constructor
{																																								//	begin
	xy[X] = tuple.xy[X]; xy[Y] = tuple.xy[Y];																			//	copy the tuple values
	return *this;																																	//	return a reference to this for other usage
}																																								//	end

//------------------------------------------------------------------------------
inline	Real	operator | (const Tuple_2d &t1, const Tuple_2d &t2)								//	dot product of two vectors
{																																								//	begin
	return (t1[X] * t2[X]) + (t1[Y] * t2[Y]);																			//	compute and return the inner product of the two mathematical vectors
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	TUPLE