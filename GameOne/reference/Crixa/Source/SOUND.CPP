//------------------------------------------------------------------------------
//	"Sound.cpp" 
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
//
//	Modified:  Mark E. Kern
//						 02/12/97								Cleared Soundbuffer from memory, removed restore soundbuffer fucntionality
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "sound.h"

//-----------------------------------------------------

LPDIRECTSOUNDBUFFER	lpDsb;




// ScaleCopy16 ----------------------------------------
// ScaleCopy16 is a modified memory copy routine that scales
// everything it copies by dividing it by the scalefactor.
void ScaleCopy16(LPWORD pdestination, LPWORD psource, int numwords, UINT scale){

	for(int i = 0; i < numwords; i++){
		*pdestination = *psource;
		++pdestination;
		++psource;
	}

};



// DSParseWaveResource --------------------------------
// This is a quick and dirty way of parsing the resource
// information, which is stored in Microsoft's Multimedia
// RIFF format. The format is basically a tagged data
// structure, where unknown tags can be ignored and skipped.
// This routine is not mine, but lifted from the MS demo code.
// Ultimately, we'd probably include our own RIFF tag for
// storing the sound's priority information and polyphony.
BOOL	DSParseWaveResource(void *pvRes, WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData, DWORD *pcbWaveSize)
{
	DWORD *pdw;
	DWORD *pdwEnd;
	DWORD dwRiff;
	DWORD dwType;
	DWORD dwLength;

	if (ppWaveHeader)
		*ppWaveHeader = NULL;

	if (ppbWaveData)
		*ppbWaveData = NULL;

	if (pcbWaveSize)
		*pcbWaveSize = 0;

	pdw = (DWORD *)pvRes;
	dwRiff = *pdw++;
	dwLength = *pdw++;
	dwType = *pdw++;

	if(dwRiff != mmioFOURCC('R', 'I', 'F', 'F'))
		goto exit;

	if(dwType != mmioFOURCC('W', 'A', 'V', 'E'))
		goto exit;

	pdwEnd = (DWORD *)((BYTE *)pdw + dwLength-4);

	while (pdw < pdwEnd)
	{
		dwType = *pdw++;
		dwLength = *pdw++;

		switch (dwType)
		{
		case mmioFOURCC('f','m','t',' '):
			if (ppWaveHeader && !*ppWaveHeader)
			{
				if (dwLength < sizeof(WAVEFORMAT))
					goto exit;

				*ppWaveHeader = (WAVEFORMATEX *)pdw;

				if ((!ppbWaveData || *ppbWaveData) &&
					(!pcbWaveSize || *pcbWaveSize))
				{
					return TRUE;
				}
			}
			break;

		case mmioFOURCC('d','a','t','a'):
			if ((ppbWaveData && !*ppbWaveData) || (pcbWaveSize && !*pcbWaveSize))
			{
				if (ppbWaveData)
					*ppbWaveData = (LPBYTE)pdw;

				if (pcbWaveSize)
					*pcbWaveSize = dwLength;

				if (!ppWaveHeader || *ppWaveHeader)
					return TRUE;
			}
			break;
		}
		pdw = (DWORD *)((BYTE *)pdw + ((dwLength+1)&~1));
	}

exit:
	return FALSE;
};

//--------------------------------------------------------------


// Sound::Sound -----------------------------------------------
Sound::Sound(void){
	memset(&dsbdesc,0, sizeof(dsbdesc));							// Clear out dsbdesc
	NumInstances = 0;												// No sounds allocated yet
};

// Sound::~Sound -----------------------------------------
Sound::~Sound(void){
};


// Sound::playing ---------------------------------------------
// Checks the status of the instance to see if its playing.
// Returns true if the sound is playing, otherwise false.
//
// Assumptions:
//	InstanceID points to a valid sound buffer;
BOOL Sound::playing(UINT InstanceID){
	if (Instances[InstanceID].loop)
		return TRUE;
	else
	{
		DWORD	status;
		Instances[InstanceID].lpDsb->GetStatus(&status);
		if (status == DSBSTATUS_PLAYING)
			return TRUE;
		else
			return FALSE;
	}
};


//--------------------------------------------------------------
// Sound::restoreSound -----REMOVED< REMOVED, NOT FUNCTIONAL IN THIS VERSION
// Takes a sound buffer that has been lost, restores it, then
// rewrites the wavedata to it. Works for any instance.
//
// Notes:
//	A buffer becomes lost because some other application grabbed
//  hardware memory on the soundcard. This means you have to keep
//  a copy of the WAV in system RAM in order to reload it into
//  the hardware buffer. It would take too long to load it from 
//  disk again. Unfortunately, this means you can greatly increase
//  the storage needed for a sound, in some cases (if there are no
//  hardware buffers on the card for example), doubling your mem reqs for sound.
//  There are better ways to handle this, perhaps keeping a compressed
//  version in system memory for restoring at a later time to the
//  lost soundbuffers. This will do for now.
//
//	It is unknown if buffers can be lost if they are allocated only
//  in system RAM. If not, then we can save memory by not checking
//  for lost buffers or restoring them on computers with lame 
//  Soundcards (i.e. no hardware mixing RAM). 
//
// Assumptions:
//	SoundID is not needed for this version, but is reserved.

