//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Last Revision:	08/21/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "ship upgrader.h"
#include "resource.h"
#include "referee.h"
#include "sprite array mgr.h"
#include "view.h"

//------------------------------------------------------------------------------
ShipUpgrader::ShipUpgrader (int typ, int id, Point_2d loc) : Device (id)
{
	type = typ;
}

//------------------------------------------------------------------------------
ShipUpgrader::~ShipUpgrader (void)
{
}

//------------------------------------------------------------------------------
int	ShipUpgrader::Trigger (const PiecePtr &piece, int code, int code2)					//	trigger the device
{
	Device::Trigger (piece, code, code2);
	if ((code == DEV_TRIGGER) & power)
		piece->AddResource (type, 1);
	return 0;
}

//------------------------------------------------------------------------------
