#pragma once
#ifndef Enemy_1_h
#define Enemy_1_h

#include "Enemy.h"
#include "Bullet.h"

const float ALIEN_BULLET_SPEED = float(0.6);

class Enemy_1 : public Enemy
{
private:
	
public:
	Enemy_1(int level, SDL_RendererFlip flip = SDL_FLIP_NONE); // khởi tạo enemy hoạt động trên màng hình

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
