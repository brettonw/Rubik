//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/15/97
//
//------------------------------------------------------------------------------

#ifndef	PACKET
#define	PACKET

//------------------------------------------------------------------------------
#include "input code.h"

//------------------------------------------------------------------------------
class Packet																																		//	a network packet for game play
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
				char					id;																												//	the id of the player
				Code					code;																											//	the input code for the player
				Packet (void) {}																												//	constructor
				Packet (int id, const Code &code);																			//	constructor
};																																							//	end

//------------------------------------------------------------------------------
inline	Packet::Packet (int ident, const Code &cod) : code (cod)								//	constructor
{																																								//	begin
	id = ident;																																		//	copy the player id
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	PACKET