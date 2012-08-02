//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Last Revision:	08/15/96
//
//------------------------------------------------------------------------------

#ifndef RESOURCE_GEN
#define RESOURCE_GEN

//------------------------------------------------------------------------------
#include "player.h"
#include "device.h"

//------------------------------------------------------------------------------
class ResourceGen : public Player, public Device																//	a resource generator
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
		int					type;																														//	resource type
		Point_2d		ejection;																												//	point of resource ejection
		Vector_2d		push;
		Real				direction;																											//	direction of resource ejection
		Real				orient;
		Real				delay;																													//	the delay time in between resource ejections
		Real				time;																														//	the current countdown
		void	Eject (void);																													//	eject a piece
	public:																																				//	members available externally
				ResourceGen (int typ, int id, Point_2d pos, Real force, Real del);			//	constructor
virtual	~ResourceGen ();																												//	destructor
virtual	int	Trigger (const PiecePtr &piece, int code, int code2);								//	trigger the device
virtual Bool Play (void);																														//	play
};																																							//	end

//------------------------------------------------------------------------------

#endif // RESOURCE_GEN