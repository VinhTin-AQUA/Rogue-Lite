#pragma once
#ifndef CollisionHandler_h
#define CollisionHandler_h

#include <iostream>
#include <vector>
#include <SDL.h>
#include "TileLayer.h"
#include "GameMap.h"
#include "Engine.h"
#include "Enemy.h"
using namespace std;

class CollisionHandler
{
private:
	TileMap m_CollisionTilemap;
	TileLayer* m_CollisionLayer;
	static CollisionHandler* s_Instance;

public:
	CollisionHandler();
	inline static CollisionHandler* GetInstance()
	{
		return s_Instance = (s_Instance != nullptr) ? s_Instance : new CollisionHandler;
	}

	bool CheckCollision(SDL_Rect a, SDL_Rect b); // kiểm tra va chạm giữa 2 hình vuông
	bool MapCollision(SDL_Rect a); // kiểm tra va chạm giữa 1 hình vuông và map

};

#endif
