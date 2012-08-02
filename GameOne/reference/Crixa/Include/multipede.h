//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/12/97
//
//------------------------------------------------------------------------------

#ifndef	MULTIPEDE
#define	MULTIPEDE

//------------------------------------------------------------------------------
#include "thug.h"
#include "weapon ship.h"
#include "piece ptr.h"
#include "planner.h"
#include "path.h"
#include "lock ptr.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class Multipede : public Player																									//	a multipede thug player
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				int					delay;

				int					count;
				int					maxCount;
				Real				separation;
				Real				damping;
				PiecePtr		*ship;																											//	pointer to the ship

				LockPtr			lock;																												//	lock on a selected target
				Planner			*planner;																										//	pointer to the planner if we have it
				Path				*path;																											//	pointer to the path if we have one
				int					state;																											//	the current ship state
				Bool				inRange;
				Point_2d		tLoc;																												//	the spawn location for the ship
				Real				rLoc;																												//	the spawn orientation for the ship
				void				NewShip (void);																							//	new ship function
				void				ChangeState (int state);																		//	switch to a new state
				void				DoState (void);																							//	do the appropriate action for the current state and situation
				Bool				GoTo (CPoint pt, Real radius);															//	fly the ship to a specified point
				void				Attack (CPoint pt);																					//	fly the ship at a specified point and fire
				void				RandomWalk (void);																					//	do a random walk flight pattern
				void				Stop (void);																								//	stop the ship
	public:																																				//	members available externally
				Multipede (CPoint tloc, Real rloc, int count);														//	constructor
virtual	~Multipede (void);																											//	destructor
virtual	Bool				Play (void);																								//	play one frame
virtual	void				SetTarget (const PiecePtr &piece);													//	set the target piece
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	MULTIPEDE
