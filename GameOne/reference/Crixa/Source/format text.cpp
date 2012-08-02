//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/15/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "format text.h"

//------------------------------------------------------------------------------
static	pstr	gHexToInt;																												//	the lookup table for the hex to int conversion

//------------------------------------------------------------------------------
void	BuildTable_h2i (void)																											//	build the lookup table for the hex to int conversion routine
{																																								//	begin
	static cstr	hexLow = "0123456789abcdef",																			//	a table for looking up lower case hex digits
				 			hexCap = "0123456789ABCDEF";																			//	a table for looking up upper case hex digits
	gHexToInt = new uchar[65536];																									//	allocate the hex to int table
	for (int i = 0; i < 256; i++)																									//	loop over all of the possible values of a byte
	{																																							//	begin
#ifdef	_MSC_VER
		gHexToInt[ (hexLow[i & 0x0f] << 8) | hexLow[(i & 0xf0) >> 4] ] = i;					//	compute an index into the hex to int table based on character values an assign the counter value
		gHexToInt[ (hexCap[i & 0x0f] << 8) | hexCap[(i & 0xf0) >> 4] ] = i;					//	compute an index into the hex to int table based on character values an assign the counter value
		gHexToInt[ (hexLow[i & 0x0f] << 8) | hexCap[(i & 0xf0) >> 4] ] = i;					//	compute an index into the hex to int table based on character values an assign the counter value
		gHexToInt[ (hexCap[i & 0x0f] << 8) | hexLow[(i & 0xf0) >> 4] ] = i;					//	compute an index into the hex to int table based on character values an assign the counter value
#else
		gHexToInt[ hexLow[i & 0x0f] | (hexLow[(i & 0xf0) >> 4] << 8)] = i;					//	compute an index into the hex to int table based on character values an assign the counter value
		gHexToInt[ hexCap[i & 0x0f] | (hexCap[(i & 0xf0) >> 4] << 8)] = i;					//	compute an index into the hex to int table based on character values an assign the counter value
		gHexToInt[ hexLow[i & 0x0f] | (hexCap[(i & 0xf0) >> 4] << 8)] = i;					//	compute an index into the hex to int table based on character values an assign the counter value
		gHexToInt[ hexCap[i & 0x0f] | (hexLow[(i & 0xf0) >> 4] << 8)] = i;					//	compute an index into the hex to int table based on character values an assign the counter value
#endif
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
void	FormatText (cstr text)																										//	format escape sequences in a text string
{																																								//	begin
	cstr	out = text;
	while (*text)
	{
		if (*text == '\\')
		{
			text++;
			int		number;
			switch (*text)
			{
				case 'n':
					*text = '\n';
					break;
				case 't':
					*text = '\t';
					break;
				case 'x':
					text++;
					number = gHexToInt[*(short *) text];
					text++;
					*text = number;
					break;
				case '0': case '1': case '2': case '3': case '4':
				case '5': case '6': case '7': case '8': case '9':
					text += 2;
					*text = ' ';	//	don't handle these for right now, just skip the character
					break;
			}
		}
		*out++ = *text++;
	}
	*out = 0;
}																																								//	end

//------------------------------------------------------------------------------
