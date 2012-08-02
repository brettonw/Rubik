//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	10/04/96
//
//------------------------------------------------------------------------------

#ifndef	DICTIONARY
#define	DICTIONARY

//------------------------------------------------------------------------------
#include "dictionary node.h"

//------------------------------------------------------------------------------
class	Dictionary																																//	dictionary class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				DNode			*root;																												//	the root of the dictionary tree
				DNode			*Find (DNode *node, char key);																//	find a node in a binary search tree
				DNode			*FindInsert (DNode *node, char key);													//	find a node in a binary search tree, or add one if it's not there
	public:																																				//	members available externally
				Dictionary (void);																											//	constructor
virtual	~Dictionary (void);																											//	destructor
				void			AddKey (cstr key, int value);																	//	store a key/value pair
				int				GetKey (cstr key);																						//	get the value associated with a key
};																																							//	end

//------------------------------------------------------------------------------

#endif	//DICTIONARY
