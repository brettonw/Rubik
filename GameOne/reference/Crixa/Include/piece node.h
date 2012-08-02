//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/13/97
//
//------------------------------------------------------------------------------

#ifndef	PIECE_NODE
#define	PIECE_NODE

//------------------------------------------------------------------------------
#include "piece ptr.h"
#include "list.h"
#include "int vector.h"
#include "stack.h"

//------------------------------------------------------------------------------
enum	{																																					//	collision flags
				NO_INTERSECT = 0,																												//	do not intersect this object with others that also have no flags set
				FULL_INTERSECT = 1,																											//	intersect this object against all moving objects in the world
				MOBILE_INTERSECT = 2,																										//	this object moves, so intersect this object with all stationary objects in the world
				NORMAL_INTERSECT = 3,																										//	a combination of the last two
				GLOBAL_INTERSECT = 4																										//	this object is a global object, and should always be intersected with
			};																																				//	end collision flags

//------------------------------------------------------------------------------
class			PieceNode : public ListNode																						//	private structure for maintaining the piece list
{																																								//	begin
	private:																																			//	members internal to this class only
static	Stack			pool;																													//	the piece node pool
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
				PiecePtr	piece;																												//	pointer to the piece
				int				flags;																												//	flags for the piece node
				List			links;																												//	list of links for this piece
	public:																																				//	members available externally
				PieceNode (void) {}																											//	constructor
  			PieceNode (const PiecePtr &piece, int flags);														//	constructor
virtual	~PieceNode (void);																											//	destructor
				void			*operator new (size_t size);																	//	free store allocation
				void			operator delete (void *ptr, size_t size);											//	free store recovery
static	void			PreAllocate (int number = 300);																//	pre-allocate piece nodes
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	PIECE_NODE
