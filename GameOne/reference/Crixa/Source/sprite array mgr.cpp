//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/30/97
//	02/28/97	DS		Added null sprite safety
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "sprite array mgr.h"

//------------------------------------------------------------------------------
SArrayMgr	gSpriteMgr (64);																											//	global sprite array manager with 64 slots

//------------------------------------------------------------------------------
class	SpriteNode : public ListNode																							//	sprite list node class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
				SArrayPtr		sprite;																											//	the sprite pointer
				char				name[13];																										//	the name of the sprite
				Bool				touch;																											//	whether or not the sprite array has been touched
				SpriteNode (cstr name);																									//	constructor
virtual	~SpriteNode (void);																											//	destructor
};																																							//	end

//------------------------------------------------------------------------------
inline	SpriteNode::SpriteNode (cstr nm)																				//	constructor
{																																								//	begin
	strcpy (name, nm);																														//	set the name
	sprite = SArrayPtr (name);																										//	load the sprite
	touch = TRUE;																																	//	set the touched flag
}																																								//	end

//------------------------------------------------------------------------------
SpriteNode::~SpriteNode (void)																									//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
SArrayMgr::SArrayMgr (int size) : table (HashText, size)												//	constructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
SArrayPtr	SArrayMgr::Fetch (cstr name, Bool save)																//	return the sprite array ptr associated with 'name'
{																																								//	begin
	List				*list = table.Fetch (name);																				//	get the indexed list from the hash table
	SpriteNode	*node = (SpriteNode *) list->Head ();															//	get the head of the list
	while (node)																																	//	loop over all of the nodes in the list
	{																																							//	begin
		if (strcmp (node->name, name) == 0)																					//	if this is the node we are looking for
		{																																						//	begin
			node->touch = TRUE;																												//	set the touch flag
			return node->sprite;																											//	return the sprite
		}																																						//	end
		node = (SpriteNode *) list->Next (node);																		//	skip to the next node
	}																																							//	end
	if (save)																																			//	we didn't find the node, if we want to save the sprite array pointer into the table
	{																																							//	begin
		node = new SpriteNode (name);																								//	create a new node
		if (node->sprite->Count())																									//	ds: if the sprite is valid
		{
			list->InsertAfter (node);																									//	insert the node into the list
			return node->sprite;																											//	return the sprite array pointer
		}
		else																																				//	otherwise, return the null sprite
			return Fetch("nullspr");
	}																																							//	end
	else																																					//	otherwise, we don't want to save the pointer
		return SArrayPtr (name);																										//	load the sprite and return the pointer
}																																								//	end

//------------------------------------------------------------------------------
void	SArrayMgr::UnTouch (void)																									//	untouch all of the loaded sprites
{																																								//	begin
	for (int i = 0; i < table.Size (); i++)																				//	loop over all of the lists in the table
	{																																							//	begin
		List				*list = table[i];																								//	get the indexed list from the hash table
		SpriteNode	*node = (SpriteNode *) list->Head ();														//	get the head of the list
		while (node)																																//	loop over all of the nodes in the list
		{																																						//	begin
			node->touch = FALSE;																											//	untouch the node
			node = (SpriteNode *) list->Next (node);																	//	skip to the next node
		}																																						//	end
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
void	SArrayMgr::CleanUp (void)																									//	unload all of the untouched sprite arrays
{																																								//	begin
	for (int i = 0; i < table.Size (); i++)																				//	loop over all of the lists in the table
	{																																							//	begin
		List				*list = table[i];																								//	get the indexed list from the hash table
		SpriteNode	*node = (SpriteNode *) list->Head ();														//	get the head of the list
		while (node)																																//	loop over all of the nodes in the list
		{																																						//	begin
			SpriteNode	*next = (SpriteNode *) list->Next (node);											//	get the next node
			if (node->touch == FALSE)																									//	if the node is not touched
			{																																					//	begin
				list->Remove (node);																										//	remove the node from the list
				delete node;																														//	free the node
			}																																					//	end
			node = next;																															//	skip to the next node
		}																																						//	end
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
