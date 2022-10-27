#pragma once
#ifndef Animation_h
#define Animation_h

#include <SDL.h>
#include <string>

using namespace std;

class Animation
{
private:
	int m_SpriteRow, m_SpriteFrame;
	int m_AnimSpeed, m_FrameCount;
	string m_TextureID;
	SDL_RendererFlip m_Flip;

public:
	Animation();

	void SetFlip(SDL_RendererFlip flip);;
	SDL_RendererFlip GetFlip();

	void Update();
	void Draw(float x, float y, int spritWidth, int spritHeight, float Angle = 0);
	void SetProps(string textureID, int spritRow, int frameCount, int animSpeed, SDL_RendererFlip m_Flip = SDL_FLIP_NONE);
};

#endif
