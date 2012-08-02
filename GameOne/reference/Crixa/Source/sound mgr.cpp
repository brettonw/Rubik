//------------------------------------------------------------------------------
//	"SoundMgr.cpp" 
//	4/12/96 
//	By Mark E. Kern	 
//	Copyright 1996, Qualia, Inc.
//	All Rights Reserved
//
//	** Confidential Intellectual Property **
//	This code may contain trade secrets and
//	other proprietary intellectual property
//	belonging to Qualia, Incorporated.
//
//	** No Unauthorized Use Permitted **
//
//  02/27/97	MK		Added PlayCDTrack & StopCD calls to support CD audio playback
//
//------------------------------------------------------------------------------


#ifdef	_MSC_VER
#include "precompile.h"
#endif

#include "sound mgr.h"
#include "text buffer.h"
#include "debugger.h"
#include "local human.h"
#include "get parameters.h"
#include "referee.h"



//------------------------------------------------------------------------------
#define	DONT_USE_SOUND	0

//------------------------------------------------------------------------------
SoundMgr	*gSoundMgr;																														//	global sound manager

//------------------------------------------------------------------------------
SoundMgr::SoundMgr(void)
{
	NumEmitters = 0;
	FirstEmitter = new emitter;
	LastEmitter = FirstEmitter;
	FirstEmitter->Sound = -1;
	FirstEmitter->SoundID = -1;
	FirstEmitter->previous = NULL;
	FirstEmitter->next = NULL;
	idTag	= 0;
	NumChannels = 0;
	CurChannel = 0;
	CurTime = 0;
	wCDid = 0;
	TxtBuffer	buffer ("sounds.h");
	int		numChans,
				numSounds;
	cstr	line;
	buffer >> numSounds >> line;

	clipDistance = ParmReal (AUDIO_CLIP_DISTANCE);
	numChans = ParmInteger (AUDIO_NUM_CHANNELS);
	scaleFactor = ParmReal (AUDIO_SCALE_FACTOR);
	panFactor = ParmReal (AUDIO_PAN_FACTOR);
	InitSound (numChans, numSounds);
	while (strcmp (line, "end"))
	{
		cstr	wave = line;
		int		maxPoly,
					priority,
					volume;
		buffer >> maxPoly >> priority >> volume >> line;
		if (!LoadSound (wave, maxPoly, priority, volume))
			DEBUG_STR ("Load WAVE file failed (%s)\n", wave);
	}
};

//------------------------------------------------------------------------------
SoundMgr::~SoundMgr(void)
{
};

//------------------------------------------------------------------------------

// SoundMgr::timefixChannels --------------------
// Updates all channel timestamps by dividing by 2
// Used to handle overflow of time.
void SoundMgr::timefixChannels(void){
	for(UINT i=0;i<NumChannels;i++)
		Channels[i].TimeStamp = Channels[i].TimeStamp/2;
};


// SoundMgr::insertSound ----------------------
// Takes a sound and inserts it in the given channel,
// while updating the channels records with the sound
// specifics.
//
void SoundMgr::insertSound(Sound *pSound, UINT myChan, UINT myVoice, UINT Priority, long ID)
{
		Channels[myChan].thisSound = pSound;
		Channels[myChan].InstanceID = myVoice;
		Channels[myChan].Priority = Priority;
		Channels[myChan].TimeStamp = CurTime;
		Channels[myChan].EmitterId = ID;
};


