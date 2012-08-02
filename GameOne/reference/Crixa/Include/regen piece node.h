//------------------------------------------------------------------------------
//	copyright ©1997 qualia, inc. All rights reserved.
//
//	03/18/97	DS		Created new RegenPieceNode class
//------------------------------------------------------------------------------

#ifndef REGEN_PIECE_NODE
#define REGEN_PIECE_NODE
//------------------------------------------------------------------------------
#include "point.h"
#include "piece ptr.h"
#include "list node.h"
//------------------------------------------------------------------------------
class RegenPieceNode : public ListNode
{
	public:
		Real				regentime;						// the regeneration time
		Real				regenwhen;						// when to regenerate
		bool				regenerating;					// flag for regenerating mode


		PiecePtr		piece;								// the piece to monitor
		int					dtype;								// the device type (used for runtime checking)

		int					type;									// the device subtype
		Point_2d		location;							// the 

		cstr				fname;								// filename pointer (use assignment to statics only!)
		int					layer;								// layer for dwall
		Real				armor, width, height;	// other dwall things

		int					amount;								// amount for resources
		
								RegenPieceNode (PiecePtr pc, int typ, Point_2d loc, int am);
								RegenPieceNode (PiecePtr pc, int typ, Point_2d loc, cstr fn, Real ar, Real w, Real h, int lay);
		virtual			~RegenPieceNode (void);
		void				CheckPiece (void);
		void				RegenDWall (void);
		void				RegenDResource (void);
		void				SetRegenTime (Real time) { regentime = time; }

};

//------------------------------------------------------------------------------
#endif // REGEN_PIECE_NODE