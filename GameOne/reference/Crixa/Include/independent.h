//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	07/27/96
//
//------------------------------------------------------------------------------

#ifndef	INDEPENDENT_PLAYER
#define	INDEPENDENT_PLAYER

//------------------------------------------------------------------------------
#include "player.h"
#include "list.h"

//------------------------------------------------------------------------------
class Independent : public Player																								//	an independent player in the game
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				List			players;																											//	the list of sub players
	public:																																				//	members available externally
				Independent (void);																											//	constructor
virtual	~Independent (void);																										//	destructor
virtual	Bool			Play (void);																									//	play one frame
				void			Install (Player *player);																			//	install a new sub player
};																																							//	end

//------------------------------------------------------------------------------
extern	Independent	*gIndependent;																							//	global independent player

//------------------------------------------------------------------------------

#endif	//	INDEPENDENT_PLAYER