//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Revisions:			02/11/97
//
//	02/21/97				MK							Added "playerspotted" variable to indicate if player has been seen
//
//------------------------------------------------------------------------------

#ifndef	THUG
#define	THUG

//------------------------------------------------------------------------------
#include "eks bug.h"

//------------------------------------------------------------------------------
enum	{DRIFT = 0, PLANNING, FOLLOWING_PATH, TRACKING_TARGET};										//	thug states

//------------------------------------------------------------------------------
class	Thug : public EksBug
{
	private:
	protected:
				Bool				playerspotted;																							// (MK) has the bug spotted the player for the first time?
				int					delay;
				Bool				inRange;																										//	whether or not the ship is in attacking range
				void				NewShip (Real rLoc, CPoint tLoc);														//	new ship function
virtual	void				DoState (void);
virtual	void				ChangeState (int state);																		//	switch to a new state
virtual	void				Attack (CPoint pt);																					//	fly the ship at a specified point and fire
virtual	void				TrackingTarget (CPoint pt, CVector v);
	public:
				Thug (CPoint tloc, Real rloc, int type);																//	constructor
virtual	~Thug (void);																														//	destructor
};

//------------------------------------------------------------------------------

#endif // THUG