//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/07/96
//
//------------------------------------------------------------------------------

#ifndef	EXPLOSION_SEQUENCE
#define	EXPLOSION_SEQUENCE

//------------------------------------------------------------------------------
#include "sequence ptr.h"

//------------------------------------------------------------------------------
class ExplSeq : public Sequence																									//	an explosion sequence
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Real			radius;																												//	circle radius
				Point_2d	location;																											//	current location of the explosion
				Vector_2d	velocity;																											//	velocity of the explosion
				int				pops;																													//	how many pops to make per frame
				Real			timeLeft;																											//	how long the sequence will last
	public:																																				//	members available externally
  			ExplSeq (CPoint pt, CVector vel, Real time, Real radius, int pops);			//	constructor
virtual	~ExplSeq (void);																												//	destructor
virtual	Bool			Step (CPoint where);																					//	draw the next frame
virtual	void			Pop (void) = 0;																								//	make a new pop
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	EXPLOSION_SEQUENCE
