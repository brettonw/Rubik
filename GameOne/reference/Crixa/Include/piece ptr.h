//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/20/96
//
//------------------------------------------------------------------------------

#ifndef	PIECE_PTR
#define	PIECE_PTR

//------------------------------------------------------------------------------
#include "piece.h"

//------------------------------------------------------------------------------
class PiecePtr																																	//	reference counted pointer to a piece
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
  			Piece				*ptr;																												//	pointer to the sequence
	public:																																				//	members available externally
  			PiecePtr (void);																												//	constructor
  			PiecePtr (Piece *piece);																								//	constructor
  			PiecePtr (const PiecePtr &ptr);																					//	constructor
  			~PiecePtr (void);																												//	destructor
				Piece				*operator -> (void) const;																	//	pointer dereference operator
				Piece				*operator () (void) const;																	//	function call operator
				PiecePtr		&operator = (const PiecePtr &ptr);													//	assignment operator
};																																							//	end

//------------------------------------------------------------------------------
inline	Piece	*PiecePtr::operator -> (void) const																//	pointer dereference operator
{																																								//	begin
	return ptr;																																		//	return the pointer
}																																								//	end

//------------------------------------------------------------------------------
inline	Piece	*PiecePtr::operator () (void) const																//	function call operator
{																																								//	begin
	return ptr;																																		//	return the pointer
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	PIECE_PTR