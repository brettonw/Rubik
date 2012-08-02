//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Last Revision:	01/23/97
//
//	03/04/97	JB		added damage function for "flashy" damage effect.
//
//------------------------------------------------------------------------------

#ifndef	BOSS_GEN
#define BOSS_GEN

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#include "device.h"
#include "ship.h"
#include "sprite array mgr.h"

//------------------------------------------------------------------------------
class	GenDev : public Device
{
	private:
	protected:
				int			output;																													//	amount of power output (positive only)
	public:
				GenDev (int id);																												//	constructor
virtual	~GenDev (void);
virtual	int			Trigger (const PiecePtr &piece, int type, int what);
};

//------------------------------------------------------------------------------
//	class definition of a boss generator
//	a boss generator provides power, and is a destroyable piece
//------------------------------------------------------------------------------
class BossGen : public Ship																											//	class definition
{																																								//
	private:																																			//	private members
	protected:																																		//	protected members
				GenDev		*bGenDev;
				Real			dyingTime;
	public:																																				//	public members
				BossGen (Player *own, GenDev *dev, Real armor, Point_2d loc, Real radius, SArrayPtr spr);//	constructor
virtual	~BossGen (void);																												//	destructor
virtual	Bool			Step (Real time);
virtual	int				Hurt (const PiecePtr &piece, Real damage);
virtual	void			AddEffects (int check);
virtual	void			Explode (const PiecePtr &piece);
};																																							//

//------------------------------------------------------------------------------

#endif // BOSS_GEN
