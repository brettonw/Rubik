//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/08/97
//	03/04/97	DS		flashing font support (kinda cheesy)
//	03/07/97	DS		changed so TextMgr can accept a mode, added looping queue,
//									disabled counts (they're not useful)
//	03/09/97	DS		added file-less text manager for shipstatus 
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "text mgr.h"
#include "text sequence.h"
#include "view.h"
#include "level file.h"
#include "text buffer.h"
#include "format text.h"
#include "sound mgr.h"
#include "debugger.h"

//------------------------------------------------------------------------------
TextMgr::TextMgr (SArrayPtr fon, SArrayPtr bold ,cstr file, int snd, int mode)	//	constructor
{																																								//	begin
	LevFile lev (file);
	TxtBuffer	buffer (&lev);																											//	load the requested file
	font = fon;																																		//	copy the font
	boldfont = bold;
	count = buffer.Length () >> 1;																								//	get the number of strings in the strings file
	strings = new StrRecord[count];																								//	allocate the strings array
	for (int i = 0; i < count; i++)																								//	loop over all of the strings
	{																																							//	begin
		cstr	string;																																//	place to get the unformatted string
		buffer >> strings[i].count >> string;																				//	loading the the strings array
		FormatText (string);																												//	format the string
		strings[i].string = new char[strlen (string) + 1];													//	allocate the string memory
		strcpy (strings[i].string, string);																					//	copy the string into the array
	}																																							//	end
	sound = snd;																																	//	copy the sound id to play when a new message is pending
	
	queueing = (mode & TXM_QUEUE) ? TRUE : FALSE;																	//	copy whether or not to queue messages
	retrigger = (mode & TXM_RETRIG) ? TRUE : FALSE;																//	copy whether or not to allow retriggering of message IDs
	loopq = (mode & TXM_LOOPQ) ? TRUE : FALSE;																		//	copy whether or not to allow looping in queue
	lastID = gotID = -1;																													//	clear the retriggering IDs
}																																								//	end

//------------------------------------------------------------------------------
TextMgr::TextMgr (SArrayPtr fon, SArrayPtr bold, int snd, int mode)							//	constructor
{																																								//	begin
	count = 0;
	sound = snd;																																	//	copy the sound id to play when a new message is pending
	
	font = fon;																																		//	copy the font
	boldfont = bold;

	queueing = (mode & TXM_QUEUE) ? TRUE : FALSE;																	//	copy whether or not to queue messages
	retrigger = (mode & TXM_RETRIG) ? TRUE : FALSE;																//	copy whether or not to allow retriggering of message IDs
	loopq = (mode & TXM_LOOPQ) ? TRUE : FALSE;																		//	copy whether or not to allow looping in queue
	lastID = gotID = -1;																													//	clear the retriggering IDs
}																																								//	end

//------------------------------------------------------------------------------
TextMgr::~TextMgr (void)																												//	destructor
{																																								//	begin
	for (int i = 0; i < count; i++)																								//	loop over all of the strings
		delete[] strings[i].string;																									//	free up each string in turn
	delete[] strings;																															//	free up the strings array
}
																																								//	end

//------------------------------------------------------------------------------
Bool	TextMgr::Step (CPoint where)																							//	draw the next frame
{																																								//	begin
	lastID = gotID;																																//	propogate the ID of the newest message
	if (retrigger)																																//	if retriggering is supported
		gotID = -1;																																	//	clear the newest message ID
	if (!current ())																															//	if there is no current message
	{																																							//	begin
		SeqPtr	*seq = queue.Dequeue ();																						//	get a message off the queue
		if (seq)																																		//	if it is a valid message
			current = *seq;																														//	set the current message up with it
	}																																							//	end
	else																																					//	otherwise, play the current message
		if (current->Step (where) == FALSE)																					//	step the message sequence, if it is finished
		{
			Clear ();																																	//	clear the sequence
			if (loopq)																																//	ds: enqueue the current message so it loops
				Recall();
		}
	return TRUE;																																	//	always return true
}																																								//	end

//------------------------------------------------------------------------------
Bool	TextMgr::Message (int id)																									//	queue the requested message string from the array
{																																								//	begin
	gotID = id;																																		//	save the id of the new message
	if ((id >= 0) && (id < count) && (id != lastID))															//	if the id is a valid entry
		if (strings[id].count)																											//	if the strings can be drawn again
		{																																						//	begin
			SeqPtr	seq (new TextSeq (font, boldfont, strings[id].string, 250));			//	create the new sequence
			if (!queueing)																														//	if queueing is not on
				Clear ();																																//	clear the current sequence
			queue.Enqueue (seq);																											//	enqueue it
//		strings[id].count--;																											//	decrement the string count (disabled as patch)
			gSoundMgr->PlaySound (sound);																							//	play the alert sound to indicate a new message is pending
			return TRUE;																															//	return true to indicate the message was queued
		}																																						//	end
	return FALSE;																																	//	return false to indicate the message was not queued
}																																								//	end

//------------------------------------------------------------------------------
void	TextMgr::Message (cstr str)																								//	queue the message string
{																																								//	begin
	SeqPtr	seq (new TextSeq (font, boldfont, str, 250));													//	create the new sequence
	if (!queueing)																																//	if queueing is not on
		Clear ();																																		//	clear the current sequence
	queue.Enqueue (seq);																													//	enqueue it
}																																								//	end

//------------------------------------------------------------------------------
void	TextMgr::Recall (void)																										//	recall the last message string
{																																								//	begin
	if (last ())																																	//	if there is a last message
	{																																							//	begin
		last->Reset ();																															//	reset it
		queue.Enqueue (last);																												//	enqueue it
		last = SeqPtr (0);																													//	clear the last string
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
void	TextMgr::Clear (void)																											//	clear the current sequence
{																																								//	begin
	if (current ())																																//	if there is a current message
	{																																							//	begin
		last = current;																															//	save the current message for later recall
		current = SeqPtr (0);																												//	clear the current message
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
void	TextMgr::ClearQueue (void)																								//	clear the current sequence
{
	Clear();
	SeqPtr	*seq;
	int i =0;
	while (seq = queue.Dequeue ()) i++;																						//	pop everything off the queue
}

//------------------------------------------------------------------------------
