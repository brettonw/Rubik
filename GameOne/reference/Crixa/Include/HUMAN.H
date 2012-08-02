//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	02/04/97
//  02/11/97	MK		Added 2 parameters thrusting & thrustsoundID
//	02/27/97	DS		Added GetLives() and AddLife() accessors
//	03/20/97	DS		made Reset() virtual
//------------------------------------------------------------------------------

#ifndef	HUMAN
#define	HUMAN

//------------------------------------------------------------------------------
#include "tqueue.h"
#include "input code.h"
#include "player.h"
#include "piece ptr.h"
#include "lock ptr.h"

//------------------------------------------------------------------------------
class Human : public Player																											//	a human player in the game
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				
																																								//  (MK)
				Bool					thrusting;																								//  flag to indicate ship is thrusting
				long					thrustsoundID;																						//  Id for thrusting sound
																																								//  (MK)																																		
				int						id;
				Point_2d			loc;
				Real					rloc;
				Vector_2d			camera;
				Real					lastLife;
				int						shakeFrame;
				Vector_2d			shakeVector;
				LockPtr				lock;																											//	pointer to the target lock
				TQueue<Code>	inputs;																										//	input code queue
				PiecePtr			crixa;																										//	the pointer to the crixa ship piece
				int						lives;																										//	how many lives you have left
				int						stillPlaying;
	protected:																																		//	members internal to this class hierarchy
				Bool					NewCrixa (CPoint where, Real orientation);								//	allocate a new crixa ship
				void					GetLock (void);																						//	get a lock
				void					ComputeCamera (void);																			//	compute the camera location for the crixa ship
	public:																																				//	members available externally
				Human (int id = 0);																											//	constructor
				Human (cstr name, int id);																							//	constructor
virtual	~Human (void);																													//	destructor
virtual	Bool					Play (void);																							//	play one frame
				Bool					DoCrixa (Code *code);																			//	play the crixa ship
				Vector_2d			Camera (void);																						//	return the location of the player's camera
				PiecePtr			GetCrixa (void);																					//	return a pointer to the crixa ship
virtual	void					Reset (void);																							//	reset the local human player
				void					Input (Code *code);																				//	enqueue an input code for the player
				int						Id (void);																								//	return the human id
				void					SetId (int id);																						//	set the human id
				void					Quit (void);																							//	have this player stop playing
				int						GetLives (void);																					//	return number of lives
				bool					AddLife (void);																						//	add a life to this player, return TRUE if successful
};																																							//	end

//------------------------------------------------------------------------------
inline	PiecePtr	Human::GetCrixa (void)																				//	return a pointer to the crixa ship
{																																								//	begin
	return crixa;																																	//	return the ship
}																																								//	end

//------------------------------------------------------------------------------
inline	int		Human::GetLives (void)																						//	return lives
{																																								//	begin
	return lives;																																	//	return the number of lives
}																																								//	end


//------------------------------------------------------------------------------
inline	void	Human::Input (Code *code)																					//	enqueue an input code for the player
{																																								//	begin
	inputs.Enqueue (*code);																												//	enqueue the input code
}																																								//	end

//------------------------------------------------------------------------------
inline	int		Human::Id (void)																									//	return the human id
{																																								//	begin
	return id;																																		//	return the id
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Human::SetId (int i)																							//	set the human id
{																																								//	begin
	id = i;																																				//	set the id
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	HUMAN
