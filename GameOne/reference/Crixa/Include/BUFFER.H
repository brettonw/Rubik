//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/08/97
//
//------------------------------------------------------------------------------

#ifndef	BUFFER
#define	BUFFER

//------------------------------------------------------------------------------
#include "file.h"

//------------------------------------------------------------------------------
class	Buffer : public File																											//	buffer class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				File			*file;																												//	pointer to the source file
				Bool			delfile;																											//	true if I created the file internally
				uPtr			data;																													//	the data
				uPtr			current;																											//	the current location
				uPtr			end;																													//	the end of the data
				Buffer (cstr name, Bool unused);																				//	constructor
				Buffer (File *file, Bool unused);																				//	constructor
	public:																																				//	members available externally
				Buffer (cstr name);																											//	constructor
				Buffer (File *file);																										//	constructor
virtual	~Buffer (void);																													//	destructor
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
inline	uPtr	Buffer::CurrentPtr (void)																					//	return the current pointer
{																																								//	begin
	return current;																																//	return the current buffer location
}																																								//	end

//------------------------------------------------------------------------------

#endif	//BUFFER
