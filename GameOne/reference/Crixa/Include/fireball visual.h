//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	12/23/96
//
//------------------------------------------------------------------------------

#ifndef	FIREBALL_VISUAL
#define	FIREBALL_VISUAL

//------------------------------------------------------------------------------
#include "sequence visual.h"
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
class FireBall : public SeqVisual																								//	visual effect sequence for a fireball
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				SArrayPtr	puff;																													//	the puff sprite
				SeqPtr		sequence;																											//	sequence to display at the body location
	public:																																				//	members available externally
				FireBall (Body *body, const SArrayPtr &puff);														//	constructor
virtual	~FireBall (void);																												//	destructor
virtual	void			Draw (CPoint where);																					//	draw the object
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	FIREBALL_VISUAL