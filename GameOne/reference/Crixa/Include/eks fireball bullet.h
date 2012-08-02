//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	02/21/97	JB		creation.
//
//------------------------------------------------------------------------------

#ifndef	EKS_FIREBALL_BULLET
#define	EKS_FIREBALL_BULLET

//------------------------------------------------------------------------------
#include "piece ptr.h"
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
class EksFireBullet : public Piece																							//	an eksekaren flaming ball o' death
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Real					damage;																										//	how much damage this bullet does
				PiecePtr			shooter;																									//	the piece that fired this bullet
				SArrayPtr			splut;																										//	the splut sprite
	public:																																				//	members available externally
				EksFireBullet (Real radius, CPoint location, SArrayPtr spr,							//	constructor
											 Real damage, const PiecePtr &shooter, SArrayPtr splut);	//	constructor (continued)
virtual	~EksFireBullet (void);																									//	destructor
virtual	Bool					Step (Real time);																					//	step the piece ahead in time
virtual	int						PostCollide (const PiecePtr &piece, Sect *sect);					//	do some work after a collision with another piece
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	EKS_FIREBALL_BULLET
