//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	12/19/96
//
//------------------------------------------------------------------------------

#ifndef	SHIP_EXPLOSION_SEQUENCE
#define	SHIP_EXPLOSION_SEQUENCE

//------------------------------------------------------------------------------
#include "explosion sequence.h"
#include "piece ptr.h"
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
class ShipExpl : public ExplSeq																									//	an explosion sequence for a ship
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				int				elapsed;																											//	how many frames have elapsed since the beginning of the explosion
				int				particles;																										//	how many particles to blast out in a big pop
				SArrayPtr	big[2];
				SArrayPtr	little[4];
	public:																																				//	members available externally
  			ShipExpl (const PiecePtr &ship);																				//	constructor
virtual	~ShipExpl (void);																												//	destructor
virtual	void			Pop (void);																										//	make a new pop
				void			BigPop (void);																								//	a big lobe in the explosion
				void			SmallPop (void);																							//	a small lobe in the explosion
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	SHIP_EXPLOSION_SEQUENCE
