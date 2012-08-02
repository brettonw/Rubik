//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	12/23/96
//
//------------------------------------------------------------------------------

#ifndef	FIREBALL
#define	FIREBALL

//------------------------------------------------------------------------------
#include "piece ptr.h"
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
class FireBall : public Piece																										//	a fireball
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				SArrayPtr	puff;																													//	the puff sprite
	public:																																				//	members available externally
				FireBall (Real rad, CPoint loc, Real rloc, CVector vel, SArrayPtr puff);//	constructor (continued)
virtual	~FireBall (void);																													//	destructor
virtual	Bool			Step (Real time);																							//	step the piece ahead in time
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	FIREBALL
