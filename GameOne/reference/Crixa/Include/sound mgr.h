//------------------------------------------------------------------------------
//	"SoundManager.h" 
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
// 02/27/97	MK	Added PlayCDTrack & StopCD calls, plus wCDid variable to hold device id of CD player
//
//------------------------------------------------------------------------------

#include "sound.h"
#include "point.h"
#include "vector.h"
#include "piece ptr.h"


//------------------------------------------------------------------------------
class SoundMgr
{
	private:
				// CD Playback variables
				UINT					wCDid;																								// MCI device ID for CD player

				// Emitter Extensions Private Data

				int						NumEmitters;
				long					idTag;																																														// Used to generate Unique IDs for emitters
				struct				emitter
				{
						int				Sound;																																														// Index location of sound in pSounds array
						long			SoundID;																																													// Unique SoundID for this playing sound 
						int				channel;																																													// Channel currently used by this emitter
						PiecePtr	noisypiece;																																												// Pointer to piece of sound's emitter/owner
						emitter		*previous,																																												// Pointer to previous entry in emitter linked list
											*next;																																														// Pointer to next entry in emitter linked list
				}							*FirstEmitter, *LastEmitter;																																			// Pointer to first emitter entry
				// End Emitter Extensions Private Data

				UINT					NumChannels, 																																											// Number of simultaneous channels to allocate.
											NumSounds,																																												// Total number of unique sounds to manage.
											NumLoaded,																																												// Number of currently loaded sounds.Also
																																																												// points to next free spot in array to load a sound.
											CurChannel,																																												// Gereral index to currently manipulated channel.
											CurTime, 																																													// Current call to SndMgr to indicate time.
											rOffset;																																													// Used for resource loading
				Real					clipDistance,																																											// Maximum distance sound can be heard before clipped
											scaleFactor,																																											// Scale Factor to modify distance to use as attenuation magnitude
											panFactor;																																												// Scale Factor to modify panning to use as L/R attenuation magnitude
				struct	channel
				{																																																								// Channels hold the current sound being played,
					Sound					*thisSound;																																											// as well as which instance of that sound and 
					UINT					InstanceID;																																											// the time it was played (expressed as # of calls
					UINT					Priority;																																												// Priority level of the sound playing
					UINT					TimeStamp;																																											// made to the SoundManager since it was created.)
					long					EmitterId;																																											// Special ID for Emitter extensions
				}							*Channels;																																												// Will form array of available channels.
			
				Sound					*pSounds;																																													// Will form array of available sound objects.
				void					timefixChannels(void);																																						// Halves all timestamps to handle time overflow.
				void					insertSound(Sound *pSound, 
																	UINT myChan, 
																	UINT myVoice, 
																	UINT priority, 
																	long ID = -1);								// Inserts a sound & its info into a given channel.
				
				UINT					getLowest(UINT newPriority);																																			// Returns the oldest lowest priority sound.
				UINT					getOldest(Sound *pNewSound);																																			// Returns oldest playing instance of a given sound
				Bool					getFreeChannel(UINT *lpMyChan, UINT thisPriority);																								// Returns a free channel, or frees a lower priority channel
				void					updateChannels(void);																																							// Scans all channels, looking for dead sounds.
				Bool					qPlaying(long ID);																																								// Check for emitter sounds and return true if found & playing
				
			  Bool					SMPlaySound(UINT SoundID, 
																	Bool loop = FALSE, 
																	long volume = 0, 
																	long freq = 0,
																	long pan = 0);											// Internal PlaySound. All Public Playsound calls are mapped to this
				
				Bool					EmitSound(UINT SoundID, 
																Bool loop, 
																long ID, 
																long volume = 0,
																long pan = 0);																			// Special playsound for emitters only, takes a unique ID for the sound 
				
				void					StopEmitting(long ID);																																						// Special StopSound() for emitters
				void					SetEmitVolume(long ID, long volume);																															// Set volume for particular emitter
				void					SetEmitPan(long ID, long pan);
				Bool					StepEmitter(void);																																								// Step the emitter, scan for stopped sounds, update volumes
				Bool					LoadSound(cstr name, 
																UINT maxpoly, 
																UINT priority, 
																UINT volume);																		// loads a sound of a given name
				
public:
				SoundMgr (void);																																																// Constructor
				~SoundMgr (void);																																																// Destructor
				void					InitSound(UINT numChans, UINT numSounds);																													// Starts DirectSound, Checks Hardware.
				Bool					PlaySound (uint id, 
																Bool loop = FALSE, 
																long volume = 0, 
																long freq = 0);																								// Non-Attenuated Sound Trigger Call
			
				Bool					PlaySound (uint SoundID, 
																const PiecePtr &noisypiece, 
																Bool loop = FALSE, 
																long attenuation = 0, 
																long freq = 0 );		// Attenuated by Distance Sound Trigger Call
			
				void					StopSound(UINT ThisChannel);																																			// Stops a particular channel's sound.
				void					Step(void);																																												// Steps the Soundmanager, and calls StepEmitter				

				void					StopAll(void);																						// Stop all sounds
				// Begin Emitter Public Extensions
				long					InsertEMSound(UINT Sound, const PiecePtr &noisypiece);																										// Insert a new sound emitter into emitter array
				Bool					RemoveEMSound(UINT Sound, long id);																																// Remove an emitter's sound
				
				// End Emitter Public Extensions

				// CD-AUDIO Playback commands
				DWORD				  PlayCDTrack(BYTE track, Bool loop = TRUE);									// Play a CD audio track
				DWORD					StopCD(void);																							// Stop the CD playback

};

//------------------------------------------------------------------------------
extern	SoundMgr	*gSoundMgr;																										//	global sound manager

//------------------------------------------------------------------------------
	
	
		

