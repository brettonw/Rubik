//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//
//	??/??/96	JB		Added code to read in moving sequences from the device file.
//	??/??/96	JB		Teleportation
//	??/??/96	JB		Laser Barriers
//	01/20/97	JB		Jenny-Turret installation code
//	01/23/97	JB		BossGen installation code
//	01/23/97	JB		Changed powerdevices array from class PowerDevice to class
//									Device in order to accomodate the BossGen device.
//	02/03/97	JB		added proper ForceMat support, destructable balls.
//	02/06/96	JB		commenting & description.
//	02/25/97	DS		Added SetTrigger() to PowerBumper, if trigger != 0
//									For support of gamestates.
//	03/05/97	DS		Added BossGen trigger (trigger, 0, 0)
//									*note* it's a weird thing, this "boss generator"
//	03/07/97	JB		fixed generic device loading (was looking for "gdv" instead
//									of "generic")
//	03/12/97	JB		reading in the boss door player (door #14).
//	03/18/97	DS		added regenerating resources to Play(), Constructor and Detructor
//
//------------------------------------------------------------------------------

/*
DESCRIPTION:
the shipyard stinks.  additionally, it's a pain in the ass to manage.  it's
responsible for loading all the devices into the world, connecting the power
systems, etc., etc., etc.
*/


//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "view.h"
#include "shipyard.h"
#include "math utility.h"
#include "player node.h"
#include "text buffer.h"
#include "level file.h"
#include "stack.h"
#include "sprite array mgr.h"
#include "sprite sequence.h"
#include "stationary sequence.h"
#include "time sequence.h"
#include "path sequence.h"
#include "turret thug.h"
#include "door player.h"
#include "trigger array.h"
#include "power device.h"
#include "generator.h"
#include "bumper player.h"
#include "power mat.h"
#include "switch sequence.h"
#include "laser barrier gen.h"
#include "ship.h"
#include "referee.h"
#include "teleporter.h"
#include "resource gen.h"
#include "ammo upgrader.h"
#include "dest wall.h"
#include "resource.h"

#include "jenny part thugs.h"
#include "boss gen.h"
#include "resource ball.h"
#include "furnace thug.h"
#include "force mat.h"
#include "debugger.h"

#include "boss door player.h"

#include "regen piece node.h"																										//	regenerating piece node class!
#include "get parameters.h"
#include "network play.h"

//------------------------------------------------------------------------------
Shipyard	*gShipyard;																														//	global shipyard player


