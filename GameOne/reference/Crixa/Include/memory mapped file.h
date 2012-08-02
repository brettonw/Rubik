//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/29/97
//
//------------------------------------------------------------------------------

#ifndef	MEMORY_MAPPED_FILE
#define	MEMORY_MAPPED_FILE

//------------------------------------------------------------------------------
#include "file.h"

//------------------------------------------------------------------------------
class	MemFile : public File																											//	memory mapped file class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				HANDLE		hFile;																												//	handle to the file
				HANDLE		hFileMapping;																									//	handle to the file mapping
				uPtr			data;																													//	the data
				uPtr			current;																											//	the current location
	public:																																				//	members available externally
				MemFile (cstr name);																										//	constructor
virtual	~MemFile (void);																												//	destructor
virtual	Bool			Open (void);																									//	open the file
virtual	void			Close (void);																									//	close the file
virtual	void			Read (void *data, long size, int num = 1);										//	read data from the buffer
virtual	long			Length (void);																								//	get the length of the buffer
virtual	void			Reset (void);																									//	move to the beginning of the buffer
virtual	void			Position (long pos);																					//	move to a specific location in the file
virtual	void			Move (long offset);																						//	move from the current offset in the file
virtual	long			Current (void);																								//	get the current location in the file
				uPtr			CurrentPtr (void);																						//	return the current pointer
};																																							//	end

//------------------------------------------------------------------------------
inline	uPtr	MemFile::CurrentPtr (void)																				//	return the current pointer
{																																								//	begin
	return current;																																//	return the current buffer location
}																																								//	end

//------------------------------------------------------------------------------

#endif	//MEMORY_MAPPED_FILE
