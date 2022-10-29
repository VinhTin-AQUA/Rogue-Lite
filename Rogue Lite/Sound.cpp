#include "Sound.h"

// khởi tạo lưu trữ âm thanh // tải âm thanh WAV
Sound::Sound()
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	sounds[SND_PLAYER_FIRE] = Mix_LoadWAV("sounds\\player_fire.wav");
	sounds[SND_ALIEN_FIRE] = Mix_LoadWAV("sounds\\alien_fire.wav");
	sounds[SND_PLAYER_DIE] = Mix_LoadWAV("sounds\\player_die.wav");
	sounds[SND_ALIEN_DIE] = Mix_LoadWAV("sounds\\alien_die.wav");
	sounds[SND_POINTS] = Mix_LoadWAV("sounds\\reward.wav");

	// load music

	musics[0] = Mix_LoadMUS("Sounds\\enter_the_game.mp3");
	musics[1] = Mix_LoadMUS("Sounds\\the_end.mp3");
	musics[2] = Mix_LoadMUS("Sounds\\through_the_level.mp3");

}
Sound::~Sound()
{
	Mix_CloseAudio();
}

// tải âm nhạc


// phát nhạc
void Sound::playMusic(int musicName, int loop) // chua dung den
{
	Mix_PlayMusic(musics[musicName], loop);
}

// phát âm thanh
void Sound::playSound(int channel, int id)
{
	Mix_PlayChannel(channel, sounds[id], 0);
}