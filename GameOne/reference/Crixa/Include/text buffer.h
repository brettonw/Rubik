//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/08/97
//
//------------------------------------------------------------------------------

#ifndef	TEXT_BUFFER
#define	TEXT_BUFFER

//------------------------------------------------------------------------------
#include "buffer.h"

//------------------------------------------------------------------------------
class	TxtBuffer : public Buffer																									//	text buffer class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				long			count;																												//	the number of tokens in the file
				long			curToken;																											//	the current token
				cstr			*token;																												//	pointer to an array of tokens
	public:																																				//	members available externally
				TxtBuffer (cstr name);																									//	constructor
				TxtBuffer (File *file);																									//	constructor
virtual	~TxtBuffer (void);																											//	destructor
virtual	long			Length (void);																								//	get the length of the buffer
virtual	void			Reset (void);																									//	move to the beginning of the buffer
virtual	void			Position (long pos);																					//	move to a specific location in the file
virtual	void			Move (long offset);																						//	move from the current offset in the file
virtual	long			Current (void);																								//	get the current location in the file
				TxtBuffer	&operator >> (int &i);																				//	read an integer from the text buffer
				TxtBuffer	&operator >> (Real &r);																				//	read a real number from the text buffer
				TxtBuffer	&operator >> (cstr &s);																				//	read a string from the text buffer
};																																							//	end

//------------------------------------------------------------------------------

#endif	//TEXT_BUFFER
