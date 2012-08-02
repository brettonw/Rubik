//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	12/23/96
//
//	02/22/97	DS		Added timestamp to output file creation
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "debugger.h"
#include <time.h>

//------------------------------------------------------------------------------
QDebugger	gDebugger ("Crixa");																									//	global debugger

//------------------------------------------------------------------------------
QDebugger::QDebugger (cstr nm)																									//	constructor
{																																								//	begin
  struct tm *timestamp;																													//	get the current time
	time_t aclock;
	time(&aclock);
	timestamp=localtime(&aclock);

	sprintf (name, "%s.debug", nm);																								//	save the name
	FILE	*fp = fopen (name, "wc");																								//	open the file

	count = 0;																																		//	start with the debugger turned on

	if (fp)																																				//	if the open was successful
	{																																							//	begin
		fprintf (fp, "%s %s\n", nm,asctime(timestamp));															//	print the name of the file & date
		fflush (fp);																																//	commit the buffers to disk
		fclose (fp);																																//	close the file
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
void	QDebugger::Soft (cstr format, ...)																				//	message function
{																																								//	begin
	if (!count) return;																														//	bail out if debugging is off
	FILE	*fp = fopen (name, "a");																								//	open the file
	if (fp)																																				//	if the open was successful
	{																																							//	begin
		va_list	var;																																//	variable argument list
		va_start (var, format);																											//	set up the variable argument list
		vfprintf (fp, format, var);																									//	print the message
		va_end (var);																																//	clean up the argument list
		fclose (fp);																																//	close the file
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
void	QDebugger::Hard (cstr format, ...)																				//	message function
{																																								//	begin
	if (!count) return;																														//	bail out if debugging is off
	FILE	*fp = fopen (name, "ac");																								//	open the file
	if (fp)																																				//	if the open was successful
	{																																							//	begin
		va_list	var;																																//	variable argument list
		va_start (var, format);																											//	set up the variable argument list
		vfprintf (fp, format, var);																									//	print the message
		va_end (var);																																//	clean up the argument list
		//fflush (fp);																																//	commit the buffers to disk
		fclose (fp);																																//	close the file
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
