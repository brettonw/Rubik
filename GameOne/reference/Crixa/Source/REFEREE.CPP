//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/17/97
//
//	03/11/97	DS		Commented function headers
//	03/18/97	DS		added master clock
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h" 
#endif

//------------------------------------------------------------------------------
#include "referee.h"
#include "naive.h"
#include "link node.h"
#include "debugger.h"

//------------------------------------------------------------------------------
Referee	*gReferee = 0;																													//	global referee

//------------------------------------------------------------------------------
Referee::Referee (Real ival) : table (HashInt, 128)															//	constructor
{																																								//	begin
	interval = ival;																															//	copy the clock interval
	clock = ZERO;																																	//	zero the clock
	masterclock = ZERO;
	eventCount = 0;																																//	zero the event counter
	events = new Naive;																														//	allocate the priority queue
}																																								//	end

//------------------------------------------------------------------------------
Referee::~Referee (void)																												//	destructor
{																																								//	begin
	delete events;																																//	free up the priority queue
}																																								//	end

//------------------------------------------------------------------------------
// ResetClock (void) - private called by Step (void)
void	Referee::ResetClock (void)																								//	reset the world clock to insure resolution
{																																								//	begin
	events->Modify (-clock);																											//	subtract the current clock from all the intersection records
	clock = ZERO;																																	//	zero the clock
}																																								//	end

//------------------------------------------------------------------------------
// FindNode (PiecePtr, List, PieceNode) - private
void	Referee::FindNode (const PiecePtr &piece, List *&list, PieceNode *&node)	//	return a piece node based on the piece
{																																								//	begin
	if (piece ())																																	//	if the piece ptr contains a real pointer
	{																																							//	begin
		list = table.Fetch (piece->GetId ());																				//	get a pointer to the list appropriate for this piece
		PieceNode	*current = (PieceNode*) list->Head ();														//	start at the head of the list
		while (current)																															//	while there are nodes in the list
			if (current->piece () == piece ())																				//	if thie current node is the one we are looking for
				break;																																	//	loop no more
			else																																			//	otherwise
				current = (PieceNode*) list->Next (current);														//	skip on to the next node
		node = current;																															//	set the node to the current node
	}																																							//	end
	else																																					//	otherwise
		node = 0;																																		//	just null the node pointer
}																																								//	end

//------------------------------------------------------------------------------
// Remove (PieceNode) - private called from Remove (void)
void	Referee::Remove (PieceNode *piece)																				//	remove all the links from a removed piece
{																																								//	begin
	LinkNode	*current = (LinkNode *) piece->links.Head ();												//	get the ehad of the links list
	while (current)																																//	while there are links
	{																																							//	begin
		PieceNode	*pair = current->intersection->piece[0];													//	get a pointer to the paired piece for the intersection
		if (pair == piece)																													//	if this is the same piece we already have
			pair = current->intersection->piece[1];																		//	get the other pointer from the intersection
		pair->links.Remove (current->pair);																					//	remove the link pair from other piece (the one that points to here)
		delete current->pair;																												//	free the paired link node
		if (current->intersection->update)																					//	if the update flag is set
			update.Remove (current->intersection);																		//	remove the intersection from the update list
		else																																				//	otherwise, the intersection is in the event queue
			events->Remove (current->intersection);																		//	remove the intersection from the event queue
		delete current->intersection;																								//	free the intersection record
		current = (LinkNode *) piece->links.Next (current);													//	move to the next link
	}																																							//	end
	delete piece;																																	//	free the piece, this will automatically free the link list
}																																								//	end

