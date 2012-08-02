//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:		Bretton Wade
//	Last Revision:	07/17/96
//
//------------------------------------------------------------------------------

#ifndef	PLASMA_BUBBLE
#define	PLASMA_BUBBLE

//------------------------------------------------------------------------------
#include "moving sequence.h"
#include "sprite array ptr.h"
#include "piece ptr.h"

//------------------------------------------------------------------------------
class PBubble : public MoveSeq																									//	a plasma bubble sequence
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Real			damage;																												//	how much damage the plasma bubble does
				PiecePtr	owner;																												//	pointer to the owning piece
	public:																																				//	members externally available
				PBubble (SArrayPtr spr, CPoint pt, CVector vel, Real dam, PiecePtr own,	//	constructor
									Real speed);																									//	constructor (continued)
virtual ~PBubble (void);																												//	destructor
virtual Bool			Step (CPoint where);																					//	move it move it
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	PLASMA_BUBBLE