//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Revisions:			02/05/97
//
//------------------------------------------------------------------------------

#ifndef	FURNACE_BLASTER
#define	FURNACE_BLASTER

//------------------------------------------------------------------------------
#include "plasma gun.h"

//------------------------------------------------------------------------------
class FurnaceBlaster : public PlasmaGun																					//	a fixed-direction, simplified plasma gun
{																																								//	begin
	private:																																			//	private members
	protected:																																		//	protected members
				int		numBubbles;																												//	the number of ejected bubbles each time the furnace erupts
	public:																																				//	public members
				FurnaceBlaster (Real ran, const PiecePtr &own, CPoint muz, Real s,			//	constructor
												Real d, SArrayPtr plas, int snd);												//	constructor (continued)
virtual	~FurnaceBlaster (void);																									//	destructor
virtual	void	Shoot (void);																											//	blast the furnace
};																																							//	end

//------------------------------------------------------------------------------

#endif // FURNACE_BLASTER
