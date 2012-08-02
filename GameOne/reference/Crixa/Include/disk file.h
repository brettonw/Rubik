//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/09/97
//
//------------------------------------------------------------------------------

#ifndef	DISK_FILES
#define	DISK_FILES

//------------------------------------------------------------------------------
#include "file.h"

//------------------------------------------------------------------------------
enum																																						//	enumerate file mode constants
{																																								//	begin
	DF_READ_ONLY = 0, DF_WRITE_ONLY, DF_APPEND_ONLY,															//	exclusive access modes
	DF_READ_WRITE, DF_READ_WRITE_NEW, DF_READ_APPEND															//	multiple access modes
};																																							//	end

//------------------------------------------------------------------------------
class	DiskFile : public File																										//	disk file class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				char			fileName[128];																								//	name of the file
				FILE			*filePtr;																											//	file pointer
	public:																																				//	members available externally
				DiskFile (cstr name);																										//	constructor
				DiskFile (const DiskFile &f);																						//	constructor
virtual	~DiskFile (void);																												//	destructor
virtual	Bool			Open (void);																									//	open the file
virtual	void			Close (void);																									//	close the file
virtual	void			Read (void *data, long size, int num = 1);										//	read data from the file
virtual	long			Length (void);																								//	get the length of the file
virtual	void			Reset (void);																									//	move to the beginning of the file
virtual	void			Position (long pos);																					//	move to a specific location in the file
virtual	void			Move (long offset);																						//	move from the current offset in the file
virtual	long			Current (void);																								//	get the current location in the file
				Bool			Open (int mode);																							//	open the file in a specific mode
				void			Write (void *data, long size, int num = 1);										//	write data to the file
				void			Flush (void);																									//	flush written data to the file
};																																							//	end

//------------------------------------------------------------------------------

#endif	//DISK_FILES