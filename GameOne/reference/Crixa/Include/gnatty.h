//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Revisions:			01/29/97.	creation.  (JB)
//									02/06/97.	breakout of state functions (JB)
//  02/21/97				MK				Added "attacking" boolean variable
//
//------------------------------------------------------------------------------

#ifndef	GNATTY_DREAD
#define	GNATTY_DREAD

//------------------------------------------------------------------------------
#include "eks bug.h"
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
class	Gnatty : public EksBug
{
	private:
	protected:
				Bool					attacking;																								// (MK) debounce flag
				Real					damage;
virtual	void					DoState (void);
virtual	void					Attack (CPoint pt);
virtual	void					TrackingTarget (CPoint loc, CVector toTarget);
				void					SelfDestruct (void);
	public:
				Gnatty (int type, Real rad, Real rLoc, CPoint tLoc, SArrayPtr sptr);
virtual	~Gnatty (void);
};

//------------------------------------------------------------------------------

#endif // GNATTY_DREAD
