//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	09/06/96
//
//	03/20/97	DS		UnloadMemory now takes a parameter
//------------------------------------------------------------------------------

#ifndef	CONTROLLER
#define	CONTROLLER

//------------------------------------------------------------------------------
#define	QUIT_CONTROLLER_ID		0																									//	the id of the quit controller
#define	NUM_CONTROLLERS				10																								//	the maximum number of controllers to register

//------------------------------------------------------------------------------
class	Controller																																//	control function class
{																																								//	begin
	private:																																			//	members internal to this class only
static	Controller	*controller;																								//	the current controller
				void				UnloadMemory (int size = 1);																//	see how big the memory space is and try to unload as much of it as possible
	protected:																																		//	members internal to this class hierarchy
				int					next;																												//	id of the next controller
static	Controller	*controllers[NUM_CONTROLLERS];															//	array of controllers
				HWND				window;																											//	the window that will own the direct draw device
	public:																																				//	members available externally
				Controller (HWND window);																								//	constructor
virtual	void				Begin (void);																								//	set up the controller
virtual	void				Loop (void) = 0;																						//	do one iteration of a control loop
virtual	void				End (void);																									//	clean up the controller
static	Bool				GetControl (void);																					//	do one iteration of the control loop for the controller
static	void				Switch (int id);																						//	function to switch the controller from a button press
				void				Switch (void);																							//	switch to the next controller in the series
				void				SetNext (int id);																						//	set the id of the next controller
};																																							//	end

//------------------------------------------------------------------------------
inline	Bool	Controller::GetControl (void)																			//	do one iteration of the control loop for the controller
{																																								//	begin
	if (controller)																																//	if a controller is installed
	{																																							//	begin
		controller->Loop ();																												//	get the current controller to do one iteration of the control loop
		return TRUE;																																//	return true to indicate successful control transfer
	}																																							//	end
	return FALSE;																																	//	return false to indicate failure to transfer control
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Controller::Switch (void)																					//	switch to the next controller in the series
{																																								//	begin
	Switch (next);																																//	switch to the next controller
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Controller::SetNext (int id)																			//	set the id of the next controller
{																																								//	begin
	next = id;																																		//	save the id of the next controller in sequence
}																																								//	end

//------------------------------------------------------------------------------

#endif	//CONTROLLER
