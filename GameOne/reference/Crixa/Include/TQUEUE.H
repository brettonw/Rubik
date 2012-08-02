//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/14/96
//
//------------------------------------------------------------------------------

#ifndef	TEMPLATE_QUEUE
#define	TEMPLATE_QUEUE

//------------------------------------------------------------------------------
#include "queue.h"
#include "stack.h"

//------------------------------------------------------------------------------
template <class T>																															//	template the following definition
class	TQNode : public ListNode																									//	node structure for the templated queue
{																																								//	begin
	public:																																				//	members available externally
				T				data;																														//	data field
				TQNode (const T &data);																									//	constructor
virtual	~TQNode (void);																													//	destructor
};																																							//	end

//------------------------------------------------------------------------------
template <class T>																															//	template the following definition
TQNode<T>::TQNode (const T &dat) : data (dat)																		//	constructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
template <class T>																															//	template the following definition
TQNode<T>::~TQNode (void)																												//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
template <class T>																															//	template the following definition
class	TQueue																																		//	templated queue class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Stack			pool;																													//	pool of nodes
				Queue			queue;																												//	the actual queue
	public:																																				//	members available externally
				T					*Dequeue (void);																							//	dequeue the head element
				void			Enqueue (const T &data);																			//	add a new element to the tail of the list
};																																							//	end

//------------------------------------------------------------------------------
template <class T>																															//	template the following definition
T		*TQueue<T>::Dequeue (void)																									//	dequeue the head element
{																																								//	begin
	TQNode<T>	*node = (TQNode<T> *) queue.Dequeue ();															//	dequeue a node from the queue
	if (node)																																			//	if the node is valid
	{																																							//	begin
		pool.Push (node);																														//	push the node onto the pool
		return &(node->data);																												//	return the data block of the node
	}																																							//	end
	return 0;																																			//	return a zero object if no object was found
}																																								//	end

//------------------------------------------------------------------------------
template <class T>																															//	template the following definition
void	TQueue<T>::Enqueue (const T &data)																				//	add a new element to the tail of the list
{																																								//	begin
	TQNode<T>	*node = (TQNode<T> *) pool.Pop ();																	//	get a node off the pool
	if (!node)																																		//	if that node is not valid
		node = new TQNode<T> (data);																								//	allocate a new node
	node->data = data;																														//	copy the data into the data block of the node
	queue.Enqueue (node);																													//	enqueue the node
}																																								//	end

//------------------------------------------------------------------------------

#endif	//TEMPLATE_QUEUE