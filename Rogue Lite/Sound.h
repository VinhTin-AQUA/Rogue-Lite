#pragma once
#ifndef Sound_h
#define Sound_h

#include <iostream>
#include <SDL.h>
#include <SDL_mixer.h>
const int MAX_SND_CHANNELS = 8; // số lượng hiệu ứng âm thanh có thể phát cùng một lúc
const int MAX_MUSICS = 8;

using namespace std;

// chỉ định kênh mà âm thanh sẽ phát qua đó,
enum // CHANEL
{
	CH_ANY = -1,
	CH_PLAYER,
	CH_ALIEN_FIRE,
	CH_POINTS
};

// xác định hiệu ứng âm thanh
enum
{
	SND_PLAYER_FIRE,
	SND_ALIEN_FIRE,
	SND_PLAYER_DIE,
	SND_ALIEN_DIE,
	SND_POINTS,
	SND_MAX,
};

// loại music
enum
{
	ENTER_THE_GAME,
	THE_END,
	THROUGH_LEVEL
};

class Sound
{
private:
	Mix_Chunk* sounds[MAX_SND_CHANNELS];
	Mix_Music* musics[MAX_MUSICS];
public:
	Sound();
	~Sound();
	// phát nhạc
	void playMusic(int musicName, int loop); // chua dung den
	// phát âm thanh
	void playSound(int channel, int id);
};

#endif