//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/20/97
//	03/04/97	DS		flashing font support
//
//------------------------------------------------------------------------------

#ifndef	TEXT_SEQUENCE
#define	TEXT_SEQUENCE

//------------------------------------------------------------------------------
#include "sequence.h"
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
class TextSeq : public Sequence																									//	sequence of text
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
  			SArrayPtr	font;																													//	array of sprites for the font
				SArrayPtr	boldfont;																											//	array of sprites for the bold font
				cstr			text;																													//	the string to draw
				int				length;																												//	the length of the string
				int				current;																											//	the current character in the string
				int				nextword;																											//	the next part to draw in blue
				int				margin;																												//	soft right hand margin
				int				displayCount;																									//	the number of frames remaining for display
	public:																																				//	members available externally
  			TextSeq (SArrayPtr font, SArrayPtr bold, cstr text, int margin);				//	constructor
  			TextSeq (SArrayPtr font, SArrayPtr bold, cstr text, int margin, int count);	//	constructor
virtual	~TextSeq (void);																												//	destructor
virtual	Bool			Step (CPoint where);																					//	draw the next frame
virtual void			Reset (void);																									//	return the sequence to its starting state
				int				Length (void);																								//	return the text length
};																																							//	end

//------------------------------------------------------------------------------
inline	int		TextSeq::Length (void)																						//	return the text length
{																																								//	begin
	return length;																																//	return the text length
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	TEXT_SEQUENCE