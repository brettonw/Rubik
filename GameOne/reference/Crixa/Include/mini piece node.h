//------------------------------------------------------------------------------
//	Copyright ©1997 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Revisions:			02/19/97: creation. (JB)
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef	MINI_PIECE_NODE
#define MINI_PIECE_NODE

//------------------------------------------------------------------------------
#include "list node.h"

//------------------------------------------------------------------------------
class MiniPieceNode : public ListNode
{
	private:
	protected:
	public:
				PiecePtr		piece;
				MiniPieceNode (const PiecePtr &piec) {piece = piec;}
};

//------------------------------------------------------------------------------

#endif // MINI_PIECE_NODE

