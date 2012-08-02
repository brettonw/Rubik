//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/08/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "buffer.h"
#include "disk file.h"

//------------------------------------------------------------------------------
Buffer::Buffer (cstr name, Bool unused)																					//	constructor
{																																								//	begin
	file = new DiskFile (name);																										//	create the file
	delfile = TRUE;																																//	set the delfile flag to delete the file pointer when I'm done
	current = 0;																																	//	empty current pointer
	data = 0;																																			//	null pointer
	end = 0;																																			//	end is same as beginning
}																																								//	end

//------------------------------------------------------------------------------
Buffer::Buffer (File *fil, Bool unused)																					//	constructor
{																																								//	begin
	file = fil;																																		//	copy the file pointer
	delfile = FALSE;																															//	set the delfile flag to leave the file pointer alone when I'm done
	current = 0;																																	//	empty current pointer
	data = 0;																																			//	null pointer
	end = 0;																																			//	end is same as beginning
}																																								//	end

//------------------------------------------------------------------------------
Buffer::Buffer (cstr name)																											//	constructor
{																																								//	begin
	file = new DiskFile (name);																										//	create the file
	delfile = TRUE;																																//	set the delfile flag to delete the file pointer when I'm done
	if (file->Open ())																														//	if the file is successfully opened
	{																																							//	begin
		long	length = file->Length ();																							//	get the length of the file
		data = new uchar[length];																										//	allocate the memory to load the file
		end = data + length;																												//	compute the end of buffer address
		current = data;																															//	start with the current location at zero offset
		file->Reset ();																															//	start the file out at the beginning
		file->Read (data, sizeof (uchar), length);																	//	read the contents of the file into the buffer memory
		file->Close ();																															//	close the file
	}																																							//	end
	else																																					//	otherwise, the file is not opened
	{																																							//	begin
		current = 0;																																//	empty current pointer
		data = 0;																																		//	null pointer
		end = 0;																																		//	end is same as beginning
	}																																							//	end
	error = file->Error ();																												//	copy the error state from the file
}																																								//	end

//------------------------------------------------------------------------------
Buffer::Buffer (File *fil)																											//	constructor
{																																								//	begin
	file = fil;																																		//	copy the file pointer
	delfile = FALSE;																															//	set the delfile flag to leave the file pointer alone when I'm done
	if (file->Open ())																														//	if the file is successfully opened
	{																																							//	begin
		long	length = file->Length ();																							//	get the length of the file
		data = new uchar[length];																										//	allocate the memory to load the file
		end = data + length;																												//	compute the end of buffer address
		current = data;																															//	start with the current location at zero offset
		file->Reset ();																															//	start the file out at the beginning
		file->Read (data, sizeof (uchar), length);																	//	read the contents of the file into the buffer memory
		file->Close ();																															//	close the file
	}																																							//	end
	else																																					//	otherwise, the file is not opened
	{																																							//	begin
		current = 0;																																//	empty current pointer
		data = 0;																																		//	null pointer
		end = 0;																																		//	end is same as beginning
	}																																							//	end
	error = file->Error ();																												//	copy the error state from the file
}																																								//	end

//------------------------------------------------------------------------------
Buffer::~Buffer (void)																													//	destructor
{																																								//	begin
	if (data)																																			//	if the pointer was actually allocated
		delete[] data;																															//	dispose of the buffer data
	if (delfile)																																	//	if I allocated the file
		delete file;																																//	delete it
}																																								//	end

//------------------------------------------------------------------------------
Bool	Buffer::Open (void)																												//	open the file
{																																								//	begin
	return current ? TRUE : FALSE;																								//	return true if there is data
}																																								//	end

//------------------------------------------------------------------------------
void	Buffer::Close (void)																											//	close the file
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	Buffer::Read (void *mv, long size, int num)																//	read data from the buffer
{																																								//	begin
	if (current < end)																														//	if the pointer is not at the end
	{																																							//	begin
		uPtr	move = uPtr (mv),																											//	copy the pointer to target memory
					stop = current + (size * num);																				//	compute the end of the copy
		if (stop > end)																															//	if that will take us past the end of the buffer
			stop = end;																																//	stop at the end of the buffer
		while (current != stop)																											//	while we have more bytes to copy
			*move++ = *current++;																											//	copy a byte
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
long	Buffer::Length (void)																											//	get the length of the buffer
{																																								//	begin
	return end - data;																														//	return the buffer end pointer - the buffer begin pointer
}																																								//	end

//------------------------------------------------------------------------------
void	Buffer::Reset (void)																											//	move to the beginning of the buffer
{																																								//	begin
	current = data;																																//	reset the buffer pointer
}																																								//	end

//------------------------------------------------------------------------------
void	Buffer::Position (long pos)																								//	set the current position
{																																								//	begin
	current = data + pos;																													//	set the current buffer location
}																																								//	end

//------------------------------------------------------------------------------
void	Buffer::Move (long offset)																								//	move from the current offset
{																																								//	begin
	if (current < end)																														//	if the pointer is not at the end
	{																																							//	begin
		current = current + offset;																									//	compute the end of the copy
		if (current > end)																													//	if that will take us past the end of the buffer
			current = end;																														//	stop at the end of the buffer
		else if (current < data)																										//	otherwize if it will take us past the beginning of the buffer
			current = data;																														//	stop at the beginning of the buffer
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
long	Buffer::Current (void)																										//	return the current position
{																																								//	begin
	return current - data;																												//	return the current buffer location
}																																								//	end

//------------------------------------------------------------------------------