// SoundMgr::getLowest --------------------------
// Scans the channel array for lowest priority sound
// effect currently playing. If several of the same
// priority exists, it picks the least recently 
// triggered sound by comparing timestamps
//
// Assumptions:
//	NewPriority != 0
//  That a channel without a sound has a priority of 0
//
UINT SoundMgr::getLowest(UINT newPriority) {
	UINT lowchannel = UINT_MAX;
	UINT lowtime = UINT_MAX,
		 LowestSoFar = UINT_MAX;
		for (UINT i=0; i< NumChannels; i++){
			if (Channels[i].Priority <= LowestSoFar){
				if (Channels[i].Priority == newPriority){
					if (Channels[i].TimeStamp < lowtime)
					LowestSoFar = Channels[i].Priority;
					lowtime = Channels[i].TimeStamp;
					lowchannel = i;
				}
				else {
					LowestSoFar = Channels[i].Priority;
					lowtime = Channels[i].TimeStamp;
					lowchannel = i;
				}
			}
		}
		return lowchannel;
};


// SoundMgr::getOldest --------------------------
// Scans the channel array for instances of the sound
// specified by Sound object pointer. It then returns the oldest
// (last used) channel containing that sound. This is 
// called when all instances of a sound are already
// taken and we need to replace the oldest instance
// with a newly triggered one.
//
// Assumptions:
//	this routine assumes that pNewSound has no instances
//  free (i.e. that all instances are already playing
//  in the channel array. Otherwise, i returns a bogus
//  value of 0.
//
UINT SoundMgr::getOldest(Sound *pNewSound) {
	UINT oldest = 0,
		lowtime = UINT_MAX;

	for (UINT i=0; i< NumChannels; i++){
		if (Channels[i].thisSound == pNewSound)
			if (Channels[i].TimeStamp < lowtime){
				lowtime = Channels[i].TimeStamp;
				oldest = i;
			}	
	}
	return oldest;
};



// SoundMgr::getFreeChannel ----------------------
// Finds a free channel to play a sound. A free channel
// is either a channel that does not contain a sound,
// or a channel that has a lower priority than the 
// requested sound, or failing that, the oldest channel
// that has the same priority that the requested sound
// does. If there all channels currently playing have
// a higher priority than the requested sound, the 
// function fails and does not return a free channel.
//
// Assumptions:
//	that thisPriority is not 0 (no valid sound has priority 0),
// 
Bool SoundMgr::getFreeChannel(UINT *lpMyChan, UINT thisPriority) {
	int lowest;
	lowest = getLowest(thisPriority);
	if(thisPriority >= Channels[lowest].Priority){
		// Stop this channel if it has a sound in it
		if (Channels[lowest].thisSound != NULL)
			StopSound(lowest);
		*lpMyChan = lowest;
		return TRUE;
	} else
		return FALSE;
};

// SoundManger::updateChannels ---------------------
// This routine checks the status of each sound in
// the channel. If the sound has stopped playing, 
// mark it as freed.
//
void SoundMgr::updateChannels(void){	
	for(UINT i=0;i<NumChannels;i++){
		if (Channels[i].thisSound != NULL){
			if (!Channels[i].thisSound->playing(Channels[i].InstanceID))
				StopSound(i);
		}
	}
};

				

// SoundMgr::initSound ---------------------------
// This routine starts the DirectDraw Soundmanager, itializes the
// number of channels, allocates them (passed as a parameter), and sets he
// MgrExists flag to TRUE if everything is ok. 
// Will also allocate array to hold sounds as they get loaded. The number of
// sounds that will be loaded must be passed upon initialization.
// Otherwise
// the flag is set to false, and no other calls to this
// object will result in any action. MgrExists will be
// set to false if no DirectSound drivers or sound HW
// exists, or if there is an error in allocating anything
void SoundMgr::InitSound (UINT numChans, UINT numSounds)
{
	NumSounds = numSounds;	
	pSounds = new Sound[NumSounds];
	NumLoaded = 0;
	NumChannels = numChans;
	Channels = new channel[NumChannels];
	for(UINT i=0;i<NumChannels;i++)
	{
		Channels[i].thisSound = NULL;								
		Channels[i].InstanceID = 0;								
		Channels[i].Priority = 0;								
		Channels[i].TimeStamp = 0;
		Channels[i].EmitterId = -1;
	}
};



