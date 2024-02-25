/****************************************************************************
 * libwiigui
 *
 * Tantric 2009
 *
 * gui_sound.cpp
 *
 * GUI class definitions
 ***************************************************************************/

#include "../gui.h"

/**
 * Constructor for the gui_sound class.
 */
gui_sound::gui_sound(const u8 * s, s32 l, int t)
{
	sound = s;
	length = l;
	type = t;
	voice = -1;
	volume = 100;
	loop = false;
	start_pos = 0;
}

/**
 * Destructor for the gui_sound class.
 */
gui_sound::~gui_sound()
{
	#ifndef NO_SOUND
	if(type == SOUND_OGG)
		StopOgg();
	#endif
}

void gui_sound::Play()
{
	#ifndef NO_SOUND
	int vol;

	switch(type)
	{
		case SOUND_PCM:
		vol = 255*(volume/100.0);
		voice = ASND_GetFirstUnusedVoice();
		if(voice >= 0)
			ASND_SetVoice(voice, VOICE_STEREO_16BIT, 48000, 0,
				(u8 *)sound, length, vol, vol, NULL);
		break;

		case SOUND_OGG:
		voice = 0;
		if(loop) PlayOgg((char *)sound, length, start_pos, OGG_INFINITE_TIME);
		else PlayOgg((char *)sound, length, start_pos, OGG_ONE_TIME);
		SetVolumeOgg(255*(volume/100.0));
		break;
	}
	#endif
}

void gui_sound::Stop()
{
	#ifndef NO_SOUND
	if(voice < 0)
		return;

	switch(type)
	{
		case SOUND_PCM:
		ASND_StopVoice(voice);
		break;

		case SOUND_OGG:
		StopOgg();
		break;
	}
	#endif
}

void gui_sound::Pause()
{
	#ifndef NO_SOUND
	if(voice < 0)
		return;

	switch(type)
	{
		case SOUND_PCM:
		ASND_PauseVoice(voice, 1);
		break;

		case SOUND_OGG:
		PauseOgg(1);
		break;
	}
	#endif
}

void gui_sound::Resume()
{
	#ifndef NO_SOUND
	if(voice < 0)
		return;

	switch(type)
	{
		case SOUND_PCM:
		ASND_PauseVoice(voice, 0);
		break;

		case SOUND_OGG:
		PauseOgg(0);
		break;
	}
	#endif
}

bool gui_sound::IsPlaying()
{
	if(ASND_StatusVoice(voice) == SND_WORKING || ASND_StatusVoice(voice) == SND_WAITING)
		return true;
	else
		return false;
}

void gui_sound::SetVolume(int vol)
{
	#ifndef NO_SOUND
	volume = vol;

	if(voice < 0)
		return;

	int newvol = 255*(volume/100.0);

	switch(type)
	{
		case SOUND_PCM:
		ASND_ChangeVolumeVoice(voice, newvol, newvol);
		break;

		case SOUND_OGG:
		SetVolumeOgg(255*(volume/100.0));
		break;
	}
	#endif
}

void gui_sound::SetLoop(bool l)
{
	loop = l;
}

void gui_sound::SetStartPos(int pos)
{
	start_pos = pos;
}