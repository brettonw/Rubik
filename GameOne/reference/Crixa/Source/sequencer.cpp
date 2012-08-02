//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	07/26/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "sequencer.h"
#include "referee.h"
#include "area.h"

//------------------------------------------------------------------------------
class	SeqNode : public ListNode																									//	private structure for maintaining sequence information
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
				SeqPtr	sequence;																												//	the sequence associated with this node
				Real		begin;																													//	how long before the sequence gets begun
	public:																																				//	members available externally
  			SeqNode (void) {}																												//	constructor
virtual	~SeqNode (void);																												//	destructor
};																																							//	end

//------------------------------------------------------------------------------
SeqNode::~SeqNode (void)																												//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	Sequencer::Install (SeqPtr sequence, Real delay)													//	install a sequence
{																																								//	begin
	SeqNode	*node = new SeqNode;																									//	create a new node
	node->sequence = sequence;																										//	set the sequence pointer
	node->begin = delay;																													//	copy the delay to start
	sequences.InsertAfter (node);																									//	insert the node at the top of the list
}																																								//	end

//------------------------------------------------------------------------------
void	Sequencer::Step (CPoint where)																						//	step the sequencer
{																																								//	begin
	Point_2d	loc = (where * parallax) - gCenter;																	//	compute the parallax and screen center offsets
	loc.Floor ();																																	//	flatten the offset
	SeqNode	*node = (SeqNode *) sequences.Head ();																//	pointer to the first node in the list
	while (node)																																	//	while there are valid nodes in the layer list
	{																																							//	begin
		SeqNode	*next = (SeqNode *) sequences.Next (node);													//	save the next node
		node->begin -= gReferee->Interval ();																				//	subtract the frame time from the wait time
		if ((node->begin <= ZERO) && (!node->sequence->Step (loc)))									//	if the sequence is playing, and it was the last frame
		{																																						//	begin
			sequences.Remove (node);																									//	remove the sequence node
			delete node;																															//	free up the node memory
		}																																						//	end
		node = next;																																//	advance to the next node
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
