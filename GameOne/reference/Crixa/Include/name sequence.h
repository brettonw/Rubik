//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/27/97
//
//------------------------------------------------------------------------------

#ifndef	NAME_SEQUENCE
#define	NAME_SEQUENCE

//------------------------------------------------------------------------------
#include "sequence.h"
#include "sprite array ptr.h"
#include "player.h"

//------------------------------------------------------------------------------
class NameSeq : public Sequence																									//	sequence of text
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Player		*player;																											//	pointer to the player this is for
  			SArrayPtr	font;																													//	array of sprites for the font
	public:																																				//	members available externally
  			NameSeq (Player *player, SArrayPtr font);																//	constructor
virtual	~NameSeq (void);																												//	destructor
virtual	Bool			Step (CPoint where);																					//	draw the next frame
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	NAME_SEQUENCE