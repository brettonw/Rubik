//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/20/96
//
//------------------------------------------------------------------------------

#ifndef	THUG
#define	THUG

//------------------------------------------------------------------------------
#include "player.h"
#include "korves ship.h"
#include "piece ptr.h"
#include "planner.h"
#include "path.h"
#include "lock ptr.h"

//------------------------------------------------------------------------------
enum	{	KORVES_DRIFT = 0, KORVES_PLANNING, KORVES_FOLLOWING_PATH, 							//	korves states
				KORVES_TRACKING_TARGET};																								//	korves states (continued)

//------------------------------------------------------------------------------
class KorvesPlay : public Player																								//	a korves player
{																																								//	begin
	private:																																			//	members internal to this class only
static	Signature		sign;
	protected:																																		//	members internal to this class hierarchy
				PiecePtr		ship;																												//	pointer to the ship
				LockPtr			lock;																												//	lock on a selected target
				Planner			*planner;																										//	pointer to the planner if we have it
				Path				*path;																											//	pointer to the path if we have one
				int					state;																											//	the current ship state
				Point_2d		tLoc;																												//	the spawn location for the ship
				Real				rLoc;																												//	the spawn orientation for the ship
				Bool				kill;																												//	whether or not to kill the target
				void				NewShip (void);																							//	new ship function
				void				ChangeState (int state);																		//	switch to a new state
				void				DoState (void);																							//	do the appropriate action for the current state and situation
				Bool				GoTo (CPoint pt);																						//	fly the ship to a specified point
				void				Attack (CPoint pt);																					//	fly the ship at a specified point and fire
				void				Stop (void);																								//	stop the ship
	public:																																				//	members available externally
				KorvesPlay (CPoint tloc, Real rloc);																		//	constructor
virtual	~KorvesPlay (void);																											//	destructor
virtual	Bool				Play (void);																								//	play one frame
				void				SetGoal (const LockPtr &lock, Bool kill);										//	set the korves base goal
				PiecePtr		GetKorves (void);
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	THUG