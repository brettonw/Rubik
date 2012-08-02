//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/09/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "disk file.h"
#include "debugger.h"

//------------------------------------------------------------------------------
DiskFile::DiskFile (cstr name)																									//	constructor
{																																								//	begin
	filePtr = 0;																																	//	empty file pointer
	error = FALSE;																																//	no error
	strcpy (fileName, name);																											//	copy the file name
}																																								//	end

//------------------------------------------------------------------------------
DiskFile::DiskFile (const DiskFile &f)																					//	constructor
{																																								//	begin
	error = f.error;																															//	copy the file error state
	filePtr = 0;																																	//	empty file pointer
	if (!error)																																		//	if there is no error condition
		strcpy (fileName, f.fileName);																							//	copy the file name
}																																								//	end

//------------------------------------------------------------------------------
DiskFile::~DiskFile (void)																											//	destructor
{																																								//	begin
	if (filePtr)																																	//	if the file is open
		Close ();																																		//	close it
}																																								//	end

//------------------------------------------------------------------------------
Bool	DiskFile::Open (void)																											//	open the file
{																																								//	begin
	if (!filePtr && !error)																												//	if there has been no error and the file is not already open
	{																																							//	begin
		filePtr = fopen (fileName, "rb");																						//	open the file
		error = filePtr ? FALSE : TRUE;																							//	test for the error condition
		if (error)																																	//	if there was an error
		{																																						//	begin
			DEBUG_HARD ("\nDiskFile:\n");																							//	report the failure
			DEBUG_HARD ("    Failed to open file (%s)\n", fileName);									//	report the failure
		}																																						//	end
	}																																							//	end
	return error ? FALSE : TRUE;																									//	return the success or failure of the open
}																																								//	end

//------------------------------------------------------------------------------
void	DiskFile::Close (void)																										//	close the file
{																																								//	begin
	if (filePtr)																																	//	if the file is open
		fclose (filePtr);																														//	close the file
}																																								//	end

//------------------------------------------------------------------------------
void	DiskFile::Read (void *data, long size, int num)														//	read data from the file
{																																								//	begin
	if (filePtr && !error)																												//	if the file is open and there is not an error condition
	{																																							//	begin
		fread (data, size, num, filePtr);																						//	do the file read
		error = ferror (filePtr) ? TRUE : FALSE;																		//	do the file read
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
long	DiskFile::Length (void)																										//	get the length of the file
{																																								//	begin
	long	length = 0L,																														//	start with a 0
				curPos;																																	//	place to get the position
	if (filePtr && !error)																												//	if the file is open and there is not an error condition
	{																																							//	begin
		 curPos = ftell (filePtr);																									//	get the current position
		 fseek (filePtr, 0, SEEK_END);																							//	go to the end of the file
		 length = ftell (filePtr);																									//	get the position into the length
		 fseek (filePtr, curPos, SEEK_SET);																					//	go back to where we were
	}																																							//	end
	return length;																																//	return the result
}																																								//	end

//------------------------------------------------------------------------------
void	DiskFile::Reset (void)																										//	move to the beginning of the file
{																																								//	begin
	if (filePtr && !error)																												//	if the file is open and there is not an error condition
		 error = fseek (filePtr, 0, SEEK_SET) ? TRUE : FALSE;												//	seek to the beginning of the file
}																																								//	end

//------------------------------------------------------------------------------
void	DiskFile::Position (long pos)																							//	move to a specific location in the file
{																																								//	begin
	if (filePtr && !error)																												//	if the file is open and there is not an error condition
		 error = fseek (filePtr, pos, SEEK_SET) ? TRUE : FALSE;											//	seek to the requested position
}																																								//	end

//------------------------------------------------------------------------------
void	DiskFile::Move (long offset)																							//	move from the current offset in the file
{																																								//	begin
	if (filePtr && !error)																												//	if the file is open and there is not an error condition
		 error = fseek (filePtr, offset, SEEK_CUR) ? TRUE : FALSE;									//	move the current position
}																																								//	end

//------------------------------------------------------------------------------
long	DiskFile::Current (void)																									//	get the current location in the file
{																																								//	begin
	long		pos = 0L;																															//	start with a 0
	if (filePtr && !error)																												//	if the file is open and there is not an error condition
		 pos = ftell (filePtr);																											//	get the current location
	return pos;																																		//	return the result
}																																								//	end

//------------------------------------------------------------------------------
Bool	DiskFile::Open (int mode)																									//	open the file in a specific mode
{																																								//	begin
	if (!filePtr && !error)																												//	if there has been no error and the file is not already open
	{																																							//	begin
		cstr	modes[] = {"rb", "wb", "ab", "rb+", "wb+", "ab+"};										//	array of mode identifiers
		filePtr = fopen (fileName, modes[mode]);																		//	open the file
		error = filePtr ? FALSE : TRUE;																							//	test for the error condition
		if (error)																																	//	if there was an error
		{																																						//	begin
			DEBUG_HARD ("\nDiskFile: mode %s\n",modes[mode]);														//	report the failure
			DEBUG_HARD ("    Failed to open file (%s)\n", fileName);									//	report the failure
		}																																						//	end
	}																																							//	end
	return error ? FALSE : TRUE;																									//	return the success or failure of the open
}																																								//	end

//------------------------------------------------------------------------------
void	DiskFile::Write (void *data, long size, int num)													//	write data to the file
{																																								//	begin
	if (filePtr && !error)																												//	if the file is open and there is not an error condition
	{																																							//	begin
		fwrite (data, size, num, filePtr);																					//	do the file write
		error = ferror (filePtr) ? TRUE : FALSE;																		//	do the file read
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
void	DiskFile::Flush (void)																										//	flush written data to the file
{																																								//	begin
	if (filePtr && !error)																												//	if the file is open and there is not an error condition
		fflush (filePtr);																														//	flush the file data
}																																								//	end

//------------------------------------------------------------------------------
