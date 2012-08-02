//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/20/97
//
//	Modified by:		Dave Seah
//	Last Revision:	01/23/97			Added EraseRect() call
//
//------------------------------------------------------------------------------

#ifndef	DISPLAY_8
#define	DISPLAY_8

//------------------------------------------------------------------------------
#include "surface.h"

//------------------------------------------------------------------------------
enum	{FRONT_BUFFER = 0, BACK_BUFFER};																					//	screen selection values

//------------------------------------------------------------------------------
class Display																																		//	display class for direct drawing
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				RECT								scrnRect;																						//	the rectangle of the full screen
				RECT								playRect;																						//	the rectangle of the play screen
				Real								rate;																								//	the display rate
				Surface							*current;																						//	currently selected surface
				int									currentIndex;																				//	array index of the current surface
				LPDIRECTDRAWSURFACE	primary;																						//	the primary surface
				LPDIRECTDRAWSURFACE	back;																								//	the back surface
				LPDIRECTDRAWPALETTE	palette;																						//	8-bit color palette
				LPDIRECTDRAWPALETTE	redpalette;																					//	8-bit color palette, red channel only
				LPDIRECTDRAWPALETTE	syspalette;																					//	pointer to the system palette
public:																																					//	members available externally
  			Display (Bool timeRate = FALSE);																				//	constructor
				~Display (void);																												//	destructor
				void			Swap (void);																									//	swap the currently selected buffer to the screen
				void			Erase (void);																									//	erase the display
				void			EraseRect (RECT &r);																					//	ds:erase a rectangular display area
				void			EraseAll (void);																							//	erase the entire display area
				Pixel			operator () (int x, int y) const;															//	array indexing operator
				Pixel			&operator () (int x, int y);																	//	array indexing operator
    		pPtr			PPtr (int x, int y);																					//	return a pointer to the requested pixel in the current surface
				RECT			&ScrnRect (void);																							//	return the full screen rect
				RECT			&PlayRect (void);																							//	return the play screen rect
    		int				Stride (void) const;																					//	return the size of a line in bytes
				void			Lock (void);																									//	prepare the back surface for drawing
				void			Unlock (void);																								//	clean up the back surface after rendering
				HDC				GetDC (void);																									//	get the display context for the primary surface
				void			ReleaseDC (HDC hdc);																					//	release the display context
				void			TimeRate (int count = 100);																		//	run a timer on the refresh rate
				Real			Rate (void);																									//	return the display rate
				void			SetRate (Real rate);																					//	set the display rate
				void			DumpToFile (cstr file);																				//	dump the display contents to a file
				void			Restore (void);																								//	restore the display in the event of an error
};																																							//	end

//------------------------------------------------------------------------------
inline	void	Display::Erase (void)																							//	erase the display
{																																								//	begin
	current->Erase ();																														//	erase the current surface
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Display::EraseRect (RECT &r)																			//	ds:erase a rectangular display area
{																																								//	begin
	current->EraseRect (r);																												//	erase the current surface
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Display::EraseAll (void)																					//	erase the entire display area
{																																								//	begin
	current->EraseAll ();																													//	erase the current surface
}																																								//	end

//------------------------------------------------------------------------------
inline	Pixel	Display::operator () (int x, int y) const													//	array indexing operator
{																																								//	begin
	return current->operator () (x, y);																						//	return the requested pixel
}																																								//	end

//------------------------------------------------------------------------------
inline	Pixel	&Display::operator () (int x, int y)															//	array indexing operator
{																																								//	begin
	return current->operator () (x, y);																						//	return the requested pixel
}																																								//	end

//------------------------------------------------------------------------------
inline	pPtr	Display::PPtr (int x, int y)																			//	return a pointer to the requested pixel
{																																								//	begin
	return current->PPtr (x, y);																									//	return the requested pointer
}																																								//	end

//------------------------------------------------------------------------------
inline	RECT	&Display::ScrnRect (void)																					//	return the full screen rect
{																																								//	begin
	return scrnRect;																															//	return the screen rect
}																																								//	end

//------------------------------------------------------------------------------
inline	RECT	&Display::PlayRect (void)																					//	return the play screen rect
{																																								//	begin
	return playRect;																															//	return the play rect
}																																								//	end

//------------------------------------------------------------------------------
inline	int		Display::Stride (void) const																			//	return the size of a line in bytes
{																																								//	begin
	return current->Stride ();																										//	return the stride of the surface
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Display::Lock (void)																							//	prepare the back surface for drawing
{																																								//	begin
	current->Lock ();																															//	lock the current surface
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Display::Unlock (void)																						//	clean up the back surface after rendering
{																																								//	begin
	current->Unlock ();																														//	unlock the current surface
}																																								//	end

//------------------------------------------------------------------------------
inline	Real	Display::Rate (void)																							//	return the display rate
{																																								//	begin
	return rate;																																	//	return the rate
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Display::SetRate (Real rat)																				//	set the display rate
{																																								//	begin
	rate = rat;																																		//	copy the new rate
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Display::DumpToFile (cstr file)																		//	dump the display contents to a file
{																																								//	begin
	current->DumpToFile (file);																										//	dump the surface contents to a file
}																																								//	end

//------------------------------------------------------------------------------
extern	Display		*gDisplay;																										//	global display pointer

//------------------------------------------------------------------------------

#endif	//	DISPLAY_8
