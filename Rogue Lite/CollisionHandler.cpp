#include "CollisionHandler.h"

#include "Bullet.h"

CollisionHandler* CollisionHandler::s_Instance = nullptr;

CollisionHandler::CollisionHandler()
{
	// lấy lớp đẩu tiên của map (.front() => lớp đầu) // back()
	m_CollisionLayer = (TileLayer*)Engine::GetInstance()->GetMap()->GetMapLayer().back();
	m_CollisionTilemap = m_CollisionLayer->GetTilemap();
}

bool CollisionHandler::CheckCollision(SDL_Rect a, SDL_Rect b) // kiểm tra va chạm giữa 2 hình vuông
{
	return  SDL_HasIntersection(&a, &b);
}
bool CollisionHandler::MapCollision(SDL_Rect a)
{
	int tilesize = 32;
	int rowcount = 30;
	int colcount = 60;

	int left_tile = a.x / tilesize;
	int right_tile = (a.x + a.w) / tilesize;

	int top_tile = a.y / tilesize;
	int bottom_tile = (a.y + a.h) / tilesize;

	if (left_tile < 0) left_tile = 0;
	if (right_tile > colcount) right_tile = colcount;

	if (top_tile < 0) top_tile = 0;
	if (bottom_tile > rowcount) bottom_tile = rowcount;

	for (int i = left_tile; i <= right_tile; ++i)
	{
		for (int j = top_tile; j <= bottom_tile; ++j)
		{
			if (m_CollisionTilemap[j][i] > 0)
			{
				// collision
				return true;
			}
		}
	}
	// do not collision
	return false;
}

