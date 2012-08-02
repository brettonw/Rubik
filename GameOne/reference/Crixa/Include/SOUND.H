//======================================================
//	"Sound.h" 
//	4/7/96 
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
//======================================================

#include "direct x.h"
#include "buffer.h"

//------------------------------------------------------

BOOL	DSParseWaveResource(void *pvRes, WAVEFORMATEX **ppWaveHeader, 
							BYTE **ppbWaveData, DWORD *pcbWaveSize);

void ScaleCopy16(LPWORD pdestination, LPWORD psource, int numwords, UINT scale);
//------------------------------------------------------


class Sound {
private:
		UINT	NumInstances,												// Number of simultaneous events of this sound allowed.
				ThisSoundID;													// Our own SoundID, used for reloading WAV if needed.
		cstr	ThisSoundName;											// Name of file which contains sound, used on restore.
		BYTE			*pbWaveData;										// To store the master waveform
		DSBUFFERDESC	dsbdesc;										// Holds the sound buffer description
		Buffer			*SoundBuffer;									// Class that contains and accesses the sound sample file.
	
	struct	instance {													// An instance of a sound holds a pointer to a copy of
		LPDIRECTSOUNDBUFFER	lpDsb;								// the DSBUFFER holding the WAV, and an indication of 
		BOOL		Free;															// if this instance is available for playing.
		BOOL		loop;
	} *Instances;																// This will hold an array of sound instances.

	BOOL restoreSound(UINT SoundID, 
		LPDIRECTSOUNDBUFFER lpDS);								// If DSBUFFERLOST, restore it with this function.



public:
	UINT 	Priority,																					// Priority of the Sound, Higher the better
				VolumeOffset;																			// Default Volume Attentuation for Sound (100ths DB)												

	Sound(void);																						// Constructor, inits dsbdesc.
	~Sound(void);																						// Destructor, default.
	BOOL		playing(UINT InstanceID);												// Checks status of sound, returns true if playing
	LPDIRECTSOUNDBUFFER loadSound(cstr SoundName, 
																LPDIRECTSOUND lpDS, 
																UINT PolyMax, 
																UINT myPriority, 
																UINT Volume);															// Loads a Sound resource and creates a DSBUFFER;
	BOOL		findFreeInstance(UINT *lpInstance);															// Checks Free flag of all instances, returns -1 if none found.
	HRESULT playSound(UINT InstanceID, 
										LPDIRECTSOUND lpDS, 
										BOOL loop, 
										LONG volume, 
										DWORD freq = 0,
										LONG pan = 0);																				// Note:Assumes Sound is loaded;
	void		setVolume(UINT InstanceID, LPDIRECTSOUND lpDS, long volume);		// Setvolume
	void    setPan(UINT InstanceID, LPDIRECTSOUND lpDS, long pan);
	void		stopSound(UINT InstanceID);																			// Stop this particular instance of sound, set its Free = TRUE.

};
	
	