//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Last Revision:	01/22/97
//
//	Revisions:			Modified Face & Cilia to use ConstrainedTarget
//									02/13/97:	added a class for mark's turret.
//	03/07/97	JB		ForeHaunch becomes Haunch.
//
//------------------------------------------------------------------------------

#ifndef JENNY_PART_THUGS
#define JENNY_PART_THUGS

//------------------------------------------------------------------------------
#include "player.h"
#include "piece ptr.h"
#include "device.h"

//------------------------------------------------------------------------------
class	JennyPartThug : public Player, public Device
{
	private:
	protected:
				PiecePtr	target;																												//	the target piece
				PiecePtr	turret;																												//	pointer to the turret belonging to this thug
				Vector_2d	orient;																												//	default orientation of the turret
				Bool			firing;
	public:
				JennyPartThug (int id);
virtual	~JennyPartThug (void);
virtual	Bool			Play (void);
virtual	int				Trigger (const PiecePtr &piece, int code, int code2);
				void			SetTarget (const PiecePtr &piece);														//	set the target piece
virtual	Bool			Target (void);
virtual	Bool			LeadTarget (void);
virtual	Bool			NoLeadTarget (void);
virtual	Bool			ConstrainedTarget (void);
//virtual	Bool			ConstrainedTargetPulse (void);
virtual	Bool			SwitchOff (void);
};

//------------------------------------------------------------------------------
inline	void	JennyPartThug::SetTarget (const PiecePtr &piece)										//	set the target piece
{																																								//	begin
	target = piece;																																//	copy the target
}																																								//	end

//------------------------------------------------------------------------------
class	JFaceThug : public JennyPartThug
{
	private:
	protected:
	public:
				JFaceThug (CPoint tLoc, Real rLoc, int type, int id);
virtual	~JFaceThug (void);
virtual	Bool			Target (void);
};

//------------------------------------------------------------------------------
class	JJawThug : public JennyPartThug
{
	private:
	protected:
				Real			nextPulseTime;
	public:
				JJawThug (CPoint tLoc, Real rLoc, int type, int id);
virtual	~JJawThug (void);
//virtual	Bool			Target (void);
};

//------------------------------------------------------------------------------
class	JMandibleThug : public JennyPartThug
{
	private:
	protected:
	public:
				JMandibleThug (CPoint tLoc, Real rLoc, int type, int id);
virtual	~JMandibleThug (void);
virtual	Bool			Target (void);
};

//------------------------------------------------------------------------------
class	JLegThug : public JennyPartThug
{
	private:
	protected:
	public:
				JLegThug (CPoint tLoc, Real rLoc, int type, int id);
virtual	~JLegThug (void);
};

//------------------------------------------------------------------------------
class	JCiliaThug : public JennyPartThug
{
	private:
	protected:
	public:
				JCiliaThug (CPoint tLoc, Real rLoc, int type, int id);
virtual	~JCiliaThug (void);
virtual	Bool			Target (void);
};

//------------------------------------------------------------------------------
class	JHaunchThug : public JennyPartThug
{
	private:
	protected:
	public:
				JHaunchThug (CPoint tLoc, Real rLoc, int type, int id);
virtual	~JHaunchThug (void);
};

//------------------------------------------------------------------------------
class	JMarkThug : public JennyPartThug
{
	private:
	protected:
	public:
				JMarkThug (CPoint tLoc, Real rLoc, int type, int id);
virtual	~JMarkThug (void);
};

//------------------------------------------------------------------------------

#endif // JENNY_PART_THUGS
