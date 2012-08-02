//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/18/96
//	03/04/97	DS		flashing font support
//	03/07/97	DS		enhanced text manager modes
//------------------------------------------------------------------------------

#ifndef	TEXT_MGR
#define	TEXT_MGR

//------------------------------------------------------------------------------
#include "sprite array ptr.h"
#include "sequence ptr.h"
#include "tqueue.h"

//------------------------------------------------------------------------------
#define TXM_SIMPLE		0x00000000
#define	TXM_QUEUE			0x00000001																								//	queue messages instead of overwriting
#define	TXM_RETRIG		0x00000002																								//	allow message to be retriggered (disable simple filtering)
#define	TXM_LOOPQ			0x00000004																								//	loop messages in queue

//------------------------------------------------------------------------------
class TextMgr : public Sequence																									//	text sequence manager
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
  			SArrayPtr				font;																										//	array of sprites for the font
				SArrayPtr				boldfont;
				SeqPtr					last;																										//	the last sequence we drew
				SeqPtr					current;																								//	the current sequence
				TQueue<SeqPtr>	queue;																									//	the sequence queue
				Bool						queueing;																								//	whether or not queueing is used for this manager
				Bool						debug;
				int							count;																									//	the number of strings in the manager
				struct	StrRecord																												//	private structure for maintaining string records
				{																																				//	begin
					int							count;																								//	how many more times to show this string
					cstr						string;																								//	the string
				}								*strings;																								//	the array of text strings
				int							lastID;																									//	id of the last requested message
				int							gotID;																									//	id of the currently requested message
				Bool						retrigger;																							//	whether or not to allow retriggering of messages
				Bool						loopq;																									//	whether to loop messages in the queue
				int							sound;																									//	id of the sound to play when a new message is pending
	public:																																				//	members available externally
  			TextMgr (SArrayPtr font, SArrayPtr bold, cstr file, int snd, int mode);	//	constructor
  			TextMgr (SArrayPtr font, SArrayPtr bold, int snd, int mode);						//	constructor
virtual	~TextMgr (void);																												//	destructor
virtual	Bool			Step (CPoint where);																					//	draw the next frame
				Bool			Message (int id);																							//	queue the requested message string from the array
				void			Message (cstr str);																						//	queue the message string
				void			Recall (void);																								//	recall the last message string
				void			Clear (void);																									//	clear the current sequence
				void			ClearQueue (void);																						//	clear the queue
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	TEXT_MGR
