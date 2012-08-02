//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	08/15/96	BW	last revision
//	02/18/97	DS	extension for "jam open" and "unjam"
//
//------------------------------------------------------------------------------

#ifndef	DOOR_PLAYER
#define	DOOR_PLAYER

//------------------------------------------------------------------------------
#include "player.h"
#include "piece ptr.h"
#include "device.h"

//------------------------------------------------------------------------------
enum	{	GENERIC = 0,																														//	special parameters used by door player on trigger events
				JAM_OPEN,												
				UNJAM														
			};

//------------------------------------------------------------------------------
struct	DoorPart																																//	structure describing door parts
{																																								//	begin
	PiecePtr	piece;																															//	pointer to the door piece
	Point_2d	lock;																																//	the fully closed and locked location of the door part
	Vector_2d	axis;																																//	constraint axis for the door part
	Real			maxPosition;																												//	the fully open position of the door part, along the constraint axis
	Real			maxVelocity;																												//	the maximum velocity for the door part
	Real			maxForce;																														//	the maximum force that can be applied to the door part
};																																							//	end

//------------------------------------------------------------------------------
class DoorPlayer : public Player, public Device																	//	a door player
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				int				count;																												//	the number of door pieces
				DoorPart	*part;																												//	pointer to an array of door parts
				Bool			open;																													//	whther or not the door should be open
				Bool			stayopen;																											//	used to jam the door open
				int				doorStart;
				int				doorStop;
	public:																																				//	members available externally
				DoorPlayer (CPoint location, int type, int id);													//	constructor
virtual	~DoorPlayer (void);																											//	destructor
virtual	Bool			Play (void);																									//	play one frame
virtual	int				Trigger (const PiecePtr &piece, int type, int what);					//	trigger the device
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	DOOR_PLAYER