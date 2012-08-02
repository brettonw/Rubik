//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/28/96
//
//------------------------------------------------------------------------------

#ifndef	SEQUENCER
#define	SEQUENCER

//------------------------------------------------------------------------------
#include "sequence ptr.h"
#include "list.h"

//------------------------------------------------------------------------------
class Sequencer																																	//	sequence of visual effects
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Real			parallax;																											//	the parallax factor
				List			sequences;																										//	the sequence list
	public:																																				//	members available externally
  			Sequencer (Real parallax = R(1.0));																			//	constructor
				void			Install (SeqPtr sequence, Real delay = ZERO);									//	install a sequence
				void			Step (CPoint where);																					//	step the sequencer
				void			SetParallax (Real parallax);																	//	set the sequencer parallax
};																																							//	end

//------------------------------------------------------------------------------
inline	Sequencer::Sequencer (Real par)																					//	constructor
{																																								//	begin
	parallax = par;																																//	copy the parallax factor
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Sequencer::SetParallax (Real par)																	//	set the sequencer parallax
{																																								//	begin
	parallax = par;																																//	copy the parallax factor
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	SEQUENCER