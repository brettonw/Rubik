//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	09/05/96
//
//------------------------------------------------------------------------------

#ifndef	DIRECT_X
#define	DIRECT_X

//------------------------------------------------------------------------------
#include <mmsystem.h>
#include <dsound.h>
#include <ddraw.h>
#include "error mgr.h"

//------------------------------------------------------------------------------
class DirectX																																		//	the direct X system
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
static	LPDIRECTDRAW		draw;																										//	direct draw object
static	LPDIRECTDRAW2		draw2;																									//	direct draw 2 object
static	LPDIRECTSOUND		sound;																									//	direct sound object
static	void			Open (HWND window);																						//	open the direct X system
static	void			Close (void);																									//	close the direct X system
};																																							//	end

//------------------------------------------------------------------------------
inline	void	DXError (HRESULT result, cstr call, cstr file, int line)					//	handle an error from a direct draw function
{																																								//	begin
	if (result != NO_ERROR)																												//	if the result is an error
	{																																							//	begin
		SetLastError (result);																											//	set the error
		Error (call, file, line);																										//	bail out
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
#define	DXCheck(a)	Check (a, DXError)																					//	macro to facilitate error checking

//------------------------------------------------------------------------------

#endif	//	DIRECT_X
