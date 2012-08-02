//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/29/97
//
//	02/21/97	JB		added team identification
//
//------------------------------------------------------------------------------

#ifndef	PLAYER
#define	PLAYER

//------------------------------------------------------------------------------
class Player																																		//	a player in the game
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				char			name[16];																											//	name of the player
				int				kills;																												//	keep track of how many kills this player got
				int				deaths;																												//	number of times this player has died
				int				turretkills;																									//	number of turret kills
				int				resources;																										//	number of resources
				int				suicides;																											//	number of suicides
				int				score;																												//	the player's score
				int				team;																													//	the player's team ID
	public:																																				//	members available externally
				Player (cstr name);																											//	constructor
virtual	~Player (void);																													//	destructor
virtual	Bool			Play (void) = 0;																							//	play one frame
				cstr			Name (void) {return name;}																		//	return the name of the player
				void			AddScore (int k, int d, int tk, int r, int s, int sc);				//	add score
				int				GetKills (void) {return kills;}																//	return kills
				int				GetDeaths (void) {return deaths;}															//	return deaths
				int				GetTurretKills (void) {return turretkills;}										//	return turretkills
				int				GetResources (void) {return resources;}												//	return resources
				int				GetSuicides (void) {return suicides;}													//	return suicides
				int				GetScore (void) {return score;}																//	return score
				int				Team (void);																									//	return the team ID
				void			SetTeam (int team);																						//	set the team ID
};																																							//	end

//------------------------------------------------------------------------------
inline	void	Player::AddScore (int k, int d, int tk, int r, int s, int sc)			//	add score stats tot he player records
{																																								//	begin
	kills += k;																																		//	add kills
	deaths += d;																																	//	add deaths
	turretkills += tk;																														//	add turretkills
	resources += r;																																//	add resources
	suicides += s;																																//	add suicides
	score += sc;																																	//	add score
}																																								//	end

//------------------------------------------------------------------------------
inline	int	Player::Team (void)																									//	return the team ID
{																																								//	begin
	return team;																																	//	return the team ID
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Player::SetTeam (int teem)																				//	set the team ID
{																																								//	begin
	team = teem;																																	//	set the team ID
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	PLAYER
