//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	10/04/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "dictionary.h"

//------------------------------------------------------------------------------
DNode	*Dictionary::Find (DNode *node, char key)																	//	find a node in a binary search tree
{																																								//	begin
	while (node && (node->key != key))																						//	loop so long as there is a valid node and we haven't found the target
		if (node->key > key)																												//	if the current node is higher than the target
			node = node->low;																													//	go down
		else																																				//	otherwise the current node is lower than the target
			node = node->high;																												//	go up
	return node;																																	//	return the found node
}																																								//	end

//------------------------------------------------------------------------------
DNode	*Dictionary::FindInsert (DNode *node, char key)														//	find a node in a binary search tree, or add one if it's not there
{																																								//	begin
	while (node->key != key)																											//	loop until the current node is the target
		if (node->key > key)																												//	if the current node is higher than the target
		{																																						//	begin
			if (!node->low)																														//	if there is no low node
				node->low = new DNode (key);																						//	make one
			node = node->low;																													//	go down
		}																																						//	end
		else																																				//	otherwise the current node is lower than the target
		{																																						//	begin
			if (!node->high)																													//	if there is no high node
				node->high = new DNode (key);																						//	make one
			node = node->high;																												//	go up
		}																																						//	end
	return node;																																	//	return the found node
}																																								//	end

//------------------------------------------------------------------------------
Dictionary::Dictionary (void)																										//	constructor
{																																								//	begin
	root = new DNode (0);																													//	create a sentinel node
}																																								//	end

//------------------------------------------------------------------------------
Dictionary::~Dictionary (void)																									//	destructor
{																																								//	begin
	delete root;																																	//	delete the sentinel
}																																								//	end

//------------------------------------------------------------------------------
void	Dictionary::AddKey (cstr key, int value)																	//	store a key/value pair
{																																								//	begin
	DNode	*node = root;																														//	get the sentinel node
	while (*key)																																	//	loop until the key has been exhausted
	{																																							//	begin
		if (node->next)																															//	if there is a node in the chain
			node = FindInsert (node->next, *key);																			//	find the one that matches the current key character, make one if necessary
		else																																				//	otherwise, there is no node in the chain
		{																																						//	begin
			node->next = new DNode (*key);																						//	make a new node
			node = node->next;																												//	move to it
		}																																						//	end
		key++;																																			//	advance the key
	}																																							//	end
	node->value = value;																													//	copy the value into the value field of the terminal node
}																																								//	end

//------------------------------------------------------------------------------
int		Dictionary::GetKey (cstr key)																							//	get the value associated with a key
{																																								//	begin
	DNode	*node = root;																														//	get the sentinel node
	while (*key)																																	//	loop until the key has been exhausted
	{																																							//	begin
		if (node->next)																															//	if there is a node in the chain
			node = Find (node->next, *key);																						//	find the one that matches the current key
		else																																				//	otherwise, there is no node in the chain
			return 0;																																	//	return 0 to indicate nothing found
		key++;																																			//	advance the key
	}																																							//	end
	return node->value;																														//	return the found value
}																																								//	end

//------------------------------------------------------------------------------
