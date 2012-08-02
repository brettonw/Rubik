//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	07/27/96
//
//------------------------------------------------------------------------------

#ifndef	REPORTER
#define	REPORTER

//------------------------------------------------------------------------------
#include "referee.h"

//------------------------------------------------------------------------------
class Reporter																																	//	reporter class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
			int					index;																												//	index to the table of piece lists
			List				*curList;																											//	pointer to the current list
			PieceNode		*curNode;																											//	pointer to the current node in the current list
			Referee			*referee;																											//	pointer to the referee
			int					flags;																												//	the search constraint flags
	public:																																				//	members available externally
				Reporter (Referee *ref, int flags = NO_INTERSECT);											//	constructor
				void			Reset (void);																									//	reset the reporter's browse state
				PiecePtr	GetPiece (void);																							//	return the current piece
				void			Next (void);																									//	move to the next piece in the list
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	REPORTER