// SoundMgr::loadSound ---------------------------------------------------------
// loadSound will take the name of a sound and attempt to load it from disk into
// memory. It takes a string for the name, as well as the number of simultanious
// instances of that sound that can be played and a priority level for the sound.
// It stores the ID for the sound (which is also the index for that sound in the pSound
// array, in the soundID parameter. It returns TRUE if everything loaded properly, FALSE
// if otherwise.
Bool SoundMgr::LoadSound(cstr name, UINT maxpoly, UINT priority, UINT volume)
{
	if((NumLoaded != NumSounds) && pSounds[NumLoaded].loadSound (name, DirectX::sound, maxpoly, priority,volume))
	{
		//*soundID = NumLoaded;
		++NumLoaded;
		return TRUE;
	}
	return FALSE;
};


//------------------------------------------------------------------------------
Bool	SoundMgr::PlaySound (uint id, Bool loop, long volume, long freq)											//	simple sound play
{
#if	DONT_USE_SOUND
	return FALSE;
#endif
	return SMPlaySound (id, loop, volume, freq);
}


//-----------------------------------------------------------------------------------
// SoundManager::playSound
// Takes a point in space and applies distance attenuation to scale sound
//

Bool SoundMgr::PlaySound(uint SoundID, const PiecePtr &noisypiece, Bool loop, long attenuation, long freq){
#if DONT_USE_SOUND
	return FALSE;
#endif
	uint volume = 0;
	Real distance;
	Body	*soundbody, *crixabody;
	Point_2d  camerapoint, soundpoint, mypoint, crixapoint;
	Vector_2d soundvector;
	Real pan = 0;

	if(gReferee->Confirm(gLocal->GetCrixa()))
	{
		soundbody = noisypiece->GetBody();
		crixabody = gLocal->GetCrixa()->GetBody();

		soundpoint = soundbody->TLocation();
		crixapoint = crixabody->TLocation();
	
		if (SoundID >= NumSounds)
			DEBUG_SOFT("Bad Sound ID: %d \n", SoundID);


		soundvector = crixapoint - soundpoint;
		distance = abs(soundvector.Norm());
		pan = soundvector[X];

		
	//		DEBUG_SOFT("Dif in X Vector: %f \n", pan);

		pan = pan * panFactor;

		
		
		//	DEBUG_SOFT("Multiplied Value: %f \n", pan);
		//	DEBUG_SOFT("Distance Value: %f \n\n", distance);
		
		distance = distance * scaleFactor;

		if (distance <= clipDistance)
			return SMPlaySound (SoundID, loop, (long)-((distance)+ attenuation), freq, pan);
		else
			return FALSE;
	}
	return FALSE;
}

//------------------------------------------------------------------------------
// SoundManger::playSound ----------------------------------------------------------
// playSound attempts to play the requested sound 
// according to this logic:
//	- A request is made to play a sound
//  - If a channel is free, check for a free voice (instance) of the requested sound
//	- Play the sound in that channel, update the channel info
//	- If no channel is free, check for a channel with a lower priority (priority, time)
//	- If a lower channel exists, replace it with the new sound, else fail.
//
Bool SoundMgr::SMPlaySound(UINT SoundID, Bool loop, long volume, long freq, long pan){
#if	DONT_USE_SOUND
	return FALSE;
#endif
	UINT myChan;
	UINT myVoice;
	Bool freeChannel = FALSE;
	Bool freeInstance = FALSE;
		if(++CurTime = UINT_MAX) {																																		// Increment timestamp and check for overflow
			CurTime = CurTime/2;																																				// If overflow, reduce all current timestamps by 1/2
			timefixChannels();
		}

		if (SoundID >= NumSounds)
			DEBUG_SOFT("Bad Sound ID: %d \n", SoundID);

		freeChannel = getFreeChannel(&myChan, pSounds[SoundID].Priority);
		freeInstance = pSounds[SoundID].findFreeInstance(&myVoice);

		if(freeChannel){
			if(freeInstance){
				insertSound(&pSounds[SoundID], myChan, myVoice, pSounds[SoundID].Priority);
				Channels[myChan].thisSound->playSound(myVoice, DirectX::sound, loop, volume, freq, pan);				// Play the sucker!
				return TRUE;
			} else {
				myChan = getOldest(&pSounds[SoundID]);
				myVoice = Channels[myChan].InstanceID;
				StopSound(myChan);
				insertSound(&pSounds[SoundID], myChan, myVoice, pSounds[SoundID].Priority);
				Channels[myChan].thisSound->playSound(myVoice, DirectX::sound, loop, volume, freq, pan);				// Play the sucker!
			}
			return TRUE;
		}
		else
			return FALSE;
};



