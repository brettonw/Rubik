//------------------------------------------------------------------------------
//	Copyright ©1997 qualia, inc. All rights reserved.
//
//	03/07/97	JB		creation.  completely revised. & uncommented.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "ammo upgrader.h"
#include "emitter piece.h"
#include "resource.h"
#include "mini piece node.h"
#include "referee.h"
#include "sound enum.h"
#include "team identification.h"
#include "sound mgr.h"
#include "debugger.h"

//------------------------------------------------------------------------------
const	int		kSndUpgrStart	= snd_lightnng;
const	int		kSndUpgrade		= snd_gen;
const	int		kSndUpgrStop	= snd_lightnng;
const	int		kSndBing			= snd_counter;
const	Real	kStepOffTime	= R(0.5);
const Real	kBingDelay		= R(0.25);

//------------------------------------------------------------------------------
class AUMiniPieceNode : public MiniPieceNode
{
	private:
	protected:
	public:
				Bool			goodUpgrade;
				Real			lastTrigger;
				Real			lastUpgrade;
				AUMiniPieceNode (PiecePtr piec, Real time);
};

//------------------------------------------------------------------------------
AUMiniPieceNode::AUMiniPieceNode (PiecePtr piec, Real time) :
MiniPieceNode (piec)
{
	goodUpgrade = FALSE;
	lastTrigger = lastUpgrade = time;
}

//------------------------------------------------------------------------------
AmmoUpgrader::AmmoUpgrader (int typ, int id, Point_2d loc, Real flowRat) :
Player ("Shipyard"),
Device (id),
soundEmitter (id, loc, kSndUpgrade, kSndUpgrStart, kSndUpgrStop)
{
	flowRate = flowRat;
	if (flowRate == ZERO)
		DEBUG_STR ("Hey!  Level Designer!  Ammo Upgrader (trigger #%d) has a ZERO flow rate!  Bo-gus!\n", id);
	type = typ;
	currentTime = ZERO;
	bingCountdown = kBingDelay;
}

//------------------------------------------------------------------------------
AmmoUpgrader::~AmmoUpgrader (void)
{
}

//------------------------------------------------------------------------------
Bool	AmmoUpgrader::Play (void)
{
	Real lastTime = currentTime;
	currentTime = gReferee->Clock ();
	if (currentTime < lastTime)
		recentPieces.Reset ();

	Bool upgrading = FALSE;
	AUMiniPieceNode *nextNode = (AUMiniPieceNode *) recentPieces.Head ();
	while (nextNode)
	{
		AUMiniPieceNode *thisNode = nextNode;
		nextNode = (AUMiniPieceNode *) recentPieces.Next (nextNode);
		Real sinceTrigger = currentTime - thisNode->lastTrigger;
		Real sinceUpgrade = currentTime - thisNode->lastUpgrade;

		upgrading |= thisNode->goodUpgrade;

		if (sinceTrigger >= kStepOffTime)
		{
			recentPieces.Remove (thisNode);
			delete thisNode;
		}
		else if (sinceUpgrade >= flowRate)
		{
			int numRes = sinceUpgrade / flowRate;
			if (thisNode->piece->AddResource (RESOURCE_AMMO_EXCHANGE, numRes))
			{
				if (bingCountdown <= ZERO)
				{
					gSoundMgr->PlaySound (kSndBing, thisNode->piece);
					bingCountdown = kBingDelay;
				}
				else
					bingCountdown -= gReferee->Interval ();

				thisNode->goodUpgrade = TRUE;
				thisNode->lastUpgrade = currentTime;
				upgrading = TRUE;
			}
			else
				thisNode->goodUpgrade = FALSE;
		}
	}

	Bool	emitting = soundEmitter.IsEmitting ();
	if (upgrading)
	{
		if (!emitting)
			soundEmitter.SwitchOn ();
	}
	else
	{
		if (emitting)
			soundEmitter.SwitchOff ();
	}
	return TRUE;
}

//------------------------------------------------------------------------------
int	AmmoUpgrader::Trigger (const PiecePtr &piece, int code, int code2)
{
	Device::Trigger (piece, code, code2);
	if ((code == DEV_TRIGGER) && power && piece ())
	{
		AUMiniPieceNode *node = (AUMiniPieceNode *) recentPieces.Head ();
		Bool notFound = TRUE;
		while (node && notFound)
		{
			if (node->piece () == piece ())
			{
				node->lastTrigger = currentTime;
				notFound = FALSE;
			}
			node = (AUMiniPieceNode *) recentPieces.Next (node);
		}
		if (notFound)
			recentPieces.InsertAfter (new AUMiniPieceNode (piece, currentTime));
	}
	return 0;
}

//------------------------------------------------------------------------------
