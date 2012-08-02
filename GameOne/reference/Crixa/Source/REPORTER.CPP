//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	07/27/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "reporter.h"

//------------------------------------------------------------------------------
Reporter::Reporter (Referee *ref, int flgs)																			//	constructor
{																																								//	begin
	referee = ref;																																//	copy the referee pointer
	flags = flgs;																																	//	coy the search constraint flags
	Reset ();																																			//	reset the reporter
}																																								//	end

//------------------------------------------------------------------------------
void	Reporter::Reset (void)																										//	reset the reporter's browse state
{																																								//	begin
	index = -1;																																		//	start the index at -1
	curNode = 0;																																	//	set the current node to null
	while ((!curNode) && (index < referee->table.Mask ()))												//	loop over all of the lists until a valid list is found
	{																																							//	begin
		index++;																																		//	skip to the next list
		curList = referee->table[index];																						//	get a pointer to the list
		curNode = (PieceNode *) curList->Head ();																		//	get the head of the list
		if (flags)																																	//	if the constraint flags are set
			while (curNode && (!(curNode->flags & flags)))														//	loop until we find a node matching the criteria or the end of the list
				curNode = (PieceNode *) curList->Next (curNode);												//	advance to the next node
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
PiecePtr	Reporter::GetPiece (void)																							//	return the current piece
{																																								//	begin
	if (curNode)																																	//	if the current node is valid
		return curNode->piece;																											//	return the piece
	return 0;																																			//	return 0 to indicate no more valid nodes
}																																								//	end

//------------------------------------------------------------------------------
void	Reporter::Next (void)																											//	move to the next piece in the list
{																																								//	begin
	curNode = (PieceNode *) curList->Next (curNode);															//	advance to the next node
	if (flags)																																		//	if the constraint flags are set
		while (curNode && (!(curNode->flags & flags)))															//	loop until we find a node matching the criteria or the end of the list
			curNode = (PieceNode *) curList->Next (curNode);													//	advance to the next node
	while ((!curNode) && (index < referee->table.Mask ()))												//	loop over the remaining lists while the node is not valid
	{																																							//	begin
		index++;																																		//	skip to the next list
		curList = referee->table[index];																						//	get a pointer to the list
		curNode = (PieceNode *) curList->Head ();																		//	get the head of the list
		if (flags)																																	//	if the constraint flags are set
			while (curNode && (!(curNode->flags & flags)))														//	loop until we find a node matching the criteria or the end of the list
				curNode = (PieceNode *) curList->Next (curNode);												//	advance to the next node
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