// SoundMgr::Step --------------------------------
// Step calls both updateChannels & StepEmitter. 
// updateChannels scans the Channel list for stopped sounds, and frees them for use.
// stepEmitter scans through the emitter list, restarting any emitters which have stopped
// and updating their volume parameters for distance cueing.

void SoundMgr::Step(void){
		updateChannels();
		StepEmitter();
};

// SoundMgr::stopSound ---------------------------
// Stops the sound playing in the given channel and updates its channel record
//
void SoundMgr::StopSound(UINT ThisChannel){
		// stopSound should never be called on a null sound channel, but if it is,
		// fix it and go-on.
		if(Channels[ThisChannel].thisSound != NULL) 
			Channels[ThisChannel].thisSound	->stopSound(Channels[ThisChannel].InstanceID);
		Channels[ThisChannel].thisSound = NULL;								
		Channels[ThisChannel].InstanceID = 0;								
		Channels[ThisChannel].Priority = 0;								
		Channels[ThisChannel].TimeStamp = 0;
		Channels[ThisChannel].EmitterId = -1;
};


// SoundMgr::qPlaying ---------------------------
// Checks to see if a particular sound (as indicated by its unique ID)
// is still playing in the channels structure. returns TRUE if it is.
//
Bool SoundMgr::qPlaying(long ID) {
	for(UINT i=0;i<NumChannels;i++){
		if ((Channels[i].thisSound != NULL) & (Channels[i].EmitterId == ID)){
			if (Channels[i].thisSound->playing(Channels[i].InstanceID))
				return TRUE;
			else 
				return FALSE;
		};
	};
	return FALSE;
};

// SoundMgr::EmitSound -----------------------------
// Only Emitter sounds get unique IDs, as such, they are installed into
// the channel list using this special call, which also starts to play the sound
//
Bool	 SoundMgr::EmitSound(UINT SoundID, Bool loop, long ID, long volume, long pan) {
#if	DONT_USE_SOUND
	return FALSE;
#endif
	UINT myChan;
	UINT myVoice;
	long freq = 0;															// unused
	Bool freeChannel = FALSE;
	Bool freeInstance = FALSE;
		if(++CurTime = UINT_MAX) {					 // Increment timestamp and check for overflow
			CurTime = CurTime/2;					 // If overflow, reduce all current timestamps by 1/2
			timefixChannels();
		}


		if (SoundID >= NumSounds)
			DEBUG_SOFT("Bad Sound ID: %d \n", SoundID);

		freeChannel = getFreeChannel(&myChan, pSounds[SoundID].Priority);
		freeInstance = pSounds[SoundID].findFreeInstance(&myVoice);

		if(freeChannel){
			if(freeInstance){
				insertSound(&pSounds[SoundID], myChan, myVoice, pSounds[SoundID].Priority, ID);
			//	*lpChan = myChan;
				Channels[myChan].thisSound->playSound(myVoice, DirectX::sound, loop, volume, freq, pan);			// Play the sucker!
				return TRUE;
			} else {
				myChan = getOldest(&pSounds[SoundID]);
				myVoice = Channels[myChan].InstanceID;
				StopSound(myChan);
				insertSound(&pSounds[SoundID], myChan, myVoice, pSounds[SoundID].Priority, ID);
				Channels[myChan].thisSound->playSound(myVoice, DirectX::sound, loop, volume, freq, pan);				// Play the sucker!
		//		*lpChan = myChan;
			}
			return TRUE;
		}
		else
			return FALSE;
};

