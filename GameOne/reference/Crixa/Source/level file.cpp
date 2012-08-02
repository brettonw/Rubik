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
#include "level file.h"
#include "disk file.h"
#include "debugger.h"

//------------------------------------------------------------------------------
DiskFile	*LevFile::level;																											//	pointer to the level file
int				LevFile::count;																												//	the number of subfiles in the archive
SubFile		*LevFile::subfile;																										//	record of subfile names and locations in the level file

//------------------------------------------------------------------------------
LevFile::LevFile (cstr name)																										//	constructor
{																																								//	begin
	if (level)
		for (int i = 0; i < count; i++)
			if (strcmp (subfile[i].name, name) == 0)
			{
				index = i;
				error = FALSE;
				break;
			}
	if (error)
		DEBUG_HARD ("Invalid file in archive: %s\n", name);
}																																								//	end

//------------------------------------------------------------------------------
LevFile::~LevFile (void)																												//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Bool	LevFile::Open (void)																											//	open the file
{																																								//	begin
	if (!error)
		level->Position (subfile[index].start);
	return error ? FALSE : TRUE;																									//	return true if there is level data
}																																								//	end

//------------------------------------------------------------------------------
void	LevFile::Close (void)																											//	close the file
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	LevFile::Read (void *mv, long size, int num)															//	read data from the buffer
{																																								//	begin
	level->Read (mv, size, num);
}																																								//	end

//------------------------------------------------------------------------------
long	LevFile::Length (void)																										//	get the length of the buffer
{																																								//	begin
	return subfile[index].end - subfile[index].start;
}																																								//	end

//------------------------------------------------------------------------------
void	LevFile::Reset (void)																											//	move to the beginning of the buffer
{																																								//	begin
	level->Position (subfile[index].start);
}																																								//	end

//------------------------------------------------------------------------------
void	LevFile::Position (long pos)																							//	set the current position
{																																								//	begin
	level->Position (subfile[index].start + pos);
}																																								//	end

//------------------------------------------------------------------------------
void	LevFile::Move (long offset)																								//	move from the current offset
{																																								//	begin
	level->Move (offset);
}																																								//	end

//------------------------------------------------------------------------------
long	LevFile::Current (void)																										//	return the current position
{																																								//	begin
	return level->Current () - subfile[index].start;
}																																								//	end

//------------------------------------------------------------------------------
void	LevFile::BeginWriting (void)																							//	gather information needed for writing over the existing data for this file
{																																								//	begin
	int		end = level->Length ();
	level->Position (end);
	subfile[index].start = end;
}																																								//	end

//------------------------------------------------------------------------------
void	LevFile::EndWriting (void)																								//	complete writing over the existing data for this file
{																																								//	begin
	subfile[index].end = level->Current ();
	level->Reset ();
	level->Write (&count, sizeof (int));
	level->Write (subfile, sizeof (SubFile), count);
	level->Flush ();
}																																								//	end

//------------------------------------------------------------------------------
void	LevFile::OpenLevel (cstr name)																						//	open a named level file
{																																								//	begin
	level = new DiskFile (name);
	if (level->Open (DF_READ_WRITE))
	{
		int	c;
		level->Read (&c, sizeof (int));
		count = c;
		subfile = new SubFile[count];
		level->Read (subfile, sizeof (SubFile), count);
	}
}																																								//	end

//------------------------------------------------------------------------------
void	LevFile::CloseLevel (void)																								//	close the current level
{																																								//	begin
	level->Close ();
	delete level;
	delete[] subfile;
}																																								//	end

//------------------------------------------------------------------------------