//------------------------------------------------------------------------------
// Remove (void) - private called from Step (void)
void	Referee::Remove (void)																										//	remove the pieces that are waiting
{																																								//	begin
	PieceNode	*current = (PieceNode *) remove.Head ();														//	get the head of the remove list
	while (current)																																//	loop over all of the nodes in the remove list
	{																																							//	begin
		remove.Remove (current);																										//	remove the piece node from the remove list
		Remove (current);																														//	get rid of the node and all of its links
		current = (PieceNode *) remove.Head ();																			//	get the next node
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
// Update (PieceNode) - private called from Update (PiecePtr)
void	Referee::Update (PieceNode *piece)																				//	update the collision times for a piece
{																																								//	begin
	LinkNode	*current = (LinkNode *) piece->links.Head ();												//	get the head of the links list
	while (current)																																//	loop over all of the nodes in the list
	{																																							//	begin
		if (!current->intersection->update)																					//	if the intersection node is not already in the update list
		{																																						//	begin
			events->Remove (current->intersection);																		//	remove the intersection node from the event queue
			update.InsertAfter (current->intersection);																//	insert it into the update list
			current->intersection->update = TRUE;																			//	set the update flag
		}																																						//	end
		current = (LinkNode *) piece->links.Next (current);													//	advance to the next link
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
// Update (void) - private called from Step (void)
void	Referee::Update (void)																										//	update the events that have been flagged
{																																								//	begin
	INode	*current = (INode *) update.Head ();																		//	get the head of the update list
	while (current)																																//	loop over all of the nodes in the update list
	{																																							//	begin
		update.Remove (current);																										//	remove the node from the update list
		current->Update ();																													//	update the intersection node
		events->Insert (current);																										//	insert it into the event queue
		current = (INode *) update.Head ();																					//	get the next node
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
// PreCollide (INode) - private called from Step (void)
Bool	Referee::PreCollide (INode *event)																				//	precollide the pair
{																																								//	begin
	PieceNode	*piece0 = event->piece[0],																					//	get piece 0 from the event, since the event might be deleted during our work
						*piece1 = event->piece[1];																					//	get piece 1 from the event
	Sect			*sect = &(event->sect);																							//	get a pointer to the intersection record
	int				result0 = piece0->piece->PreCollide (piece1->piece, sect),					//	pre collide piece 0
						result1 = piece1->piece->PreCollide (piece0->piece, sect);					//	pre collide piece 1
	return ((result0 & PIECE_UPDATE) && (result1 & PIECE_UPDATE)) ? TRUE : FALSE;	//	return true if both pieces are still alive
}																																								//	end

//------------------------------------------------------------------------------
// PostCollide (INode) - private called from Step (void)
void	Referee::PostCollide (INode *event)																				//	postcollide the pair
{																																								//	begin
	PieceNode	*piece0 = event->piece[0],																					//	get piece 0 from the event, since the event might be deleted during our work
						*piece1 = event->piece[1];																					//	get piece 1 from the event
	Sect			*sect = &(event->sect);																							//	get a pointer to the intersection record
	int				result0 = piece0->piece->PostCollide (piece1->piece, sect),					//	post collide piece 0
						result1 = piece1->piece->PostCollide (piece0->piece, sect);					//	post collide piece 1
	if (!(result0 & PIECE_ALIVE))																									//	if piece 0 is not still alive
		Remove (piece0->piece);																											//	remove the piece
	else if (result0 & PIECE_UPDATE)																							//	otherwise if piece 0 should be updated
		Update (piece0);																														//	update the piece
	if (!(result1 & PIECE_ALIVE))																									//	if piece 1 is not still alive
		Remove (piece1->piece);																											//	remove the piece
	else if (result1 & PIECE_UPDATE)																							//	if piece 1 should be updated
		Update (piece1);																														//	update the piece
}																																								//	end

//------------------------------------------------------------------------------
// Step (Real) - private called from Step(void)
void	Referee::Step (Real time)																									//	step all of the pieces the specified time
{																																								//	begin
	for (int i = 0; i < table.Size (); i++)																				//	loop over all of the lists in the hash table
	{																																							//	begin
		List	*list = table[i];																											//	get the ith list
		PieceNode	*current = (PieceNode *) list->Head ();														//	get the head of the list
		while (current)																															//	loop over all of the pieces in the list
		{																																						//	begin
			PieceNode	*next = (PieceNode *) list->Next (current);											//	get the next piece in the list
			if (!current->piece->Step (time))																					//	step the piece the specified time, if it dies
			{																																					//	begin
				list->Remove (current);																									//	remove the piece from the list
				remove.InsertAfter (current);																						//	add it to the remove list
			}																																					//	end
			current = next;																														//	go on to the next piece
		}																																						//	end
	}																																							//	end
	clock += time;																																//	advance the world clock
	masterclock += time;																													//	ds: advance the master clock
}																																								//	end

//------------------------------------------------------------------------------
// STEP -- PUBLIC INTERFACE called by main game loop
void	Referee::Step (void)																											//	step the world ahead one frame
{																																								//	begin
	if (clock > R(6.0e2))																													//	if the clock has surpassed 600 seconds (10 minutes)
		ResetClock ();																															//	reset the clock to insure resolution
	Real	remaining = interval;																										//	set up the interval loop
	int		zeroCount = 0;
	while (remaining > ZERO)																											//	loop until the frame time has expired
	{																																							//	begin
		Update ();																																	//	update the events that need it
		INode	*event = (INode *) events->Peek ();																		//	peek at the top event in the queue
		Real	itime;																																//	an interval time
		if (event)																																	//	if there is a event
			itime = event->Value () - clock;																					//	set the interval time to be the time from now until the event
		else																																				//	otherwise
			itime = remaining;																												//	the interval time is the rest of the frame
		if (itime < remaining)																											//	if the event occurs before the end of the frame
		{																																						//	begin
			if (itime < EPSILON)																											//	if the intersection time is very small
				itime = ZERO;																														//	zero it to avoid errors
			Step (itime);																															//	step the world up to the event
			eventCount++;
			if (PreCollide (event))																										//	pre-collide the pieces, if both remain
				event->DoCollide ();																										//	compute the collision impulses for the two bodies
			PostCollide (event);																											//	post collide the pieces
			if (itime < R(1.0e-4))																										//	if it's a small timestep
			{
				if (++zeroCount > 8)																										//	and we've had more than 8 of them in this interval
				{
					for (int i = 0; i < 2; i++)																						//	take action to quiet things down!
					{
						Body	*body = event->piece[i]->piece->GetBody ();										//	if piece has a valid body
						if (body)
							body->AddVelocity (-body->TVelocity (), -body->RVelocity ());			//	then zero the velocity
					}
					itime = R(1.0e-4);
				}
			}
			else
				zeroCount = 0;
			remaining -= itime;																												//	subtract the elapsed time from the frame time
		}																																						//	end
		else																																				//	otherwise, any events happen after this frame
		{																																						//	begin
			Step (remaining);																													//	step the world through the rest of the frame
			remaining = ZERO;																													//	end the loop
		}																																						//	end
		Remove ();																																	//	remove pieces that are waiting
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
// INSTALL - PUBLIC INTERFACE
void	Referee::Install (const PiecePtr &piece, int flags)												//	install a new piece in the world
{																																								//	begin
	PieceNode	*pieceNode = new PieceNode (piece, flags);													//	make a new piece node for the new piece
	for (int i = 0; i < table.Size (); i++)																				//	loop over all of the lists in the hash table
	{																																							//	begin
		List			*list = table[i];																									//	get the ith piece list
		PieceNode	*current = (PieceNode *) list->Head ();														//	get the head of the piece list
		while (current)																															//	loop over all of the pieces in the list
		{																																						//	begin
			if ((flags & FULL_INTERSECT) || (current->flags & FULL_INTERSECT))				//	if either object has the intersection flag set
				if ((flags & MOBILE_INTERSECT) || (current->flags & MOBILE_INTERSECT))	//	if either object is mobile
				{																																				//	begin
					INode			*intersection = new INode (pieceNode, current);							//	create a new intersection record for the pair of objects
					LinkNode	*linkA = new LinkNode (intersection),												//	create a new link record for the existing object
										*linkB = new LinkNode (intersection);												//	create a new link record for the new object
					linkA->pair = linkB;																									//	set the link pair
					linkB->pair = linkA;																									//	set the link pair
					current->links.InsertAfter (linkA);																		//	insert the link into the existing piece record
					pieceNode->links.InsertAfter (linkB);																	//	insert the link into the new piece record
					update.InsertAfter (intersection);																		//	insert the intersection into the event queue
				}																																				//	end
			current = (PieceNode *) list->Next (current);															//	advance to the next piece
		}																																						//	end
	}																																							//	end
	List	*list = table.Fetch (piece->GetId ());																	//	get a pointer to the list appropriate for this piece
	list->InsertAfter (pieceNode);																								//	insert the piece record in the piece list
}																																								//	end

//------------------------------------------------------------------------------
// UPDATE - PUBLIC INTERFACE
void	Referee::Update (const PiecePtr &piece)																		//	update the collision times for a piece
{																																								//	begin
	List	*list;																																	//	list pointer
	PieceNode	*node;																															//	node pointer
	FindNode (piece, list, node);																									//	find the node and list that the piece is stored in
	if (node)																																			//	if a valid node was found
		Update (node);																															//	update it
}																																								//	end

//------------------------------------------------------------------------------
// REMOVE - PUBLIC INTERFACE
void	Referee::Remove (const PiecePtr &piece)																		//	remove an existing piece from the world
{																																								//	begin
	List	*list;																																	//	list pointer
	PieceNode	*node;																															//	node pointer
	FindNode (piece, list, node);																									//	find the node and list that the piece is stored in
	if (node)																																			//	if a valid node was found
	{																																							//	begin
		list->Remove (node);																												//	remove the piece from the list
		remove.InsertAfter (node);																									//	add it to the remove list
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
// REMOVE - PUBLIC INTERFACE
Real	Referee::TraceRay (CPoint origin, CVector direction, PiecePtr &piece)			//	find the first piece struck by a ray
{																																								//	begin
	PiecePtr	firstPiece;																													//	place to save the first piece hit by the ray
	Real			firstTime = INFINITY;																								//	place to save the time of the first hit
	for (int i = 0; i < table.Size (); i++)																				//	loop over all of the lists in the hash table
	{																																							//	begin
		List	*list = table[i];																											//	get the ith list
		PieceNode	*current = (PieceNode *) list->Head ();														//	get the head of the list
		while (current)																															//	loop over all of the pieces in the list
		{																																						//	begin
			if (current->flags & FULL_INTERSECT)																			//	if the current object is intersectable
			{																																					//	begin
				Real	time = current->piece->TraceRay (origin, direction);							//	intersect the ray with the current piece
				if (time < firstTime)																										//	if this intersection occurs before any that we have already found
				{																																				//	begin
					firstTime = time;																											//	save the time
					firstPiece = current->piece;																					//	save the piece
				}																																				//	end
			}																																					//	end
			current = (PieceNode *) list->Next (current);															//	get the next piece in the list
		}																																						//	end
	}																																							//	end
	piece = firstPiece;																														//	copy the piece pointer out
	return firstTime;																															//	and return the intersection time
}																																								//	end

//------------------------------------------------------------------------------
// CONTAINS - PUBLIC INTERFACE
Bool	Referee::Contains (CPoint location, PiecePtr &piece)											//	find the piece which contains the location point
{																																								//	begin
	for (int i = 0; i < table.Size (); i++)																				//	loop over all of the lists in the hash table
	{																																							//	begin
		List	*list = table[i];																											//	get the ith list
		PieceNode	*current = (PieceNode *) list->Head ();														//	get the head of the list
		while (current)																															//	loop over all of the pieces in the list
		{																																						//	begin
			if (current->flags & FULL_INTERSECT)																			//	if the current object is intersectable
				if (current->piece->Contains (location))																//	if the curent piece contains the location
				{																																				//	begin
					piece = current->piece;																								//	save the piece pointer
					return TRUE;																													//	return true to indicate containment
				}																																				//	end
			current = (PieceNode *) list->Next (current);															//	get the next piece in the list
		}																																						//	end
	}																																							//	end
	return FALSE;																																	//	return false to indicate no containing pieces
}																																								//	end

//------------------------------------------------------------------------------
