//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	07/14/96
//	Revised:				02/15/97 	Added [] string checking for state manager
//	02/07/97				JB				modified the "//" comment read to work correctly
//														for mac and pc newlines.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "tokenize.h"

//------------------------------------------------------------------------------
int		Tokenize (cstr data, cstr *&token)																				//	process a string into a stream of tokens
{																																								//	begin
	int		count = 0;																															//	start with a zero count
	token = 0;																																		//	empty token array
	if (data)																																			//	if the string is valid
	{																																							//	begin
		cstr	current = data;																												//	set up an iterator for the data
		int		last = 1;																															//	set the last flag
		while (*current)																														//	loop until we hit the null at the end of the string
		{																																						//	begin
			switch (*current)																													//	look at the current character
			{																																					//	begin
				case '/':																																//	if it is a slash
					if (*(short*) current == '//')																				//	and the next character is also a slash
						while (*current && (*current != '\n') && (*current != '\r'))															//	loop until we find a newline
							*current++ = 0;																										//	setting the current character to 0 and moving to the next one
						if (*current == 0)																									//	if we have hit the null terminator at the end of the file
							current--;																												//	back up so the process will terminate correctly
				case '\n':																															//	if it is a newline
				case '\r':																															//	if it is a carriage return
				case ' ':																																//	if it is a space
				case '\t':																															//	if it is a tab
					*current = 0;																													//	zero the character
					last  = 1;																														//	set the last flag
					break;																																//	end case
				case '[':																																//	if there is a [
					count++;																															// 	increment counter
					last =0;																															//	clear the last flag
					*current = 0;																													//	zero the character
					do																																		// 	loop
						current++;																													// 	increment the position
					while (*current && (*current != ']'));																//	loop until we find the end of the string
					*current = 0;																													//	zero the character
					break;																																//	end case
				case '"':																																//	if it is a quote
					count++;																															//	increment the count
					last = 0;																															//	clear the last flag
					*current = 0;																													//	zero the character
					do																																		//	loop
						current++;																													//	increment the position
					while (*current && (*current != '"'));																//	loop until we find the end of the string
					*current = 0;																													//	zero the character
					break;																																//	end case
				default:																																//	for anything else
					count += last;																												//	increment the count if the last flag is set
					last = 0;																															//	clear the last flag
					break;																																//	end case
			}																																					//	end
			current++;																																//	move on to the next character
		}																																						//	end
		token = new cstr[count];																										//	allocate the token array
		current = data;																															//	reset the iterator
		for (int i = 0; i < count; i++)																							//	loop over all of the tokens
		{																																						//	begin
			while (!*current)																													//	loop until we find a valid character
				current++;																															//	incrementing the character as we go
			token[i] = cstr (current);																								//	save the current pointer into the token array
			while (*current)																													//	loop until we find a null character
				current++;																															//	incrementing the character as we go
		}																																						//	end
	}																																							//	end
	return count;																																	//	return the token count
}																																								//	end

//------------------------------------------------------------------------------