BOOL Sound::restoreSound(UINT SoundID, LPDIRECTSOUNDBUFFER lpDsb) 
{
/*	HRESULT			hr;
	LPVOID			lpvPtr1, lpvPtr2;
	DWORD			dwBytes1, dwBytes2;

	hr = lpDsb->Restore();

	if(DS_OK==hr) {
		hr = lpDsb->Lock(0, dsbdesc.dwBufferBytes, &lpvPtr1, 
			&dwBytes1, &lpvPtr2, &dwBytes2,0);
		if (DSERR_BUFFERLOST == hr) {
			lpDsb->Restore();
			hr = lpDsb->Lock(0, dsbdesc.dwBufferBytes, 
				&lpvPtr1, &dwBytes1, &lpvPtr2, &dwBytes2,0);
		}
		CopyMemory(lpvPtr1, pbWaveData, dwBytes1);
		// ScaleCopy16((LPWORD)lpvPtr1, (LPWORD)pbWaveData, dwBytes1/2, ScaleFactor);

		if( dwBytes2 != NULL)
			CopyMemory(lpvPtr2, pbWaveData+dwBytes1, dwBytes2);
			  //ScaleCopy16((LPWORD)lpvPtr1, (LPWORD)pbWaveData+dwBytes1, dwBytes2/2, ScaleFactor);
		lpDsb->Unlock(lpvPtr1, dwBytes1, lpvPtr2, dwBytes2);
		return TRUE;
	}
	else
		return FALSE;*/
	return FALSE;
	
}; 


// Sound::loadSound --------------------------------------------
// Takes a sound resource ID and the pointer to the direct
// sound object. This routine loads a resource file, parses it
// for WAV data, and creates a DirectSound buffer to hold it all.
// Then, it creates an array of duplicated sound buffers to contain
// multiple instances of the WAV. The number or instances is set to
// the PolyMax parameter passed to this function. Ultimately, this
// value will be loaded from a seperate settings file. Also down the road,
// this routine may be modified to handle loading
// sounds from one large data file, or from compressed resources.
// Note that loadSound returns NULL if any part should fail.
// 
// NOTE: There is a problem here, in that the resource is never
// freed once loaded. This is fine, but hogs memory. We could unlock
// it, but then we'd have to store the Waveformat information in our
// sound object, instead of just pointing to where that info is in
// the resource, like we do now (not me! MS Code!). Works for now.
//
// Assumptions:
// lpDS is valid (not NULL), SoundID is valid resource number, PolyMax > 0
LPDIRECTSOUNDBUFFER Sound::loadSound(cstr SoundName, LPDIRECTSOUND lpDS, UINT PolyMax, UINT myPriority, UINT volume)
{
	HRESULT			hr;
	LPVOID			lpvPtr1, lpvPtr2;
	DWORD			dwBytes1, dwBytes2;
//	HRSRC			hResInfo;
//	HGLOBAL			hResData;
	void			*pvRes;
/*	if (((hResInfo = FindResource(NULL, MAKEINTRESOURCE(SoundID), "WAVE")) != NULL) &&
		((hResData = LoadResource(NULL, hResInfo)) != NULL) &&
		((pvRes = LockResource(hResData)) != NULL) &&
		DSParseWaveResource(pvRes, &dsbdesc.lpwfxFormat, 
			&pbWaveData, &dsbdesc.dwBufferBytes)) { 
			*/

	char	filename[64];
	sprintf (filename, "Sound\\%s", SoundName);
	SoundBuffer = new Buffer(filename);
	pvRes = SoundBuffer->CurrentPtr();
//	ScaleFactor = Scale;

	if (DSParseWaveResource(pvRes, &dsbdesc.lpwfxFormat, 
			&pbWaveData, &dsbdesc.dwBufferBytes)){
			
		dsbdesc.dwSize = sizeof(dsbdesc);
		dsbdesc.dwFlags = DSBCAPS_LOCSOFTWARE;
	//	dsbdesc.dwFlags = DSBCAPS_CTRLDEFAULT;


		hr = lpDS->CreateSoundBuffer(&dsbdesc, &lpDsb, NULL);



		if(DS_OK==hr) {
			hr = lpDsb->Lock(0, dsbdesc.dwBufferBytes, &lpvPtr1, 
				&dwBytes1, &lpvPtr2, &dwBytes2,0);
			if (DSERR_BUFFERLOST == hr) {
				lpDsb->Restore();
				hr = lpDsb->Lock(0, dsbdesc.dwBufferBytes, 
					&lpvPtr1, &dwBytes1, &lpvPtr2, &dwBytes2,0);
			}
			CopyMemory(lpvPtr1, pbWaveData, dwBytes1);
//			ScaleCopy16((LPWORD)lpvPtr1, (LPWORD)pbWaveData, dwBytes1/2, ScaleFactor);

			if( dwBytes2 != NULL)
				 CopyMemory(lpvPtr2, pbWaveData+dwBytes1, dwBytes2);
				 // ScaleCopy16((LPWORD)lpvPtr2, (LPWORD)pbWaveData+dwBytes1, dwBytes2/2, ScaleFactor);
			lpDsb->Unlock(lpvPtr1, dwBytes1, lpvPtr2, dwBytes2);
		}

		// delete the buffer
		delete SoundBuffer;

		NumInstances = PolyMax;
		Instances = new instance[NumInstances];
		Instances[0].Free = TRUE;
		Instances[0].lpDsb = lpDsb;


		for(UINT i=1; i < NumInstances; i++){
			hr = lpDS->DuplicateSoundBuffer(lpDsb,&Instances[i].lpDsb);

		//	switch(hr) {
		//	case DSERR_ALLOCATED:
		//		break;
		//	case DSERR_INVALIDCALL:
		//		break;
		//	case DSERR_INVALIDPARAM:
		//		break;
		//	case DSERR_OUTOFMEMORY:
		//		break;
		//	default:
		//		_ASSERTE(hr == DS_OK);
		//	} 

			Instances[i].Free = TRUE;		
		}


	//	ThisSoundID = SoundID;
		ThisSoundName = SoundName;
		Priority = myPriority;
		VolumeOffset = volume;
		
		return lpDsb;
	} 
	else
		return NULL;
}; 

