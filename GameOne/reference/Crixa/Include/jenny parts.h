//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Last Revision:	01/22/96
//
//	03/07/97	JB		changed JForeHaunchPart to JHaunchPart
//
//------------------------------------------------------------------------------

#ifndef JENNY_PARTS
#define JENNY_PARTS

//------------------------------------------------------------------------------
#include "turret.h"

//------------------------------------------------------------------------------
class	JennyPart : public Turret
{
	private:
	protected:
				Real			damageSum;
	public:
				JennyPart (Player *own, Real radius, CPoint loc, Real rLoc, SArrayPtr spr);
virtual	~JennyPart (void);
virtual	void			Explode (void);
virtual	void			Damage (const PiecePtr &piece, Real damage);
virtual	Bool			Step (Real time);																							//	step the piece ahead in time
				void			Kill (void);
};

//------------------------------------------------------------------------------
class	JFacePart : public JennyPart
{
	private:
	protected:
	public:
				JFacePart (Player *own, Real radius, CPoint loc, Real rLoc, SArrayPtr spr);
virtual	~JFacePart (void);
};

//------------------------------------------------------------------------------
class	JJawPart : public JennyPart
{
	private:
	protected:
	public:
				JJawPart (Player *own, Real radius, CPoint loc, Real rLoc, SArrayPtr spr);
virtual	~JJawPart (void);
};

//------------------------------------------------------------------------------
class	JMandiblePart : public JennyPart
{
	private:
	protected:
	public:
				JMandiblePart (Player *own, Real radius, CPoint loc, Real rLoc, SArrayPtr spr);
virtual	~JMandiblePart (void);
};

//------------------------------------------------------------------------------
class	JLegPart : public JennyPart
{
	private:
	protected:
	public:
				JLegPart (Player *own, Real radius, CPoint loc, Real rLoc, SArrayPtr spr);
virtual	~JLegPart (void);
};

//------------------------------------------------------------------------------
class	JCiliaPart : public JennyPart
{
	private:
	protected:
	public:
				JCiliaPart (Player *own, Real radius, CPoint loc, Real rLoc, SArrayPtr spr);
virtual	~JCiliaPart (void);
};

//------------------------------------------------------------------------------
class	JHaunchPart : public JennyPart
{
	private:
	protected:
	public:
				JHaunchPart (Player *own, Real radius, CPoint loc, Real rLoc, SArrayPtr spr);
virtual	~JHaunchPart (void);
};

//------------------------------------------------------------------------------
class	JMarkPart : public JennyPart
{
	private:
	protected:
	public:
				JMarkPart (Player *own, Real radius, CPoint tLoc, Real rLoc, SArrayPtr spr);
virtual	~JMarkPart (void);
};

//------------------------------------------------------------------------------

#endif // JENNY_PARTS
