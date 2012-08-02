//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	and:						Bretton Wade
//	Last Revision:	08/16/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "power device.h"
#include "switch sequence.h"

//------------------------------------------------------------------------------
PowerDevice::PowerDevice (SeqPtr seq, int id) : Device (id)											//	constructor
{																																								//	begin
	sequence = seq;
}																																								//	end

//------------------------------------------------------------------------------
PowerDevice::~PowerDevice (void)																								//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Bool	PowerDevice::SwitchOn (void)																							//	turn the device on
{																																								//	begin
	Bool	work = Device::SwitchOn ();
	if (work)
	{
		SwitchSeq	*seq = (SwitchSeq *) sequence ();
		if (seq)
			seq->Switch (TRUE);
	}
	return work;
}																																								//	end

//------------------------------------------------------------------------------
Bool	PowerDevice::SwitchOff (void)																							//	turn the device off
{																																								//	begin
	Bool	work = Device::SwitchOff ();
	if (work)
	{
		SwitchSeq	*seq = (SwitchSeq *) sequence ();
		if (seq)
			seq->Switch (FALSE);
	}
	return work;
}																																								//	end

//------------------------------------------------------------------------------
