//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/08/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "text buffer.h"
#include "tokenize.h"

//------------------------------------------------------------------------------
TxtBuffer::TxtBuffer (cstr name) : Buffer (name, TRUE)													//	constructor
{																																								//	begin
	if (file->Open ())																														//	if the file is successfully opened
	{																																							//	begin
		long	fileLength = file->Length (),																					//	get the length of the file
					length = fileLength + 1;																							//	increment the length for an additional trailing 0
		data = new uchar[length];																										//	allocate the memory to load the file
		data[fileLength] = 0;																												//	set the end character to 0
		end = data + length;																												//	compute the end of buffer address
		current = data;																															//	start with the current location at zero offset
		file->Reset ();																															//	start the file out at the beginning
		file->Read (data, sizeof (uchar), fileLength);															//	read the contents of the file into the buffer memory
		file->Close ();																															//	close the file
		count = Tokenize (cstr (data), token);																			//	build the file tokens
		current = data;																															//	reset the file
		curToken = 0;																																//	start the tokenizer with the first token
	}																																							//	end
	else																																					//	otherwise there is no valid buffer
		count = 0;																																	//	set the count to 0
	error = file->Error ();																												//	copy the error state from the file
}																																								//	end

//------------------------------------------------------------------------------
TxtBuffer::TxtBuffer (File *fil) : Buffer (fil, TRUE)														//	constructor
{																																								//	begin
	if (file->Open ())																														//	if the file is successfully opened
	{																																							//	begin
		long	fileLength = file->Length (),																					//	get the length of the file
					length = fileLength + 1;																							//	increment the length for an additional trailing 0
		data = new uchar[length];																										//	allocate the memory to load the file
		data[fileLength] = 0;																												//	set the end character to 0
		end = data + length;																												//	compute the end of buffer address
		current = data;																															//	start with the current location at zero offset
		file->Reset ();																															//	start the file out at the beginning
		file->Read (data, sizeof (uchar), fileLength);															//	read the contents of the file into the buffer memory
		file->Close ();																															//	close the file
		count = Tokenize (cstr (data), token);																			//	build the file tokens
		current = data;																															//	reset the file
		curToken = 0;																																//	start the tokenizer with the first token
	}																																							//	end
	error = file->Error ();																												//	copy the error state from the file
}																																								//	end

//------------------------------------------------------------------------------
TxtBuffer::~TxtBuffer (void)																										//	destructor
{																																								//	begin
	if (token)																																		//	if the token array was actually allocated
		delete[] token;																															//	dispose of the token data
}																																								//	end

//------------------------------------------------------------------------------
long	TxtBuffer::Length (void)																									//	get the length of the buffer
{																																								//	begin
	return count;																																	//	return the number of tokens in the file
}																																								//	end

//------------------------------------------------------------------------------
void	TxtBuffer::Reset (void)																										//	move to the beginning of the buffer
{																																								//	begin
	curToken = 0;																																	//	start the tokenizer over at the first token
}																																								//	end

//------------------------------------------------------------------------------
void	TxtBuffer::Position (long pos)																						//	move to a specific location in the file
{																																								//	begin
	if ((pos >= 0) && (pos < count))																							//	if the target token is a valid token
		curToken = pos;																															//	set the current token to be the target token
}																																								//	end

//------------------------------------------------------------------------------
void	TxtBuffer::Move (long offset)																							//	move from the current offset in the file
{																																								//	begin
	long	result = curToken + offset;																							//	add the offset to the current token
	if ((result >= 0)	&& (result < count))																				//	if the target token is a valid token
		curToken = result;																													//	set the current token to the result
}																																								//	end

//------------------------------------------------------------------------------
long	TxtBuffer::Current (void)																									//	get the current location in the file
{																																								//	begin
	return curToken;																															//	return the current token
}																																								//	end

//------------------------------------------------------------------------------
TxtBuffer	&TxtBuffer::operator >> (int &i)																			//	read an integer from the text buffer
{																																								//	begin
	i = atoi (token[curToken]);																										//	convert the token string to an integer
	curToken++;																																		//	increment the token
	return *this;																																	//	return the text buffer object
}																																								//	end

//------------------------------------------------------------------------------
TxtBuffer	&TxtBuffer::operator >> (Real &r)																			//	read a real number from the text buffer
{																																								//	begin
	r = Real (atof (token[curToken]));																						//	convert the token string to a real number
	curToken++;																																		//	increment the token
	return *this;																																	//	return the text buffer object
}																																								//	end

//------------------------------------------------------------------------------
TxtBuffer	&TxtBuffer::operator >> (cstr &s)																			//	read a string from the text buffer
{																																								//	begin
	s = token[curToken];																													//	copy the string pointer from the current token
	curToken++;																																		//	increment the token
	return *this;																																	//	return the text buffer object
}																																								//	end

//------------------------------------------------------------------------------