// Emitter Extensions
//--------------------------------------------------------------------------


// SoundMgr::InsertEMSound
// This function will install a sound into a special list called the emitter list.
// The emitter list is a list of all the pieces in the world that emit a continuous
// looped sound that gets louder and softer as you move towards and away from that
// piece's location.
//
// InsertEMSound inserts a new emitter in the list, which is then updated and tracked
// by the StepEmitter() function called when the Soundmanager is Stepped. Since the
// emitter extensions are not aware of reference counter pointers (for now), if a piece
// is inserted and then removed or destroyed in the world, RemoveEMSound must be called
// immediately to remove that sound from the list.
//
// Since multiple pieces can share the same sound, a unique ID is assigned each piece
// as it is installed. This is passed back on return from InsertEMSound, and is used to
// remove that emitter from the list when calling RemoveEMSound
//
// Note: Installing an emitter will not play the sound associated with it. The sound will
// be played on the next Step() of the SoundManager.

long	SoundMgr::InsertEMSound(UINT Sound, const PiecePtr &noisypiece){
		emitter *newnode;

		if ((FirstEmitter->Sound == -1))					// New Emitter List?
		{
				FirstEmitter->Sound = Sound;																					// Insert Sound
				FirstEmitter->channel = -1;																					// Not playing yet, so channel is set to null

				FirstEmitter->noisypiece = noisypiece;																

				idTag++;																														// Create new unique ID (will wrap, potential bug)
				FirstEmitter->SoundID	= idTag;																			// Insert Unique ID
				return idTag;
		}
		else																																		// Not a new List
		{
				newnode = new emitter;																							// Create a new node
				LastEmitter->next = newnode;																				// Point Last Emitter.next node to newnode
				newnode->previous = LastEmitter;																		// Point newnode back to last emitter
				LastEmitter = newnode;																							// Update LastEmitter to point to newnode
				LastEmitter->Sound = Sound;																					// Insert Sound
				LastEmitter->channel = -1;																					// Not playing yet, so channel is set to null
																		
				LastEmitter->noisypiece = noisypiece;
				
				idTag++;																														// Create new unique ID (will wrap, potential bug)
				LastEmitter->SoundID	= idTag;																			// Insert Unique ID
				LastEmitter->next = NULL;																						// Terminate the list
				return idTag;
		};
		// endif
};


