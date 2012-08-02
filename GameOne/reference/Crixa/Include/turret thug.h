//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/15/96
//
//------------------------------------------------------------------------------

#ifndef	TURRET_THUG
#define	TURRET_THUG

//------------------------------------------------------------------------------
#include "player.h"
#include "turret.h"
#include "piece ptr.h"
#include "device.h"

//------------------------------------------------------------------------------
class TurretThug : public Player, public Device																	//	a turret thug player
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				PiecePtr	target;																												//	the target piece
				PiecePtr	turret;																												//	pointer to the turret belonging to this thug
				Vector_2d	orient;																												//	default orientation of the turret
				Bool			triggered;																										//  (MK) trigger debounce flag
	public:																																				//	members available externally
				TurretThug (CPoint location, Real orientation, int type, int id);				//	constructor
virtual	~TurretThug (void);																											//	destructor
virtual	Bool			Play (void);																									//	play one frame
virtual	int				Trigger (const PiecePtr &piece, int code, int code2);					//	trigger the device
				void			SetTarget (const PiecePtr &piece);														//	set the target piece
};																																							//	end

//------------------------------------------------------------------------------
inline	void	TurretThug::SetTarget (const PiecePtr &piece)											//	set the target piece
{																																								//	begin
	target = piece;																																//	copy the target
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	TURRET_THUG