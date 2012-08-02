//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	10/08/96
//
//------------------------------------------------------------------------------

#ifndef	DICTIONARY_NODE
#define	DICTIONARY_NODE

//------------------------------------------------------------------------------
class	DNode																																			//	a node in a dictionary
{																																								//	begin
	private:																																			//	members internal to this class only
static	DNode			*pool;																												//	the intersection node pool
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
				int				value;																												//	the value stored in the node
				char			key;																													//	the key associated with the node
				DNode			*high;																												//	pointer to subtrees that go up the alphabet
				DNode			*low;																													//	pointer to subtrees that go down the alphabet
				DNode			*next;																												//	pointer to the next subtree in the chain
	public:																																				//	members available externally
				DNode (char key);																												//	constructor
				~DNode (void);																													//	destructor
				void			*operator new (size_t size);																	//	free store allocation
				void			operator delete (void *ptr, size_t size);											//	free store recovery
static	void			PreAllocate (int number = 1000);															//	pre-allocate dictionary nodes
};																																							//	end

//------------------------------------------------------------------------------
inline	void	*DNode::operator new (size_t size)																//	free store allocation
{																																								//	begin
	if (!pool)																																		//	if there is no pool
		return ::operator new (size);																								//	make a new one and return it
	DNode	*node = pool;																														//	get the first node off the pool
	pool = pool->next;																														//	skip the pool to the next one on the stack
	return node;																																	//	return the node
}																																								//	end

//------------------------------------------------------------------------------
inline	void	DNode::operator delete (void *ptr, size_t size)										//	free store recovery
{																																								//	begin
	((DNode *) ptr)->next = pool;																									//	push the deleted node onto the pool
}																																								//	end

//------------------------------------------------------------------------------

#endif	//DICTIONARY_NODE
