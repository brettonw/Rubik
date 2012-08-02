//------------------------------------------------------------------------------
//	condition node pointer node
//	copyright ©1997 qualia, inc. All rights reserved.
//
//	02/25/97	DS		Created to avoid link problems
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef	CPNODE
#define CPNODE

//------------------------------------------------------------------------------
#include "piece ptr.h"
#include "condition node.h"

//------------------------------------------------------------------------------
class CPNode : public ListNode																									// For queuing pointers to condition nodes
{	
	protected:
		CondNode		*cnPtr;																													// pointer to a condition node
		PiecePtr		piece;
	public:
		CPNode (CondNode* node, const PiecePtr &p);																	// constructor
		void				SendTriggers (void);
		virtual			~CPNode (void);																									// virtual destructor
};

//------------------------------------------------------------------------------
inline CPNode::CPNode (CondNode *node, const PiecePtr &p)												// constructor
{
	cnPtr = node;
	piece = p;
}

//------------------------------------------------------------------------------
inline void CPNode::SendTriggers (void)																					// cpnode triggers
{
	cnPtr->SendTriggers(piece);																										// pass the piece along to CondNode
}

//------------------------------------------------------------------------------
#endif