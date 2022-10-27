#include "HitFire.h"

HitFire* HitFire::Instance = nullptr;
HitFire* HitFire::GetInstance()
{
	return Instance = (Instance != nullptr) ? Instance : new HitFire();
}
// kiểm tra va chạm giữa đạn và enemies
vector<int> HitFire::BulletHitEnemies(vector<Bullet*> bullets, vector<Enemy*> enemies) 
{
	vector<int> temp;
	for (unsigned int i = 0; i < bullets.size(); i++) // duyệt danh sách đạn
	{
		for (unsigned int j = 0; j < enemies.size(); j++) // duyệt danh sách enemies
		{
			// nếu có va chạm giữa dạn và enemies thì trả về chỉ số của enemy đó
			if (CollisionHandler::GetInstance()->CheckCollision(bullets[i]->GetBox(), enemies[j]->GetBox()) == true)
			{
				temp.push_back(i); // trả về enemiy có đan
				temp.push_back(j);
				return temp;
			}
		}
	}
	return temp;
}

int HitFire::BulletHitPlayer(vector<Bullet*> bullets, Player* player)
{
	for (unsigned int i = 0; i < bullets.size(); i++) // duyệt danh sách đạn
	{
		if (CollisionHandler::GetInstance()->CheckCollision(bullets[i]->GetBox(), player->GetBox()) == true)
		{
			return i;
		}
	}

	return -1;
}
