//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/08/97
//
//------------------------------------------------------------------------------

#ifndef	FILES
#define	FILES

//------------------------------------------------------------------------------
class	File																																			//	file class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Bool			error;																												//	the error condition of the file
	public:																																				//	members available externally
				File (void);																														//	constructor
virtual	~File (void);																														//	destructor
virtual	Bool			Open (void) = 0;																							//	open the file
virtual	void			Close (void) = 0;																							//	close the file
virtual	void			Read (void *data, long size, int num = 1) = 0;								//	read data from the file
virtual	long			Length (void) = 0;																						//	get the length of the file
virtual	void			Reset (void) = 0;																							//	move to the beginning of the file
virtual	void			Position (long pos) = 0;																			//	move to a specific location in the file
virtual	void			Move (long offset) = 0;																				//	move from the current offset in the file
virtual	long			Current (void) = 0;																						//	get the current location in the file
				Bool			Error (void) const;																						//	check for an error
};																																							//	end

//------------------------------------------------------------------------------
inline	File::File (void)																												//	constructor
{																																								//	begin
	error = TRUE;																																	//	no file data, so error state must be manually reset by subclasses
}																																								//	end

//------------------------------------------------------------------------------
inline	Bool	File::Error (void) const																					//	check for an error
{																																								//	begin
	return error;																																	//	return the error state
}																																								//	end

//------------------------------------------------------------------------------

#endif	//FILES
