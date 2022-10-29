#pragma once
#ifndef Enemy_3_h
#define Enemy_3_h

#include <SDL.h>
#include "Enemy.h"
#include "Bullet.h"

#define  HEALTH_ENEMY 15


class Enemy_3 : public Enemy
{
private:
	int TimerBullet; // thời gian tồn tại của đạn enemy
public:
	Enemy_3(int level, SDL_RendererFlip flip = SDL_FLIP_NONE); // khởi tạo enemy hoạt động trên màng hình

	void DecReload();
	int GetReload();
	void ResetReload();


	vector<Bullet*> GetBullets(); // lấy danh sách đạn
	void AddBullet(string _BulletTextureID, float x_E, float y_E, int w_E, int h_E, float x_P, float y_P, int w_P, int h_P);
	void DeleteBullet(int i);
	void DoBullet();
	void DrawBullet();
	void SetPosition();
};


#endif // !1
