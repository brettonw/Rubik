//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/17/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "debugger.h"

//------------------------------------------------------------------------------
QDebugger	gDebugger ("Crixa Server");																						//	global debugger

//------------------------------------------------------------------------------
QDebugger::QDebugger (cstr nm)																									//	constructor
{																																								//	begin
	count = 0;																																		//	start with the debugger turned off
}																																								//	end

//------------------------------------------------------------------------------
void	QDebugger::Soft (cstr format, ...)																				//	message function
{																																								//	begin
	if (!count) return;																														//	bail out if debugging is off
	va_list	var;																																	//	variable argument list
	va_start (var, format);																												//	set up the variable argument list
	vprintf (format, var);																												//	print the message
	va_end (var);																																	//	clean up the argument list
}																																								//	end

//------------------------------------------------------------------------------
void	QDebugger::Hard (cstr format, ...)																				//	message function
{																																								//	begin
	if (!count) return;																														//	bail out if debugging is off
	va_list	var;																																	//	variable argument list
	va_start (var, format);																												//	set up the variable argument list
	vprintf (format, var);																												//	print the message
	va_end (var);																																	//	clean up the argument list
	fflush (stdout);																															//	commit the buffers to disk
}																																								//	end

//------------------------------------------------------------------------------
