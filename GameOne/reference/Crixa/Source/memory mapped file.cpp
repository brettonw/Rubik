//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/29/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "memory mapped file.h"
#include "debugger.h"

//------------------------------------------------------------------------------
MemFile::MemFile (cstr name)																										//	constructor
{																																								//	begin
	hFile = CreateFile (name, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, 0);
  hFileMapping = CreateFileMapping (hFile, 0, PAGE_READWRITE, 0, 0, 0);
  current = data = uPtr (MapViewOfFile (hFileMapping, FILE_MAP_WRITE, 0, 0, 0));
	if (current)
		error = FALSE;
	else
		DEBUG_STR ("Failed to map file (%s)\n", name);
}																																								//	end

//------------------------------------------------------------------------------
MemFile::~MemFile (void)																												//	destructor
{																																								//	begin
  UnmapViewOfFile (data);
  CloseHandle (hFileMapping);
  CloseHandle (hFile);
}																																								//	end

//------------------------------------------------------------------------------
Bool	MemFile::Open (void)																											//	open the file
{																																								//	begin
	return current ? TRUE : FALSE;																								//	return true if there is data
}																																								//	end

//------------------------------------------------------------------------------
void	MemFile::Close (void)																											//	close the file
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	MemFile::Read (void *mv, long size, int num)															//	read data from the buffer
{																																								//	begin
	uPtr	move = uPtr (mv),																												//	copy the pointer to target memory
				stop = current + (size * num);																					//	compute the end of the copy
	while (current != stop)																												//	while we have more bytes to copy
		*move++ = *current++;																												//	copy a byte
}																																								//	end

//------------------------------------------------------------------------------
long	MemFile::Length (void)																										//	get the length of the buffer
{																																								//	begin
	return GetFileSize (hFile, 0);																								//	compute the end of buffer address
}																																								//	end

//------------------------------------------------------------------------------
void	MemFile::Reset (void)																											//	move to the beginning of the buffer
{																																								//	begin
	current = data;																																//	reset the buffer pointer
}																																								//	end

//------------------------------------------------------------------------------
void	MemFile::Position (long pos)																							//	set the current position
{																																								//	begin
	current = data + pos;																													//	set the current buffer location
}																																								//	end

//------------------------------------------------------------------------------
void	MemFile::Move (long offset)																								//	move from the current offset
{																																								//	begin
	current += offset;																														//	compute the new location
	if (current < data)																														//	otherwize if it will take us past the beginning of the buffer
		current = data;																															//	stop at the beginning of the buffer
}																																								//	end

//------------------------------------------------------------------------------
long	MemFile::Current (void)																										//	return the current position
{																																								//	begin
	return current - data;																												//	return the current buffer location
}																																								//	end

//------------------------------------------------------------------------------
