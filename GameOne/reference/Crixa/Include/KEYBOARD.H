//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	12/18/96
//
//------------------------------------------------------------------------------

#ifndef	KEYBOARD
#define	KEYBOARD

//------------------------------------------------------------------------------
class Keyboard																																	//	keyboard input class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				uchar			keyboard[256];																								//	the keyboard state
	public:																																				//	members available externally
				Keyboard (void);																												//	constructor
				Bool			GetKey (int key);																							//	get a key status
				void			SetKey (int key, Bool state);																	//	set a key status
				void			Update (void);																								//	get the keyboard state
};																																							//	end

//------------------------------------------------------------------------------
#define	VK_UNUSEDKEY	0																													//	virtual key code for an unused key

//------------------------------------------------------------------------------

#endif	//	KEYBOARD