/****************************************************************************
 * libwiigui Template
 * Tantric 2009
 *
 * audio.cpp
 * Audio support
 ***************************************************************************/

#include <gccore.h>
#include <ogcsys.h>
#include <asndlib.h>

/****************************************************************************
 * init_audio
 *
 * Initializes the Wii's audio subsystem
 ***************************************************************************/
void init_audio()
{
	AUDIO_Init(NULL);
	ASND_Init();
	ASND_Pause(0);
}

/****************************************************************************
 * ShutdownAudio
 *
 * Shuts down audio subsystem. Useful to avoid unpleasant sounds if a
 * crash occurs during shutdown.
 ***************************************************************************/
void ShutdownAudio()
{
	ASND_Pause(1);
	ASND_End();
}
