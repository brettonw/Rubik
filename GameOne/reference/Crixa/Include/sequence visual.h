//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/20/96
//
//	02/24/97	DS		Added AltDrawMode
//
//------------------------------------------------------------------------------

#ifndef	SEQUENCE_VISUAL
#define	SEQUENCE_VISUAL

//------------------------------------------------------------------------------
#include "visual.h"
#include "sequence ptr.h"

//------------------------------------------------------------------------------
class SeqVisual : public Visual																									//	visual effect sequence for a body
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				SeqPtr		sequence;																											//	sequence to display at the body location
	public:																																				//	members available externally
				SeqVisual (Body *body, const SeqPtr &seq);															//	constructor
virtual	~SeqVisual (void);																											//	destructor
virtual	void			Draw (CPoint where);																					//	draw the object
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	SEQUENCE_VISUAL