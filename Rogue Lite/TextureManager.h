#pragma once
#ifndef TextureManager_h
#define TextureManager_h

#include <string>
#include <map>
#include <SDL.h>
#include <SDL_image.h>
#include "Engine.h"
#include <string>
using namespace std;

class TextureManager
{
private:
	static TextureManager* s_Instance;
	map<string, SDL_Texture*> m_TextureMap; // danh sách lưu trữ kết cấu

public:
	static TextureManager* GetInstance()
	{
		return s_Instance = (s_Instance != nullptr) ? s_Instance : new TextureManager();
	}
	bool Load(string id, string filename); // tải 1 kết cấu
	SDL_Texture* GetTexture(string id); // lấy kết theo id
	void Drop(string id); // xóa 1 kết cấu khỏi danh sách
	void Clean(); // giải phóng toàn bộ kết cấu

	void Draw(string id, float x, float y, int width, int height, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void DrawFrame(string id, float x, float y, int width, int height, int row, int frame, SDL_RendererFlip flip = SDL_FLIP_NONE, float Angle = 0);
	void DrawTile(string tilesetID, int tileSize, float x, float y, int row, int frame, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void DrawEntity(SDL_Texture* texture, float x, float y);
};

#endif