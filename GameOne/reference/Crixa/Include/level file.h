//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/09/97
//
//------------------------------------------------------------------------------

#ifndef	LEVEL_FILES
#define	LEVEL_FILES

//------------------------------------------------------------------------------
#include "disk file.h"

//------------------------------------------------------------------------------
class	SubFile
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
				char			name[16];																											//	a fifteen character name, null terminated
				int				start;																												//	the offset from the beginning of the file to the start of this file
				int				end;																													//	the offset from the beginning of the file to the end of this file
};																																							//	end

//------------------------------------------------------------------------------
class	LevFile : public File																											//	level file class
{																																								//	begin
	private:																																			//	members internal to this class only
static	DiskFile	*level;																												//	pointer to the level file
static	int				count;																												//	the number of subfiles in the archive
static	SubFile		*subfile;																											//	record of subfile names and locations in the level file
	protected:																																		//	members internal to this class hierarchy
				int				index;																												//	the index of the subfile corresponding to the file I am using
	public:																																				//	members available externally
				LevFile (cstr name);																										//	constructor
virtual	~LevFile (void);																												//	destructor
virtual	Bool			Open (void);																									//	open the file
virtual	void			Close (void);																									//	close the file
virtual	void			Read (void *data, long size, int num = 1);										//	read data from the file
virtual	long			Length (void);																								//	get the length of the file
virtual	void			Reset (void);																									//	move to the beginning of the file
virtual	void			Position (long pos);																					//	move to a specific location in the file
virtual	void			Move (long offset);																						//	move from the current offset in the file
virtual	long			Current (void);																								//	get the current location in the file
				void			BeginWriting (void);																					//	gather information needed for writing over the existing data for this file
				void			EndWriting (void);																						//	complete writing over the existing data for this file
static	void			OpenLevel (cstr name);																				//	open a named level file
static	void			CloseLevel (void);																						//	close the current level
};																																							//	end

//------------------------------------------------------------------------------

#endif	//LEVEL_FILES