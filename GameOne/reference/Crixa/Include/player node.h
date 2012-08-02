//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/17/96
//
//------------------------------------------------------------------------------

#ifndef	PLAYER_NODE
#define	PLAYER_NODE

//------------------------------------------------------------------------------
#include "list node.h"
#include "player.h"

//------------------------------------------------------------------------------
class PlayerNode : public ListNode																							//	a thug player
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
				Player		*player;																											//	pointer to the player
	public:																																				//	members available externally
				PlayerNode (Player *player);																						//	constructor
virtual	~PlayerNode (void);																											//	destructor
};																																							//	end

//------------------------------------------------------------------------------
inline	PlayerNode::PlayerNode (Player *play)																		//	constructor
{																																								//	begin
	player = play;																																//	copy the player
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	PLAYER_NODE