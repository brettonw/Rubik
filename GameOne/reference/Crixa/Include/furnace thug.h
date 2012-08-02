//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Revisions:			01/31/97: creation.  (JB)
//
//------------------------------------------------------------------------------

#ifndef FURNACE_THUG
#define FURNACE_THUG

//------------------------------------------------------------------------------
#include "player.h"
#include "furnace.h"
#include "piece ptr.h"
#include "device.h"
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
class FurnaceThug : public Player, public Device																//	a flame throwing furnace
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				PiecePtr	furnace;																											//	pointer to the turret belonging to this thug
				Bool			trigger;
	public:																																				//	members available externally
				FurnaceThug (CPoint tLoc, Real rLoc, Real rad, SArrayPtr spr, int trig);//	constructor
virtual	~FurnaceThug (void);																										//	destructor
virtual	Bool			Play (void);																									//	play one frame
virtual	int				Trigger (const PiecePtr &piece, int code, int code2);					//	trigger the device
};																																							//	end

//------------------------------------------------------------------------------

#endif // BLAST_FURNACE_THUG
