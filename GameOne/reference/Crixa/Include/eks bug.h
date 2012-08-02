//------------------------------------------------------------------------------
//	Copyright ©1997 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Revisions:			02/11/97.	creation.  see "eks bug.cpp" for description (JB)
//
//  02/20/97				MK				Added HKELITE type to BugType
//------------------------------------------------------------------------------

#ifndef EKS_BUG
#define EKS_BUG

//------------------------------------------------------------------------------
#include "player.h"
#include "piece ptr.h"
#include "lock ptr.h"
#include "planner.h"
#include "path.h"

//------------------------------------------------------------------------------
enum	BugType
{
	LBUG = 0,
	HUNTER1,
	HUNTER2,
	HUNTER3,
	DRONE1,
	DRONE2,
	DRONE3,
	PRINCESS,
	GNATTY,
	HKELITE,
	BARRACK
};

//------------------------------------------------------------------------------
class	EksBug : public Player
{
	private:
	protected:
				Bool					thrusting;																								// Flag to indicate if ship is thrusting
				long					thrustEMid;																								// Emitter ID to identifying thrusting sound
				Bool					gotplanner;																								// (MK) For planning bugs, indicate if we are the ones who have the planner
				int						ekthrustsound, ekangersound;															// Sound indexes for EK sounds
				int						type;
				int						state;
				Player				*parent;
				PiecePtr			ship;
				LockPtr				lock;
				Planner				*planner;
				Path					*path;
	protected:
virtual	void					ChangeState (int state);
virtual	void					DoState (void) = 0;
virtual	void					Drifting (void);
virtual	void					Attack (CPoint pt) = 0;
virtual	void					TrackingTarget (CPoint loc, CVector toTarget) = 0;
virtual	Bool					GoTo (CPoint pt, Real radius);
virtual	Bool					GoAt (CPoint pt);
virtual	void					RandomWalk (void);																				//	do a random walk flight pattern
virtual	void					Stop (void);
virtual	Bool					PlayAlive (void);
virtual	Bool					PlayDead (void);
	public:
				EksBug (Player *parent, int type);
virtual	~EksBug (void);
virtual	Bool					Play (void);
virtual	void					SetTarget (const PiecePtr &piece);												//	set the target piece
				void					SetParent (Player *parent);
virtual	void					RegisterDeath (int type);
virtual void					SetSounds(int ekthrust, int ekanger);
};

//------------------------------------------------------------------------------
inline	void	EksBug::SetParent (Player *paren)
{
	parent = paren;
}

//------------------------------------------------------------------------------

#endif // EKS_BUG