#include "Explosion.h"

#include "TextureManager.h"
#include <iostream>

Explosion::Explosion(string id, float _x, float _y, int _width, int _height, int frameCount, int _a)
{
	texture = id;
	r = 0; g = 0; b = 0; // màu sắc của hoạt ảnh
	a = _a;  // thời gian hoạt ảnh tồn tại
	
	// tọa độ hoạt ảnh được vec tại enemy vừa bị tiêu diệt
	x = _x;
	y = _y;

	// chọn màu ngẫu nhiên cho hoạt ảnh
	srand((int)time(0));
	switch (rand() % 4)
	{
	case 0:
		r = 255;
		break;

	case 1:
		r = 255;
		g = 128;
		break;

	case 2:
		r = 255;
		g = 255;
		break;

	default:
		r = 255;
		g = 255;
		b = 255;
		break;
	}

	// kích thước của hoạt ảnh
	width = _width; height = _height;

	// khởi tạo hoạt ảnh
	_Animation = new Animation;
	_Animation->SetProps(id, 0, frameCount, 90);
}
Explosion::~Explosion()
{
	delete _Animation;
}
float Explosion::GetX()
{
	return x;
}
float Explosion::GetY()
{
	return y;
}
int Explosion::GetR()
{
	return r;
}
int Explosion::GetG()
{
	return g;
}
int Explosion::GetB()
{
	return b;
}
int Explosion::GetA()
{
	return a;
}

int Explosion::CheckA()
{
	return --a;
}

void Explosion::Draw(bool check)
{
	SDL_SetRenderDrawBlendMode(Engine::GetInstance()->GetRenderer(), SDL_BLENDMODE_ADD);
	SDL_SetTextureBlendMode(TextureManager::GetInstance()->GetTexture(texture), SDL_BLENDMODE_ADD);
	
	if (check == true)
	{
		SDL_SetTextureColorMod(TextureManager::GetInstance()->GetTexture(texture), r, g, b);
		
	}
	else
	{
		SDL_SetTextureColorMod(TextureManager::GetInstance()->GetTexture(texture), 255, 255, 255);
		
	}
	SDL_SetTextureAlphaMod(TextureManager::GetInstance()->GetTexture(texture), (Uint8)1000);

	_Animation->Draw(x, y, width, height);

	SDL_SetRenderDrawBlendMode(Engine::GetInstance()->GetRenderer(), SDL_BLENDMODE_NONE);
}

void Explosion::Update()
{
	_Animation->Update();
}