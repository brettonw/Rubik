//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	03/06/97	JB		creation.
//
//------------------------------------------------------------------------------

#ifndef ALT_PUMPING_SEQUENCE
#define ALT_PUMPING_SEQUENCE

//------------------------------------------------------------------------------
#include "pumping sequence.h"

//------------------------------------------------------------------------------
class AltPumpSeq : public PumpSeq
{
	private:
	protected:
	public:
				AltPumpSeq (const SArrayPtr &spr, const PiecePtr &obj, CVector offset);	//	constructor
virtual	~AltPumpSeq (void);
virtual	Bool			Step (CPoint where);
};

//------------------------------------------------------------------------------

#endif // ALT_PUMPING_SEQUENCE
