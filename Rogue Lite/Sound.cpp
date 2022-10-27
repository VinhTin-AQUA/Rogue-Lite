#include "Sound.h"

// khởi tạo lưu trữ âm thanh // tải âm thanh WAV
Sound::Sound()
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	music = nullptr;

	sounds[SND_PLAYER_FIRE] = Mix_LoadWAV("sounds\\player_fire.wav");
	sounds[SND_ALIEN_FIRE] = Mix_LoadWAV("sounds\\alien_fire.wav");
	sounds[SND_PLAYER_DIE] = Mix_LoadWAV("sounds\\player_die.wav");
	sounds[SND_ALIEN_DIE] = Mix_LoadWAV("sounds\\alien_die.wav");
	sounds[SND_POINTS] = Mix_LoadWAV("sounds\\reward.wav");
}
Sound::~Sound()
{
	Mix_CloseAudio();
}

// tải âm nhạc
void Sound::loadMusic(char* filename) // chua dung den
{
	if (music != NULL)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = NULL;
	}

	music = Mix_LoadMUS(filename);
}

// phát nhạc
void Sound::playMusic(int loop) // chua dung den
{
	Mix_PlayMusic(music, (loop) ? -1 : 0);
}

// phát âm thanh
void Sound::playSound(int channel, int id)
{
	Mix_PlayChannel(channel, sounds[id], 0);
}