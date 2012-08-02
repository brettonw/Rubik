//------------------------------------------------------------------------------
//	Copyright ©1997 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Revisions:			02/11/97
//
//	02/28/97	JB		moved visual stuff into barrack ship, where it belongs.
//
//------------------------------------------------------------------------------

#ifndef	BARRACK_PLAYER
#define	BARRACK_PLAYER 

//------------------------------------------------------------------------------
#include "eks bug.h"
#include "barrack ship.h"
#include "list.h"
#include "sequence ptr.h"
#include "emitter device.h"

//------------------------------------------------------------------------------
class Barrack : public EksBug																										//	a special eks bug that creates other bugs
{
	private:
	protected:
				SeqPtr				sequence;																									//	pointer to the barrack's sequence
				int						index;																										//	the barrack's identification
				int						defaultBugType;																						//	the default bug type of this barrack
				int						currentBugType;																						//	the current bug type that this barrack is working on
				int						numChildren;																							//	the current number of children the barrack has
				int						maxChildren;																							//	the maximum number of children the barrack has
				Real					timeSpent;																								//	the amount of time the barrack has spent on creating the bug
				Real					gestationTime;																						//	the total time required for the creation of the current bug
				List					children;																									//	the list of live kids
				List					deceased;																									//	the list of dead kids
				EmitterDevice	soundDevice;
				void					CreateNewBug (void);																			//	make a new bug
virtual	void					DoState (void);																						//	simplistic AI
virtual	void					Attack (CPoint pt);																				//	nothing
virtual	void					TrackingTarget (CPoint pt, CVector vect);									//	nothing
				void					PlayChildren (void);																			//	tell the kids to go have fun
				int						DetermineProduction (void);																//	figure out what to make next
				Real					DetermineGestationTime (int bugType);											//	figure out how long it will take
virtual	Bool					PlayDead (void);																					//	what to do on the way out
	public:
				Barrack (Player *parent, int index, int type, Real radius, Real rloc,		//	constructor
								 Real armor, CPoint tloc, cstr sptr, int maxKids, int bugType);	//	constructor (continued)
virtual	~Barrack (void);																												//	destructor
virtual	void					SetTarget (const PiecePtr &piece);												//	set target
				void					AttachBug (EksBug *eksBug);																//	make the kid mine
				void					RegisterDeath (int bugType);															//	so the bugs can tell their parents that they died
				int						Identification (void);																		//	return its identification index
				void					Switch (int state);																				//	visually switch to another state
};

//------------------------------------------------------------------------------
inline	int	Barrack::Identification (void)																			//	return index identification
{
	return index;																																	//	return index identification
}

//------------------------------------------------------------------------------
inline	void	Barrack::Switch (int state)																				//	visual switch
{
	((BarrackShip *) ship ())->Switch (state);																		//	visual switch
}

//------------------------------------------------------------------------------

#endif // BARRACK_PLAYER
