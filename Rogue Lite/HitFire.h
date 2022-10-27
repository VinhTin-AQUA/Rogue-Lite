#pragma once
#ifndef HitFire_h
#define HitFire_h

#include <vector>
#include "Player.h"
#include "Enemies.h"
#include "CollisionHandler.h"

class HitFire
{
private:
	static HitFire* Instance;
public:
	static HitFire* GetInstance(); // lấy biến tĩnh

	// kiểm tra va chạm giữa đạn của player và enemies
	vector<int> BulletHitEnemies(vector<Bullet*> bullets, vector<Enemy*> enemies); 

	// kiểm tra va chạm giữa đạn enemies và player
	int BulletHitPlayer(vector<Bullet*> bullets, Player* player);
};

#endif