// SoundMgr::RemoveEMSound -------------------------------------
// Removes an emitter from the emitter list and stops the sound
// if necessary. Must be called when an piece containing an emitter
// is destroyed or removed from the world. Needs the uniqe ID returned
// from InstallEMSound in order to know which emitter to remove.
//
Bool SoundMgr::RemoveEMSound(UINT Sound, long id){
	emitter	*currentEM;

	currentEM = FirstEmitter;

	while (currentEM != NULL) {
		if ((currentEM->Sound == (int)Sound) && (currentEM->SoundID == id))			// Found a match
		{
			if ((currentEM == FirstEmitter) && (FirstEmitter != LastEmitter))						// If found in first node of multiple node list																		// Delete FirstEmitter special case
			{
						StopEmitting(currentEM->SoundID);																			// Stop the sound
						FirstEmitter = FirstEmitter->next;																		// Point the FirstEmitter list entry point to next node
						FirstEmitter->previous = NULL;																				// Nullify previous node pointer to old element
						delete currentEM;																											// Delete the old node
						return TRUE;																													// Exit with "found" flag set to TRUE;
			}
			else if ((currentEM == FirstEmitter) && (FirstEmitter == LastEmitter))			// If found in first node of single node list
			{
						StopEmitting(currentEM->SoundID);																			// Stop the sound
						FirstEmitter->Sound = -1;																							// Set all parms to initial empty list state
						FirstEmitter->SoundID = -1;																					
						FirstEmitter->next = NULL;																						// No need to delete the node, since its the only one
						FirstEmitter->previous = NULL;
						return TRUE;																													// Exit with "found" set to TRUE;
			}
			else if ((currentEM == LastEmitter) && (FirstEmitter != LastEmitter))				// If found as the last node of a multiple node list
			{
						StopEmitting(currentEM->SoundID);																			// Stop the sound
						LastEmitter = currentEM->previous;																		// Point the LastEmitter node to the new last emitter 
						LastEmitter->next = NULL;																							// Terminate the list at the previous node
						delete currentEM;																											// delete the old LastEmitter
						return TRUE;																													// Exit with "found" set to TRUE;
			}
			else																																				// If found as internal node of multiple node list
			{
						StopEmitting(currentEM->SoundID);																			// Stop the sound
						currentEM->previous->next = currentEM->next;													// The element previous to current points to current's next node
						currentEM->next->previous = currentEM->previous;											// The element next to current points back at currents previous node
						delete currentEM;																											// Links should now "skip" over current node, so delete it.
						return TRUE;																													// return "found"
			}	
		} //endif matched
		currentEM = currentEM->next;																								// Not found yet, so check next item
	} // endwhile not end of list
	return FALSE;																																		// Never found it, so return FALSE
}


// SoundMgr::StepEmitter ---------------------------
// step through all emitters and
// - checks to see if it is playing in channels
// - if so, update distance volume
// - if not, retrigger the sound with updated distance volume
// - if still playing, and distance is greater than clip, stop the sound
//
Bool SoundMgr::StepEmitter(void){
	emitter *currentEM;
	uint volume = 0;
	Real distance, pan;
  Vector_2d cameraloc;
	Body	*soundbody, *crixabody;
	Point_2d  camerapoint, soundpoint, mypoint, crixapoint;
	Vector_2d soundvector;

	currentEM = FirstEmitter;

	while ((currentEM != NULL) && (currentEM->Sound > -1)) // while there are emitters to check
	{																													//update volume value
				if(gReferee->Confirm(gLocal->GetCrixa()))
				{
							soundbody = currentEM->noisypiece->GetBody();
							crixabody = gLocal->GetCrixa()->GetBody();

							soundpoint = soundbody->TLocation();
							crixapoint = crixabody->TLocation();
							soundvector = crixapoint - soundpoint;
							distance = abs(soundvector.Norm());


							pan = soundvector[X] * panFactor;
							distance = distance * scaleFactor;



				if (qPlaying(currentEM->SoundID)) 
				{
						if (distance > clipDistance)
								StopEmitting(currentEM->SoundID);
						else
						{
								SetEmitVolume(currentEM->SoundID, (long)-(distance));
								SetEmitPan(currentEM->SoundID, (long) pan);
						}
				}
				else
						if (distance <= clipDistance)
							EmitSound(currentEM->Sound, TRUE, currentEM->SoundID, (long)-(distance), (long) 0);
				}
			currentEM = currentEM->next;

			
	}
	return TRUE;

};


// SoundMgr::StopEmitting -----------------------------------------
// Internal function used to stop an emitter's sound from playing
//
void	SoundMgr::StopEmitting(long ID){
	for(UINT i=0;i<NumChannels;i++){
		if ((Channels[i].thisSound != NULL) && (Channels[i].EmitterId == ID)){
				StopSound(i);
				}
	}
};



// SoungMgr::StopAll -------------------------------------
// Stop all emitters & sounds from playing
void SoundMgr::StopAll(void){
	for(UINT i=0; i<NumChannels;i++)
			StopSound(i);
}

