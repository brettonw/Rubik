//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Last Revision:	08/20/96
//
//------------------------------------------------------------------------------

#ifndef SHIP_UPGRADER
#define SHIP_UPGRADER

//------------------------------------------------------------------------------
#include "device.h"

//------------------------------------------------------------------------------
class ShipUpgrader : public Device																							//	a resource generator
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
		int		type;																																	//	resource type
		void	Upgrade (void);																												//	eject a piece
	public:																																				//	members available externally
				ShipUpgrader (int typ, int id, Point_2d pos);														//	constructor
virtual	~ShipUpgrader (void);																										//	destructor
virtual	int		Trigger (const PiecePtr &piece, int code, int code2);							//	trigger the device
};																																							//	end

//------------------------------------------------------------------------------

#endif // SHIP_UPGRADER