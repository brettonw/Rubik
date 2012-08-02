//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	07/28/96
//
//------------------------------------------------------------------------------

#ifndef	SHIPYARD
#define	SHIPYARD

//------------------------------------------------------------------------------
#include "player.h"
#include "list.h"
#include "regen piece node.h"

//------------------------------------------------------------------------------
enum DeviceType {	D_NONE, D_TURRET, D_DOOR, D_TELEPORT, D_LASERBARRIER, D_RESOURCEGEN,
									D_UPGRADER, D_DESTWALL, D_DESTBALL, D_RESOURCE, D_FORCEMAT, D_GENERIC };
enum PDeviceType 	{	P_NONE, P_GENERATOR, P_PIPE, P_SWITCH, P_MULTI, P_BUMPER, P_MAT };
enum ConnectType	{	TO_NONE, P_TO_P, P_TO_D };

//------------------------------------------------------------------------------
class Shipyard : public Player																									//	the shipyard player
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				List			players;																											//	the list of sub players
				List			regenList;																										//	number of regenerating walls
				RegenPieceNode	*rnode;																									//	current node we're inspecting
	public:																																				//	members available externally
				Shipyard (void);																												//	constructor
virtual	~Shipyard (void);																												//	destructor
virtual	Bool			Play (void);																									//	play one frame
};																																							//	end

//------------------------------------------------------------------------------
extern	Shipyard	*gShipyard;																										//	global shipyard player

//------------------------------------------------------------------------------

#endif	//	SHIPYARD