//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/17/97
//
//------------------------------------------------------------------------------

#ifndef	RANDOM
#define	RANDOM

//------------------------------------------------------------------------------
#include "debugger.h"

//------------------------------------------------------------------------------
class QRandom																																		//	random number generator class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				int				size;																													//	size of the random number table
				int				mask;																													//	mask to prevent overflow of the table
				int				index;																												//	the current index into the table
				struct Couple																														//	internal structure for storing value couples
				{																																				//	begin
					int				integer;																										//	the integer value
					Real			real;																												//	the real value
				}					*table;																												//	array of randomly generated values as integer and real values
	public:																																				//	members available externally
				QRandom (int bits = 8);																									//	constructor
				QRandom (int size, int *table);																					//	constructor
				~QRandom (void);																												//	destructor
				int				RndInteger (void);																						//	return a random integer
				Real			RndReal (void);																								//	return a random real number
				int				Size (void);																									//	return the size of the table
				int				GetIndex (void);																							//	return the random table index
				void			SetIndex (int index);																					//	set the index into the random table
};																																							//	end

//------------------------------------------------------------------------------
inline	int		QRandom::RndInteger (void)																				//	return a random integer
{																																								//	begin
	int		number = table[index].integer;																					//	get the next number out of the table
	index = (index + 1) & mask;																										//	increment the table index
	return number;																																//	return the number
}																																								//	end

//------------------------------------------------------------------------------
inline	Real	QRandom::RndReal (void)																						//	return a random real number
{																																								//	begin
	Real	number = table[index].real;																							//	get the next number out of the table
	index = (index + 1) & mask;																										//	increment the table index
	return number;																																//	return the number
}																																								//	end

//------------------------------------------------------------------------------
inline	int		QRandom::Size (void)																							//	return the size of the table
{																																								//	begin
	return size;																																	//	return the size
}																																								//	end

//------------------------------------------------------------------------------
inline	int		QRandom::GetIndex (void)																					//	return the random table index
{																																								//	begin
	return index;																																	//	return the index
}																																								//	end

//------------------------------------------------------------------------------
inline	void	QRandom::SetIndex (int i)																					//	set the index into the random table
{																																								//	begin
	index = i & mask;																															//	set the index of the table
}																																								//	end

//------------------------------------------------------------------------------
extern	QRandom	gRandom;																												//	global random number generator
extern	int			gRandomTable[1024];																							//	a pre-computed table of random 10-bit integers with a uniform distribution
extern	QRandom	gLocalRandom;																										//	global random number generator for local visual effects only
//------------------------------------------------------------------------------

#endif	//	RANDOM
