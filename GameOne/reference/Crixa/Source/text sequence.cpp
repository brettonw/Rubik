//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/29/97
//	03/04/97	DS		Added bold font
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "text sequence.h"

//------------------------------------------------------------------------------
TextSeq::TextSeq (SArrayPtr array, SArrayPtr bold, cstr tex, int mar)						//	constructor
{																																								//	begin
	font = array;																																	//	copy the sprites array
	boldfont = bold;										
	current = 0;																																	//	start with the first character
	length = strlen (tex);																												//	compute the string length
	text = new char[length + 1];																											//	allocate the text string
	strcpy (text, tex);																														//	copy the string to draw
	margin = mar;																																	//	copy the soft margin
	length--;																																			//	change the length to represent the max
	nextword = 0;
	for (int i=0;i<length;i++)																										//	look for first space
		if (text[i]==' ')
		{
			nextword = i;																															//	
			break;
		}
	displayCount = length * 3;																										//	show the text for 2 frames for each character
}																																								//	end

//------------------------------------------------------------------------------
TextSeq::TextSeq (SArrayPtr array, SArrayPtr bold, cstr tex, int mar, int coun)	//	constructor
{																																								//	begin
	font = array;																																	//	copy the sprites array
	boldfont = bold;										
	current = 0;																																	//	start with the first character
	length = strlen (tex);																												//	compute the string length
	text = new char[length + 1];																											//	allocate the text string
	strcpy (text, tex);																														//	copy the string to draw
	margin = mar;																																	//	copy the soft margin
	length--;																																			//	change the length to represent the max
	nextword = 0;
	for (int i=0;i<length;i++)																										//	look for first space
		if (text[i]==' ')
		{
			nextword = i;																															//	
			break;
		}
	displayCount = coun;																													//	show the text for 2 frames for each character
}																																								//	end

//------------------------------------------------------------------------------
TextSeq::~TextSeq (void)																												//	destructor
{																																								//	begin
	delete[] text;
}																																								//	end

//------------------------------------------------------------------------------
Bool	TextSeq::Step (CPoint where)																							//	draw the next frame
{																																								//	begin
	int		ix = int (where[X]),
				x = 0,
				y = int (where[Y]);

/*	if ((nextword == length) && (displayCount & 0x00000010))											// flash stuff
	else
	{
*/
	int i = 0;
	for (i = 0; i <= current; i++)																								//	loop up to the current position in the string
		switch (text[i])
		{
			case '\t':
				x = (((x / font->Width ()) + 2) & 0x0000fffe) * font->Width ();
				break;
			case '\n':
				y -= font->Height ();
				x = 0;
				break;
			case ' ':
				if (x > margin)
				{
					y -= font->Height ();
					x = 0;
				}
				else
					x += font['n']->Width ();																							//	use an em space for the space.
				break;
			default:
				font[text[i]]->Draw (Point_2d (Real (x + ix), Real (y)), TRUE);					//	draw the string up to the current point
				x += font[text[i]]->Width ();
				break;
		}

/*	if (current < length)
		current++; */
	for (i;i<nextword;i++)
		switch (text[i])
		{
			case '\t':
				x = (((x / font->Width ()) + 2) & 0x0000fffe) * font->Width ();
				break;
			case '\n':
				y -= font->Height ();
				x = 0;
				break;
			case ' ':
				if (x > margin)
				{
					y -= font->Height ();
					x = 0;
				}
				else
					x += font['n']->Width ();																							//	use an em space for the space.
				break;
			default:
				boldfont[text[i]]->Draw (Point_2d (Real (x + ix), Real (y)), TRUE);			//	draw the string up to the current point
				x += boldfont[text[i]]->Width ();
				break;
		}

	current = nextword;
	nextword = length;
	for (int c=i;c<length;c++)																										//	look for first space
		if (text[c]==' ')
		{
			nextword = c;																															//	
			break;
		}

	return (--displayCount > 0) ? TRUE : FALSE;																		//	return whether or not the string has been displayed long enough
}																																								//	end

//------------------------------------------------------------------------------
void	TextSeq::Reset (void)																											//	return the sequence to its starting state
{																																								//	begin
	current = 0;																																	//	start with the first character
	nextword = 0;
	for (int i=0;i<length;i++)																										//	look for first space
		if (text[i]==' ')
		{
			nextword = i;																															//	
			break;
		}
	displayCount = length * 3;																										//	show the text for 2 frames for each character
}																																								//	end

//------------------------------------------------------------------------------