//------------------------------------------------------------------------------
Shipyard::Shipyard (void) : Player ("Shipyard")																	//	constructor
{																																								//	begin
	LevFile		lev ("Devices");
	TxtBuffer txt (&lev);																													//	load the device file
	SArrayMgr	sprites;																														//	an internal sprite manager
	cstr			line,																																//	place to get the beginning of each line
						title;																															//	place to get the name of each device
	int				numLayers,																													//	place to get the number of layers of devices
						numDevices,
						numPowerDevices;
	List			genlist;
	Real			x, y;																																//	places to get the location of each device
	int				type,																																//	place to get the piece type
						trigger;																														//	place to get the trigger id

	txt >> numDevices >> numPowerDevices >> numLayers;														//	get the number of devices, powerdevices, and layers

	Device **devices = new Device *[numDevices];																	//	create the device array
	for (int i = 0; i < numDevices; i++)																					//	loop through the device array
		devices[i] = 0;																															//	and initialize the elements
	Device **powerdevices = new Device *[numPowerDevices];												//	create the power devicearray
	for (i = 0; i < numPowerDevices; i++)																					//	loop through the power device array
		powerdevices[i] = 0;																												//	and initialize the elements

	txt >> line;																																	//	next line
	while (strcmp (line, "end"))																									//	loop until an "end" statement is encountered
	{																																							//	begin
		int		deviceIndex;																													//	placeholder for the current device index
		title = line;																																//	save the title (filename)
		txt >> deviceIndex >> type >> trigger;																			//	get the device index, type, and trigger
		DeviceType dtype;																														//	devicetype 
		if 			(strcmp (title, "turret") == 0)				dtype = D_TURRET;							//	check if the device is a turret
		else if	(strcmp (title, "door") == 0)					dtype = D_DOOR;								//	check if the device is a door
		else if (strncmp (title, "tele", 4) == 0)			dtype = D_TELEPORT;						//	check if the device is a teleporter
		else if (strncmp (title, "lasbar", 6) == 0)		dtype = D_LASERBARRIER;				//	check if the device is a laser barrier
		else if (strncmp (title, "rgen", 4) == 0)			dtype = D_RESOURCEGEN;				//	check if the device is a resource generator
		else if (strncmp (title, "upgr", 4) == 0)			dtype = D_UPGRADER;
		else if (strncmp (title, "dwall", 5) == 0)		dtype = D_DESTWALL;
		else if (strncmp (title, "dball", 5) == 0)		dtype = D_DESTBALL;
		else if (strncmp (title, "resource", 8) == 0)	dtype = D_RESOURCE;
		else if (strncmp (title, "forcemat", 8) == 0)	dtype = D_FORCEMAT;
		else if (strncmp (title, "generic", 7) == 0)	dtype = D_GENERIC;
		else																																				//	otherwise
			dtype = D_NONE;																														//	no such device

		switch (dtype)																															//	switch on device type
		{																																						//	begin
		case	D_TURRET:																															//	if it's a turret
		{																																						//	begin
			Real		orient;																														//	get the turret orientation
			txt >> orient >> x >> y;																									//	get the orientation, location , and next title
			orient = DegreesToRadians (orient);																				//	convert the orientation from degrees to radians
			switch (type)
			{
				//	jenny turret thugs
				case	10:
				{
					JFaceThug		*jface = new JFaceThug (Point_2d (x, y), orient, type, trigger);
					devices[deviceIndex] = jface;
					PlayerNode	*node = new PlayerNode (jface);
					players.InsertAfter (node);
					gTrigger->SetTrigger (trigger, jface);
				} break;
				case	11:
				case	12:
				{
					JJawThug		*jjaw = new JJawThug (Point_2d (x, y), orient, type, trigger);
					devices[deviceIndex] = jjaw;
					PlayerNode	*node = new PlayerNode (jjaw);
					players.InsertAfter (node);
					gTrigger->SetTrigger (trigger, jjaw);
				} break;
				case	13:
				case	14:
				{
					JMandibleThug		*jmand = new JMandibleThug (Point_2d (x, y), orient, type, trigger);
					devices[deviceIndex] = jmand;
					PlayerNode	*node = new PlayerNode (jmand);
					players.InsertAfter (node);
					gTrigger->SetTrigger (trigger, jmand);
				} break;
				case	15:
				case	16:
				case	17:
				case	18:
				case	21:
				case	22:
				{
					JLegThug		*jleg = new JLegThug (Point_2d (x, y), orient, type, trigger);
					devices[deviceIndex] = jleg;
					PlayerNode	*node = new PlayerNode (jleg);
					players.InsertAfter (node);
					gTrigger->SetTrigger (trigger, jleg);
				} break;
				case	23:
				case	24:
				{
					JCiliaThug		*jcil = new JCiliaThug (Point_2d (x, y), orient, type, trigger);
					devices[deviceIndex] = jcil;
					PlayerNode	*node = new PlayerNode (jcil);
					players.InsertAfter (node);
					gTrigger->SetTrigger (trigger, jcil);
				} break;
				case	19:
				case	20:
				case	25:
				case	26:
				{
					JHaunchThug		*jhaun = new JHaunchThug (Point_2d (x, y), orient, type, trigger);
					devices[deviceIndex] = jhaun;
					PlayerNode	*node = new PlayerNode (jhaun);
					players.InsertAfter (node);
					gTrigger->SetTrigger (trigger, jhaun);
				} break;

				//	furnace turret thug
				case	30:
				{
					FurnaceThug		*furnace = new FurnaceThug (Point_2d (x, y), orient, R(40.0), gSpriteMgr.Fetch ("turret30"), trigger);
					devices[deviceIndex] = furnace;
					PlayerNode *node = new PlayerNode (furnace);
					players.InsertAfter (node);
					gTrigger->SetTrigger (trigger, furnace);
				} break;

				case	40:
				case	41:
				case	42:
				{
					JMarkThug	*jmark = new JMarkThug (Point_2d (x, y), orient, type, trigger);
					devices[deviceIndex] = jmark;
					PlayerNode *node = new PlayerNode (jmark);
					players.InsertAfter (node);
					gTrigger->SetTrigger (trigger, jmark);
				} break;


				//	regular turret
				default:
				{
					TurretThug	*thug = new TurretThug (Point_2d (x, y), orient, type, trigger);//	create the turret
					devices[deviceIndex] = thug;																							//	save a pointer to the turret thug
					PlayerNode	*node = new PlayerNode (thug);																//	create a node for the turret player
					players.InsertAfter (node);																								//	store the turret player in the sub-player list
					gTrigger->SetTrigger (trigger, thug);																			//	link the turret to the trigger device
				}
			}
		}	break;																																		//	end / break
		
		case	D_DOOR:																																//	if it's a door
		{																																						//	begin
			txt >> x >> y;																														//	get the location and next title
			Point_2d doorloc (x, y);
			DoorPlayer *door;
			if (type == 14)
				door = new BossDoorPlayer (doorloc, type, trigger);
			else
				door = new DoorPlayer (doorloc, type, trigger);
//			DoorPlayer	*door = new DoorPlayer (Point_2d (x, y), type, trigger);			//	create the door
			devices[deviceIndex] = door;																							//	save a pointer to the door player
			PlayerNode	*node = new PlayerNode (door);																//	create a node for the door player
			players.InsertAfter (node);																								//	store the door player in the sub-player list
			gTrigger->SetTrigger (trigger, door);																			//	link the door to the trigger device
		}	break;
		
		case D_TELEPORT:
		{
			Real	dest_x,
						dest_y;
			int		layer;
			
			txt >> x >> y >> dest_x >> dest_y >> layer;
			Teleporter *tele = new Teleporter (trigger, Point_2d (dest_x, dest_y));
			devices[deviceIndex] = tele;
			PlayerNode *node = new PlayerNode (tele);
			players.InsertAfter (node);
			gTrigger->SetTrigger (trigger, tele);
			StatSeq *statseq = new StatSeq (Point_2d (x, y), SeqPtr (new SpriteSeq (gSpriteMgr.Fetch (title))));
			gView->Install (SeqPtr (statseq) ,layer);
		} break;
		
		case D_LASERBARRIER:
		{
			Real width, height, damage;
			
			txt >> damage >> x >> y >> width >> height;
			LaserBarrierGen *lbg = new LaserBarrierGen (type, trigger, x, y, width, height, damage);
			devices[deviceIndex] = lbg;
		}	break;

		case D_RESOURCEGEN:
		{	
			int layer;
			Real velocity, delay;
			
			txt >> velocity >> delay >> x >> y >> layer;
			Point_2d pos (x, y);
			ResourceGen *rg = new ResourceGen (type, trigger, pos, velocity, delay);
			PlayerNode *node = new PlayerNode (rg);
			players.InsertAfter (node);
			gTrigger->SetTrigger (trigger, rg);
			devices[deviceIndex] = rg;
			StatSeq *statseq = new StatSeq (Point_2d (x, y), SeqPtr (new SpriteSeq (gSpriteMgr.Fetch (title))));
			gView->Install (SeqPtr (statseq), layer);
		}	break;

		case D_UPGRADER:
		{
			int		layer;
			Real	flowRate;
			txt >> flowRate >> x >> y >> layer;
			AmmoUpgrader *ammoUpgr = new AmmoUpgrader (type, trigger, Point_2d (x, y), flowRate);
			players.InsertAfter (new PlayerNode (ammoUpgr));
			gTrigger->SetTrigger (trigger, ammoUpgr);
			devices[deviceIndex] = ammoUpgr;

		} break;
		
		case D_DESTWALL:
		{
			char	*fname;
			int		layer;
			Real	armor, width, height;
			txt >> armor >> x >> y >> width >> height >> layer >> fname;
			SeqPtr seq (new SpriteSeq (gSpriteMgr.Fetch (fname)));
			DestWall *dwall = new DestWall (this, armor, Point_2d (x, y), width, height, seq, layer);
			gReferee->Install (dwall, FULL_INTERSECT);
			// ds add piece to regeneration list
			if ((strcmp(fname,"dwall02")==0) && (gClient))
			{
				RegenPieceNode *node = new RegenPieceNode (dwall, type, Point_2d (x,y), "dwall02", armor, width, height, layer);
				regenList.InsertAfter(node);
			}
		} break;
		
		case D_DESTBALL:
		{
			Real	armor,
						radius;
			int		resType,
						resAmount;
			cstr	spr_name;
			txt >> armor >> resType >> resAmount >> radius >> x >> y >> spr_name;
			ResBall	*rBall = new ResBall (this, armor, Point_2d (x, y), radius,
											 gSpriteMgr.Fetch (spr_name), resType, resAmount);
			gReferee->Install (rBall, NORMAL_INTERSECT);
		} break;

		case D_RESOURCE:
		{
			int amount;
			txt >> amount >> x >> y;
 			Resource *resource = new Resource  (R(4.0), Point_2d (x, y), R(0.0), gSpriteMgr.Fetch ("res"), type, amount);
			gReferee->Install (resource, MOBILE_INTERSECT);
			// ds add piece to regeneration list!
			if (gClient)
			{
				RegenPieceNode *node = new RegenPieceNode (resource, type, Point_2d (x,y), amount);
				regenList.InsertAfter(node);
			}
		} break;

		case D_FORCEMAT:
		{
			Real	force;
			Real	direction;
			txt >> force >> direction;
			direction = DegreesToRadians (direction);
			ForceMat *forceMat = new ForceMat (force, direction, trigger);
			gTrigger->SetTrigger (trigger, forceMat);
			devices[deviceIndex] = forceMat;
		} break;

		case	D_GENERIC:
		{
			Real	x,
						y;
			cstr	spr_name;
			switch (type)
			{
				case 00:
				{
					//	an invisible sound emitter
					int	emSnd;
					int	startSnd;
					int	stopSnd;
					txt >> emSnd >> startSnd >> stopSnd;
					txt >> x >> y >> spr_name;
					Point_2d loc (x, y);
					EmitterDevice *emDev = new EmitterDevice (trigger, loc, emSnd, startSnd, stopSnd);
					devices[deviceIndex] = emDev;
					//	register this device with the shipyard for destruction on exit
				}	break;
				
				default:
					DEBUG_STR ("ERROR (shipyard): Unknown Generic Device %s %d %d %d\n",
										line, deviceIndex, type, trigger);
			}
		}	break;

		default:
			DEBUG_STR ("ERROR (shipyard): Unknown Device %s %d %d %d\n",
								line, deviceIndex, type, trigger);
			break;
		}																																						//	end
		txt >> line;
	}																																							//	end
	for (i = 0; i < numLayers; i++)																								//	loop through the layers
	{																																							//	begin
		txt >> line;																																//	get the next line
		while (strcmp (line, "end") != 0)																						//	while not at the end of the layer
		{																																						//	begin
			PDeviceType object;																												//	object identifier
			if 			(strncmp (line, "pg", 2) == 0)	object = P_GENERATOR;							//	it's a generator
			else if (strncmp (line, "pp", 2) == 0)	object = P_PIPE;									//	it's a pipe
//			else if (strncmp (line, "ps", 2) == 0)	object = P_SWITCH;								//	it's a switch
//			else if (strncmp (line, "px", 2) == 0)	object = P_MULTI;									//	it's a multiplex
			else if (strncmp (line, "pb", 2) == 0)	object = P_BUMPER;								//	it's a bumper
			else if (strncmp (line, "pm", 2) == 0)	object = P_MAT;										//	it's a pressure mat
			else																																			//	otherwise
				object = P_NONE;																												//	none of the above
			char linea[256];																													//	linea holds the animated power piece sprite name
			strcpy (linea, line);																											//	copy the name
			strcat (linea, "a");																											//	append an 'a'

			int		pDeviceIndex,																												//	pDeviceIndex is the power device index id
						width,																															//	the width of the device
						height;																															//	the height of the device

			txt >> pDeviceIndex >> type >> trigger >> x >> y;													//	get the index, the type, the trigger, and the position
			SwitchSeq *switchseq;																											//	a pointer to a switch sequence
			switch (object)																														//	switch
			{																																					//	begin
				case P_GENERATOR:																												//	if it's a generator
				{																																				//
					switch (type)																													//	check what type of generator we are loading
					{																																			//
						// ****** Mod: BossGenerator (012397 JB) ******
						case	01:																														//	type 01: BossGen
						{																																		//
							GenDev *bGenDev = new GenDev (0);																	//	create a new generator device
							powerdevices[pDeviceIndex] = bGenDev;															//	register it as a power device (for linkage)
							genlist.InsertAfter (new DeviceNode (bGenDev));										//	register it as a generator (for startup)
							BossGen *bGen = new BossGen (this, bGenDev, R(1.0e2),							//	create a boss generator
											Point_2d (x, y), R(70.0), gSpriteMgr.Fetch (line));				//
							bGen->SetDeathTrigger(trigger,0,0);																//	
							gReferee->Install (bGen, FULL_INTERSECT);													//	install the boss gen piece into the world
						} break;																														//

						default:																														//	default: regular generator
						{																																		//
							switchseq = new SwitchSeq (Point_2d (x, y),												//	create a new switch sequence
													SeqPtr (new SpriteSeq (gSpriteMgr.Fetch (line))),			//	with a plain 'off' sprite
													SeqPtr (new TimeSeq (gSpriteMgr.Fetch (linea), TRUE)));//	and an animated 'on' sprite
							gView->Install (SeqPtr (switchseq), i);														//	install the sequence
							Generator *gen = new Generator (switchseq);												//	create the generator
							if (trigger)																											//	if this device is to be triggered
								gTrigger->SetTrigger (trigger, gen);														//	set the trigger
							powerdevices[pDeviceIndex] = gen;																	//	save a pointer to the generator in the device list
							genlist.InsertAfter (new DeviceNode (gen));												//	save a pointer to the generator in a generator specific list
						} break;																														//
					}																																			//
				}	break;																																//

				case P_PIPE:																														//	if it's a pipe
				{
					switchseq = new SwitchSeq (Point_2d (x, y),														//	create a new switch sequence
								SeqPtr (new SpriteSeq (gSpriteMgr.Fetch (line))),								//	with a plain 'off' sprite
								SeqPtr (new TimeSeq (gSpriteMgr.Fetch (linea), TRUE)));					//	and an animated 'on' sprite
					gView->Install (SeqPtr (switchseq), i);																//	install the sequence
					powerdevices[pDeviceIndex] = new PowerDevice (switchseq, trigger);		//	create the pipe
				}	break;																																//	break

				case P_BUMPER:																													//	if it's a bumper
				{
					txt >> width >> height;																								//	we'll need the width and height
					BumpPlayer *pb = new BumpPlayer (type, Point_2d (x, y),								//	create a new bumper
								Real (width), Real (height), gSpriteMgr.Fetch (line), trigger);	//	create a new bumper
					powerdevices[pDeviceIndex] = pb;																			//	save a pointer to the bumper
					PlayerNode *pnode = new PlayerNode (pb);
					players.InsertAfter (pnode);
					if (trigger)																													//	ds: install the trigger
						gTrigger->SetTrigger (trigger,pb);
				}	break;																																//	break

				case P_MAT:																															//	if it's a pressure sensitive mat
				{																																				//
					switchseq = new SwitchSeq (Point_2d (x, y),														//	create a new switch sequence
											SeqPtr (new SpriteSeq (gSpriteMgr.Fetch (line))),								//	with a plain 'off' sprite
											SeqPtr (new TimeSeq (gSpriteMgr.Fetch (linea), TRUE)));					//	and an animated 'on' sprite
					gView->Install (SeqPtr (switchseq), i);																//	install the sequence
					PowerMat *pm = new PowerMat (type, switchseq, trigger);								//	create a new mat
					powerdevices[pDeviceIndex] = pm;																			//	save a pointer to the bumper
					PlayerNode *pnode = new PlayerNode (pm);															//	create a player node for the mat
					players.InsertAfter (pnode);																					//	insert the player into the shipyard's player list
					gTrigger->SetTrigger (trigger, pm);																		//	set the associated trigger for the mat
				}	break;																																//

				default:																																//	ignore invalid devices
					break;																																//
			}																																					//
			txt >> line;																															//	get the next line
		}																																						//
	}																																							//

	txt >> line;
	while (strcmp (line, "end"))
	{
		int	parent,
				child;
		txt >> parent >> child;
		ConnectType ctype;
		if      (!strcmp (line, "topower"))		ctype = P_TO_P;
		else if (!strcmp (line, "todevice"))	ctype = P_TO_D;
		else
			ctype = TO_NONE;

		switch (ctype)
		{
		case P_TO_P:
			powerdevices[parent]->Connect (powerdevices[child]);
			break;
		case P_TO_D:
			powerdevices[parent]->Connect (devices[child]);
			break;
		default:
			break;
		}			
		txt >> line;
	}
	//	launch the generators after this
	DeviceNode *gnode = (DeviceNode *) genlist.Head ();
	while (gnode)
	{
		gnode->device->Trigger (PiecePtr (NULL), DEV_TOGGLE, 0);
		gnode = (DeviceNode *) genlist.Next (gnode);
	}

	delete[] devices;
	delete[] powerdevices;

	// ds new additions
	rnode = (RegenPieceNode*) regenList.Head();																		//	initialize rnode to point to head of list
}																																								//	end

//------------------------------------------------------------------------------
Shipyard::~Shipyard (void)																											//	destructor
{																																								//	begin
	regenList.Reset();																														//	ds: release all nodes
}																																								//	end

//------------------------------------------------------------------------------
Bool	Shipyard::Play (void)																											//	play one frame
{																																								//	begin
	PlayerNode	*node = (PlayerNode *) players.Head ();														//	get the first node in the sub-player list
	while (node)																																	//	loop over all of the players in the list
	{																																							//	begin
		node->player->Play ();																											//	give the sub-player time
		node = (PlayerNode *) players.Next (node);																	//	move on to the next node
	}																																							//	end
	// ds respawning resources for network play
	if ((ParmInteger (RESPAWN_RESOURCES)) && (gClient))														//	only respawn if network play
	{
		int i = 5;																																	//	do only a few pieces every frame
		while ((rnode) && ((i--)>0))
		{
			rnode->CheckPiece();
			rnode = (RegenPieceNode *) regenList.Next (rnode);
		}
		if (rnode==0) rnode = (RegenPieceNode *) regenList.Head ();									//	if at end of list, then reset to beginning
	}
	return TRUE;																																	//	the shipyard is always part of the game
}																																								//	end

//------------------------------------------------------------------------------
