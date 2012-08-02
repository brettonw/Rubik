//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/17/97
//	03/18/97	DS		Added master clock, for total elapsed time since referee
//									instance started
//
//------------------------------------------------------------------------------

#ifndef	REFEREE
#define	REFEREE

//------------------------------------------------------------------------------
#include "hash table.h"
#include "priority.h"
#include "intersection node.h"

//------------------------------------------------------------------------------
class Referee																																		//	game referee
{																																								//	begin
	private:																																			//	members internal to this class only
				void			Step (Real time);																							//	step all of the pieces the specified time
				void			FindNode (const PiecePtr &pc, List *&list, PieceNode *&node);	//	return a piece node based on the piece
				void			Remove (PieceNode *piece);																		//	remove all the links from a removed piece
				void			Remove (void);																								//	remove the pieces that are waiting
				void			Update (PieceNode *piece);																		//	update the collision times for a piece
				void			Update (void);																								//	update the events that are waiting
	protected:																																		//	members internal to this class hierarchy
				friend	class	Reporter;																									//	reporters have access to the piece list
				HashTable	table;																												//	table of piece lists
				List			update;																												//	list of intersection nodes waiting to be updated
				List			remove;																												//	list of piece nodes waiting to be removed
				Priority	*events;																											//	priority queue for events
				Real			clock;																												//	the game clock
				Real			masterclock;																									//	the master clock (never resets)
				Real			interval;																											//	the frame interval
				int				eventCount;																										//	the event count
	public:																																				//	members available externally
				Referee (Real interval);																								//	constructor
				~Referee (void);																												//	destructor
				void			Step (void);																									//	step the world ahead one frame
				Real			Clock (void);																									//	return the world time
				Real			MasterClock (void);																						//	return the elapsed time for this level
				Real			Interval (void);																							//	return the clock interval (frame time)
				void			SetInterval (Real interval);																	//	set the referee interval
				void			ResetClock (void);																						//	reset the world clock to insure resolution
				void			Install (const PiecePtr &piece, int flags = NORMAL_INTERSECT);//	install a new piece in the world
				void			Remove (const PiecePtr &piece);																//	remove an existing piece from the world
				void			Update (const PiecePtr &piece);																//	update the collision times for a piece
				Bool			Confirm (const PiecePtr &piece);															//	confirm the existence of a piece
				Bool			PreCollide (INode *intersection);															//	precollide the pair
				void			PostCollide (INode *intersection);														//	postcollide the pair
				Real			TraceRay (CPoint origin, CVector direction, PiecePtr &piece);	//	find the first piece struck by a ray
				Bool			Contains (CPoint location, PiecePtr &piece);									//	find the piece which contains the location point
				int				NextEvent (void);																							//	return the event count
};																																							//	end

//------------------------------------------------------------------------------
inline	Real	Referee::Clock (void)																							//	return the world time
{																																								//	begin
	return clock;																																	//	return the clock
}																																								//	end

//------------------------------------------------------------------------------
inline	Real	Referee::MasterClock (void)																				//	return the world time
{																																								//	begin
	return masterclock;																														//	return the clock
}																																								//	end

//------------------------------------------------------------------------------
inline	Real	Referee::Interval (void)																					//	return the clock interval (frame time)
{																																								//	begin
	return interval;																															//	return the interval
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Referee::SetInterval (Real val)																		//	set the referee interval
{																																								//	begin
	interval = val;																																//	set the interval
}																																								//	end

//------------------------------------------------------------------------------
inline	Bool	Referee::Confirm (const PiecePtr &piece)													//	confirm the existence of a piece
{																																								//	begin
	List			*list;																															//	list pointer
	PieceNode	*node;																															//	piece node pointer
	FindNode (piece, list, node);																									//	get the node containing the piece
	return node ? TRUE : FALSE;																										//	return true if the node is valid
}																																								//	end

//------------------------------------------------------------------------------
inline	int		Referee::NextEvent (void)																					//	return the event count
{																																								//	begin
	return eventCount;																														//	return the event count
}																																								//	end

//------------------------------------------------------------------------------
extern	Referee	*gReferee;																											//	global referee

//------------------------------------------------------------------------------

#endif	//	REFEREE