// findFreeInstance ----------------------------------------
// All this routine does is to loop through the instances 
// and return the first instance that is free. If no 
// instances are free, this function returns -1;
//
// Assumptions:
//   At least one Instance actually exist, that sound is loaded.
BOOL Sound::findFreeInstance(UINT *lpInstance){
	for(UINT i=0; i < NumInstances; i++){
		if(Instances[i].Free == TRUE) {
				*lpInstance = i;
				return TRUE;
		}
	}
	return FALSE;
};


// playSound -----------------------------------------------
// This routine is used by the Soundmanager object, and should
// not be directly called by the programmer. Essentially, this
// routine takes an instance number and a pointer to a 
// DirectSound object (for use in reloading sound if needed).
// It then attempts to play the sound, checking to see if the
// buffer has been lost. If the buffer has been lost, this
// routine will call restoreSound in an attempt to fix it.
// Playsound then marks the Instance FREE flag as FALSE. The
// sound class does not check on a timer or interrupt basis 
// on whether this sound has stopped playing. An explicit
// stopSound command must be executed to free the instance,
// whether or not the sound has actually stopped playing.
// This is because it is simpler to have the SoundManager class
// to check each channel's playing status, and to issue automatic
// stopSound commands to those sounds which have stopped playing.
//
// Assumptions:
//	InstanceID is valid, lpDS not used, reserved for future use.
//
HRESULT Sound::playSound(UINT InstanceID, LPDIRECTSOUND lpDS, BOOL loop, LONG volume, DWORD freq, LONG pan)
{
	HRESULT hr;

	
	hr = Instances[InstanceID].lpDsb->SetVolume(volume + VolumeOffset);
	if(freq != 0)
		hr = Instances[InstanceID].lpDsb->SetFrequency(freq);
	if(pan != 0)
		hr = Instances[InstanceID].lpDsb->SetPan(pan);

	if (loop) 
		hr = Instances[InstanceID].lpDsb->Play(NULL, NULL, DSBPLAY_LOOPING);
	else
		hr = Instances[InstanceID].lpDsb->Play(NULL, NULL, NULL);


	if(DSERR_BUFFERLOST == hr)
		if(restoreSound(ThisSoundID, Instances[InstanceID].lpDsb))
			if (loop) 
				hr = Instances[InstanceID].lpDsb->Play(NULL, NULL, DSBPLAY_LOOPING);
			else
				hr = Instances[InstanceID].lpDsb->Play(NULL, NULL, NULL);
	Instances[InstanceID].Free = FALSE;
	Instances[InstanceID].loop = loop;
	return hr;
};


// stopSound ------------------------------------------------
// This routine stops the sound instance. It doesn't care if
// the sound is actually playing, it attempts to stop it anyway,
// and then sets the Free flag to TRUE.
//
// Assumptions:
//   That the free flag is actually false,
//	 that the intanceID is valid.
void Sound::stopSound(UINT InstanceID)
{
	HRESULT hr;
	hr = Instances[InstanceID].lpDsb->Stop();
	hr = Instances[InstanceID].lpDsb->SetCurrentPosition(0);			// Needed to reset buffer offset pointer
	hr = Instances[InstanceID].lpDsb->SetVolume(0);								// Reset volume to original
	Instances[InstanceID].Free = TRUE;
	Instances[InstanceID].loop = FALSE;
};



// setVolume ------------------------------------------------
// Wrapper to set volume
//
void		Sound::setVolume(UINT InstanceID, LPDIRECTSOUND lpDS, long volume){
	HRESULT hr;
	hr = Instances[InstanceID].lpDsb->SetVolume(volume + VolumeOffset);
};


// setPan
// Wrapper to Direct Sound pan
//
void    Sound::setPan(UINT InstanceID, LPDIRECTSOUND lpDS, long pan){
	HRESULT hr;
	hr = Instances[InstanceID].lpDsb->SetPan(pan);
};