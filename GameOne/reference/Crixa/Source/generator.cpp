//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Last Revision:	08/16/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "generator.h"

//------------------------------------------------------------------------------
Generator::Generator (SeqPtr seq, int out) : PowerDevice (seq, 0)								//	default constructor
{																																								//	begin
	output = out;																																	//	save the output level
}																																								//	end

//------------------------------------------------------------------------------
Generator::~Generator (void)																										//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
int	Generator::Trigger (const PiecePtr &piece, int type, int what)							//	trigger the generator
{																																								//	begin
	if (type == DEV_TOGGLE)																												//	if the generator is to switch states
	{
		type = DEV_POWER;
		what = power ? -output : output;
	}
	return PowerDevice::Trigger (piece, type, what);
}																																								//	end

//------------------------------------------------------------------------------
