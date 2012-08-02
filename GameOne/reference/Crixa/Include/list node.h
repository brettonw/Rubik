//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/10/96
//
//------------------------------------------------------------------------------

#ifndef	LIST_NODE
#define	LIST_NODE

//------------------------------------------------------------------------------
class	ListNode																																	//	list node class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				friend	class	List;																											//	lists can play with my private parts
				friend	class	Stack;																										//	stacks can play with my private parts
				friend	class	Queue;																										//	queues can play with my private parts
				ListNode	*next;																												//	pointer to the next node in the list
				ListNode	*previous;																										//	pointer to the previous node in the list
	public:																																				//	members available externally
				ListNode (void) {}																											//	constructor
virtual	~ListNode (void);																												//	destructor
};																																							//	end

//------------------------------------------------------------------------------

#endif	//LIST_NODE