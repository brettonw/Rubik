//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	03/06/97	JB		creation.
//
//------------------------------------------------------------------------------

#ifndef BOSS_EXPLOSION_SEQUENCE
#define	BOSS_EXPLOSION_SEQUENCE

//------------------------------------------------------------------------------
#include "explosion sequence.h"
#include "piece ptr.h"

//------------------------------------------------------------------------------
class	BossExplSeq : public ExplSeq
{
	private:
	protected:
	public:
				BossExplSeq (const PiecePtr &piece, Real time, int numPops);
virtual	~BossExplSeq (void);
virtual	void				Pop (void);
};

//------------------------------------------------------------------------------

#endif // BOSS_EXPLOSION_SEQUENCE
