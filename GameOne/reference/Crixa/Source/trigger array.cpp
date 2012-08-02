//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/21/96
//	Revision:				02/17/97	Added State Manager hook DS
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "trigger array.h"
#include	"state mgr.h"
#include	"debugger.h"

//------------------------------------------------------------------------------
TArray	*gTrigger;																															//	global trigger array

//------------------------------------------------------------------------------
class	TriggerNode : public ListNode																							//	trigger list node class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
				Device			*device;																										//	the device pointer
				TriggerNode (void) {}																										//	constructor
virtual	~TriggerNode (void);																										//	destructor
};																																							//	end

//------------------------------------------------------------------------------
TriggerNode::~TriggerNode (void)																								//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
TArray::TArray (int count)																											//	constructor
{																																								//	begin
	devices = new List[count];																										//	allocate the device list array
}																																								//	end

//------------------------------------------------------------------------------
TArray::~TArray (void)																													//	destructor
{																																								//	begin
	delete[] devices;																															//	free the device list array
}																																								//	end

//------------------------------------------------------------------------------
int		TArray::Trigger (int trigger, const PiecePtr &piece, int code, int code2)	//	trigger the device
{																																								//	begin
	TriggerNode	*node = (TriggerNode *) devices[trigger].Head ();									//	get the head of the requested trigger list
	while (node)																																	//	loop over all the nodes in the list
	{																																							//	begin
		node->device->Trigger (piece, code, code2);																	//	trigger the current device
		node = (TriggerNode *) devices[trigger].Next (node);												//	move to the next node
	}																																							//	end
	gStateMgr->Trigger (trigger, piece, code, code2);															//	ds: let state manager handle conditions
	return 0;																																			//	always return 0
}																																								//	end

//------------------------------------------------------------------------------
void	TArray::SetTrigger (int trigger, Device *device)													//	set a trigger device
{																																								//	begin
	if (trigger)																																	//	trigger zero is reserved, so if we are trying to set any trigger except 0
	{																																							//	begin
		TriggerNode	*node = new TriggerNode;																				//	allocate a new trigger node
		node->device = device;																											//	copy the devic epointer into the node
		devices[trigger].InsertAfter (node);																				//	insert the node in the list
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
