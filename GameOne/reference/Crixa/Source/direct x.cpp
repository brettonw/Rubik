//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	09/06/96
//	Revision DS:		02/19/97	Added window debugging stuff
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include <objbase.h>
#include <initguid.h>
#include "direct x.h"
#include "vid debug.h"

//------------------------------------------------------------------------------
LPDIRECTDRAW		DirectX::draw;																									//	direct draw object
LPDIRECTDRAW2		DirectX::draw2;																									//	direct draw 2 object
LPDIRECTSOUND		DirectX::sound;																									//	direct sound object

//------------------------------------------------------------------------------
void	DirectX::Open (HWND w)																										//	initialize direct X
{																																								//	begin
	DXCheck (DirectDrawCreate (0, &draw, 0));																			//	create the direct draw object

#if CRIXA_DEBUG_HARMONY																											//	ds: want to run in debugger-friendly mode?
	DXCheck (draw->SetCooperativeLevel (w, DDSCL_NORMAL ));												//	ds: make a normal windows app
#else
	DXCheck (draw->SetCooperativeLevel (w, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN));	//	ds: make myself the exclusive owner of the direct draw object
#endif
  DXCheck (draw->QueryInterface (IID_IDirectDraw2, (LPVOID *) &draw2));					//	get a ddraw2 object
	DXCheck (DirectSoundCreate (0, &sound, 0));																		//	create the direct sound object
	DXCheck (sound->SetCooperativeLevel (w, DSSCL_EXCLUSIVE));										//	make myself the exclusive owner of the direct sound object
}																																								//	end

//------------------------------------------------------------------------------
void	DirectX::Close (void)																											//	close direct X
{																																								//	begin
	DXCheck (sound->Release ());																									//	release the direct sound object
	DXCheck (draw2->Release ());																									//	release the direct draw 2 object
	DXCheck (draw->Release ());																										//	release the direct draw object
}																																								//	end

//------------------------------------------------------------------------------
