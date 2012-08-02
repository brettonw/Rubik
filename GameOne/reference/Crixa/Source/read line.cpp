//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	05/28/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "Precompile.pch++"
#endif

//------------------------------------------------------------------------------
#include "read line.h"

//------------------------------------------------------------------------------
int		ReadLine (FILE *fp, cstr line)																						//	read a line from a text file
{																																								//	begin
	int		c = fgetc (fp),																													//	get a chracter from the file
				index = 0;																															//	start reading into the beginning of the line
	while ((c != EOF) && (c != '\n'))																							//	while this character is not the end of the file or the end of the line
	{																																							//	begin
		line[index] = c;																														//	save the character into the line
		index++;																																		//	advance the index
		c = fgetc (fp);																															//	get the next character from the file
	}																																							//	end
	line[index] = 0;																															//	null terminate the line string
	return (index != 0);																													//	return whether or not we read any characters into the line
}																																								//	end

//------------------------------------------------------------------------------
