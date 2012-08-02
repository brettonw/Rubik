//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	09/06/96
//
//	03/20/97	DS		Modified UnloadMemory() to accept a "desired size"
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "controller.h"

//------------------------------------------------------------------------------
Controller	*Controller::controller = 0;																				//	the main controller
Controller	*Controller::controllers[NUM_CONTROLLERS] = {0};										//	the main controller list

//------------------------------------------------------------------------------
void	Controller::UnloadMemory (int size)																				//	see how big the memory space is and try to unload as much of it as possible
{																																								//	begin
	bool cycle = (size != 1);																											//	set flag for default behavior
	char	*ptr = new char[size * 1024 * 1024];																		//	allocate the memory
	while (ptr)																																		//	loop as long as the allocation succeeds
	{																																							//	begin
		for (int i = 0; i < size; i++)																							//	loop over each meg of the allocated memory
			*(ptr + (i * 1024 * 1024)) = 0;																						//	touching the first byte to be sure VM is loaded
		delete[] ptr;																																//	delete the block
		if (cycle) break;
		size++;																																			//	increment the size counter
		ptr = new char[size * 1024 * 1024];																					//	allocate a new block
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
Controller::Controller (HWND wind)																							//	constructor
{																																								//	begin
	window = wind;																																//	save the controller window
	next = QUIT_CONTROLLER_ID;																										//	default to quit
}																																								//	end

//------------------------------------------------------------------------------
void	Controller::Begin (void)																									//	set up the controller
{																																								//	begin
	UnloadMemory (32);																														//	unload the memory space
}																																								//	end

//------------------------------------------------------------------------------
void	Controller::End (void)																										//	clean up the controller
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	Controller::Switch (int id)																								//	jumping off point for button actions
{																																								//	begin
	if (controller)																																//	if there is a valid controller installed
		controller->End ();																													//	tell the controller to stop what it is doing
	controller = controllers[id];																									//	install the new controller
	if (controller)																																//	if this is a valid controller
		controller->Begin ();																												//	tell it to start up
}																																								//	end

//------------------------------------------------------------------------------
