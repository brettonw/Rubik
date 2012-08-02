//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/14/96
//
//------------------------------------------------------------------------------

#ifndef	STACK
#define	STACK

//------------------------------------------------------------------------------
#include "list node.h"

//------------------------------------------------------------------------------
class	Stack																																			//	file class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				ListNode	*head;																												//	pointer to the head of the stack
	public:																																				//	members available externally
				Stack (void);																														//	constructor
				~Stack (void);																													//	destructor
				ListNode	*Pop (void);																									//	return the head of the list
				void			Push (ListNode *node);																				//	insert a new node at the head of the list
				void			Reset (void);																									//	revert the list to an empty state
};																																							//	end

//------------------------------------------------------------------------------

#endif	//STACK