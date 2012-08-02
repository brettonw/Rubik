//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/21/96
//
//------------------------------------------------------------------------------

#ifndef	FLOOR_TRIGGER
#define	FLOOR_TRIGGER

//------------------------------------------------------------------------------
#include "list.h"
#include "piece ptr.h"

//------------------------------------------------------------------------------
class TFloor																																		//	the floor trigger array
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				int				width;																												//	the width of the grid
				int				height;																												//	the height of the grid
				List			***grid;																											//	the trigger array
	public:																																				//	members available externally
				TFloor (void);																													//	constructor
virtual	~TFloor (void);																													//	destructor
virtual	void			Trigger (const PiecePtr &piece);															//	test for triggers
};																																							//	end

//------------------------------------------------------------------------------
extern	TFloor	*gFloor;																												//	global floor trigger set

//------------------------------------------------------------------------------

#endif	//	FLOOR_TRIGGER