// SoundMgr::SetEmitVolume --------------------------------------
// Internal function used to modify the volume of the currently
// playing sound.
//
void	SoundMgr::SetEmitVolume(long ID, long volume){
	for(UINT i=0;i<NumChannels;i++){
		if ((Channels[i].thisSound != NULL) && (Channels[i].EmitterId == ID))
			Channels[i].thisSound->setVolume(Channels[i].InstanceID, DirectX::sound, volume);
	}
};


// SoundMgr::SetEmitPan ------------------------------------------
// Internal function used to set the panning of an emitter
// panning is from -10000 to 10000, with negative panning being towards
// the right, and positive panning being towards the left.
void SoundMgr::SetEmitPan(long ID, long pan){
	for(UINT i=0;i<NumChannels;i++){
		if ((Channels[i].thisSound != NULL) && (Channels[i].EmitterId == ID))
			Channels[i].thisSound->setPan(Channels[i].InstanceID, DirectX::sound, pan);
	}
};


// SoundMgr::PlayCDTrack
// Opens the CD player device (if not already open) and plays a specified track.
// Takes a looping parameter, which is not currently supported.
//
DWORD	SoundMgr::PlayCDTrack(BYTE track, Bool loop)									// Play a CD audio track, loop defaults to TRUE
{
		DWORD dwReturn;
		MCI_OPEN_PARMS mciOpenParms;																					// Data structure for opening device
		MCI_SET_PARMS  mciSetParms;																						// Data structure for device settings
		MCI_PLAY_PARMS mciPlayParms;																					// Data structure for playback parameters

		
		if (wCDid == NULL) {																									// if the device isn't already open, open it.
			// Open the CD audio device by using device name.

			mciOpenParms.lpstrDeviceType = "cdaudio";
			if (dwReturn = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE, (DWORD)(LPVOID) &mciOpenParms))
			{
					// failed to open device. Don't close it (since we never opened it), just return error.
				wCDid = NULL;
				return (dwReturn);
			}

			// device opened successfully; get the device ID.

			wCDid = mciOpenParms.wDeviceID;																				// wCDid is  contained in the class header file


			// set the tiem format to track/minute/second/frame (TMSF).

			mciSetParms.dwTimeFormat = MCI_FORMAT_TMSF;
			if (dwReturn = mciSendCommand(wCDid, MCI_SET, MCI_SET_TIME_FORMAT, (DWORD)(LPVOID) &mciSetParms))
			{
					mciSendCommand(wCDid, MCI_CLOSE, 0, NULL);
					wCDid = NULL;
					return (dwReturn);																								// error, failed to set time, so close device &return
			}

			// Device opened & Time Set, so play the track!

		}


		mciPlayParms.dwFrom = 0L;
		mciPlayParms.dwTo = 0L;
		mciPlayParms.dwFrom = MCI_MAKE_TMSF(track, 0, 0 , 0);
		mciPlayParms.dwTo = MCI_MAKE_TMSF(track+1, 0, 0, 0);
		mciPlayParms.dwCallback = (DWORD) NULL;																// no callback when track stops

		if (dwReturn = mciSendCommand(wCDid, MCI_PLAY, MCI_FROM | MCI_TO , (DWORD) (LPVOID) &mciPlayParms))
		{
			mciSendCommand(wCDid, MCI_CLOSE, 0, NULL);
			wCDid = NULL;
			return (dwReturn);																										// playback failed, so return
		}
		return (0L);
};




// SoundMgr::StopCD()
// Stops any audio track from playing
// Stop the CD, close the device if open
//
DWORD	SoundMgr::StopCD(void)																									// Stop the CD playback
{

	MCI_GENERIC_PARMS mciStopParms;
	DWORD dwReturn;
	
	if (wCDid)																													// if device is open
		{

			dwReturn = mciSendCommand(wCDid, MCI_STOP, 0, (DWORD) (LPVOID) &mciStopParms);
			return (dwReturn);
		}
		
		return (0L);
};
