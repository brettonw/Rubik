//------------------------------------------------------------------------------
//	Copyright ©1997 qualia, inc. All rights reserved.
//
//	03/12/97	JB		creation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "boss door player.h"
#include "referee.h"
#include "sound enum.h"
#include "math utility.h"
#include "sound mgr.h"

//------------------------------------------------------------------------------
BossDoorPlayer::BossDoorPlayer (CPoint tLoc, int type, int trigger) :
DoorPlayer (tLoc, type, trigger)
{
}

//------------------------------------------------------------------------------
BossDoorPlayer::~BossDoorPlayer (void)
{
}

//------------------------------------------------------------------------------
Bool	BossDoorPlayer::Play (void)
{
	open |= stayopen;																															//	force open TRUE if stayopen TRUE. Heh.
	for (int i = 0; i < count; i++)																								//	loop over all of the door parts
	{
		Body	*body = part[i].piece->GetBody ();
		Real	deltaVelocity;
		if (power)
		{
			Vector_2d	deltaVector = body->TLocation () - part[i].lock;
			Real			position = deltaVector | part[i].axis,
								deltaPosition;
			if (open)
				deltaPosition = part[i].maxPosition - position;
			else
				deltaPosition = -position;
			deltaVelocity = deltaPosition / gReferee->Interval ();

			Real	v = body->TVelocity () | part[i].axis,
						fv = FABS (v);
			if ((FABS (deltaPosition) < R(5.0e-2)) && (fv > R(1.0e-2)))
				DoorStop (i);
			if (FABS (deltaPosition) > R(5.0))
			 if ((fv < EPSILON) || (Sign (deltaVelocity) != Sign (v)))
				DoorStart (i);
			if (FABS (deltaVelocity) > part[i].maxVelocity)
				deltaVelocity = Sign (deltaVelocity) * part[i].maxVelocity;
			deltaVelocity -= body->TVelocity () | part[i].axis;
		}
		else
		{
			deltaVelocity = -body->TVelocity () | part[i].axis;
			if ((FABS (deltaVelocity) > ZERO) && (FABS (deltaVelocity) < R(1.0e-2)))
				DoorStop (i);
		}
		Real	deltaForce = deltaVelocity * body->TInertia ();
		if (FABS (deltaForce) > part[i].maxForce)
			deltaForce = Sign (deltaForce) * part[i].maxForce;
		if (deltaForce != ZERO)
		{
			body->ApplyForce (part[i].axis * deltaForce);
			gReferee->Update (part[i].piece);
		}
	}
	open = FALSE;
	return TRUE;
}

//------------------------------------------------------------------------------
void	BossDoorPlayer::DoorStart (int which)
{
	gSoundMgr->PlaySound (doorStart, part[which].piece);
	emID = gSoundMgr->InsertEMSound (snd_movedoor, part[which].piece);
}

//------------------------------------------------------------------------------
void	BossDoorPlayer::DoorStop (int which)
{
	gSoundMgr->PlaySound (doorStop, part[which].piece);
	gSoundMgr->RemoveEMSound (snd_movedoor, emID);
}

//------------------------------------------------------------------------------
