#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <cmath>

#include "Enemies.h"
#include "Enemy_1.h"
#include "Enemy_2.h"
#include "Enemy_3.h"
#include "CollisionHandler.h"


Enemies::Enemies() // khoi tao enemies
{
	// khởi tạo enemeies
	for (unsigned int i = 0; i < 3; i++)
	{
		Enemy_1* enemy;
		enemy = new Enemy_1(1);
		_Enemies.push_back(enemy);
	}

	for (unsigned int i = 0; i < 3; i++)
	{
		Enemy_2* enemy;
		enemy = new Enemy_2(1);
		_Enemies.push_back(enemy);
	}

	for (unsigned int i = 0; i < 3; i++)
	{
		Enemy_3* enemy;
		enemy = new Enemy_3(1);
		_Enemies.push_back(enemy);
	}
	sound = new Sound();
}
Enemies::~Enemies()
{
	delete sound; sound = nullptr;
}

vector<Enemy*> Enemies::GetEnemies() { return _Enemies; }  // lấy danh sách enemies
Enemy* Enemies::GetEnemy(int i) { return _Enemies[i]; }
int Enemies::GetSize() { return _Enemies.size(); } // lấy số lượng enemies
void Enemies::SetHealthEnemy(int i) { _Enemies[i]->SetHealth(); } // cài đặt máu cho enemy thứ i
int Enemies::GetHealthEnemy(int i) { return _Enemies[i]->GetHealth(); } // trả về máu của enemy thứ i

void Enemies::DrawEnemies() // ve enemies
{
	for (unsigned int i = 0; i < _Enemies.size(); i++) // vẽ enemies
	{
		_Enemies[i]->Draw();
	}
}
void Enemies::DrawBulletsEnemies()
{
	for (unsigned int i = 0; i < _Enemies.size(); i++) // vẽ enemies
	{
		_Enemies[i]->DrawBullet();
	}
} 
void Enemies::Update(Player* player, bool AlivePlayer)
{
	// cập nhật enemies
	for (unsigned int i = 0; i < _Enemies.size(); i++)
	{
		// thay đổi tọa độ enemies
		_Enemies[i]->SetPosition();

		// khi chạm tường thì đổi hướng
		if (CollisionHandler::GetInstance()->MapCollision(_Enemies[i]->GetBox()) == true)
		{
			_Enemies[i]->ChangeDirection();
		}
		else _Enemies[i]->Moving();

		if (AlivePlayer == true) // nếu player còn sống thì enemies mới bắn player
		{
			// ENEMIES FIRE
			if (_Enemies[i]->GetReload() > 0) _Enemies[i]->DecReload();
			else
			{
				sound->playSound(1, 1); // phát âm thanh mỗi khi bắn enemies

				// thêm đạn enemy
				_Enemies[i]->AddBullet(_Enemies[i]->GetBulletID(), _Enemies[i]->GetX(), _Enemies[i]->GetY(), _Enemies[i]->GetW(), _Enemies[i]->GetH(),
					player->GetX(), player->GetY(), player->GetW(), player->GetH()); // thêm đạn cho mỗi enemy
			
				_Enemies[i]->ResetReload();
			}
		}
		// update hoạt ảnh di chuyển của enemies
		_Enemies[i]->Update();
	}
	DoBulletEnemies();

	// update hoạt ảnh vụ nổ và xóa hoạt ảnh đó sau 1 khoản thời gian
	for (unsigned int i = 0; i < _ExplosionDestroy.size(); i++)
	{
		_ExplosionDestroy[i]->Update();
		if (_ExplosionDestroy[i]->CheckA() <= 0)
		{
			_ExplosionDestroy.erase(_ExplosionDestroy.begin() + i);
		}
	}

	 //update hoạt ảnh đạn bắn trúng đôi tượng và xóa nó sau 1 khoản thời gian
	for (unsigned int i = 0; i < _Enemies.size(); i++) // vẽ vụ nổ nếu đạn bắn trúng đối tượng
	{
		_Enemies[i]->Update_ExplosionBullet();
	}
}

void Enemies::DeleteEnemy(int i) 
{
	// thêm vụ nổ
	int Width = 0, Height = 0;
	SDL_QueryTexture(TextureManager::GetInstance()->GetTexture(_Enemies[i]->GetExplosionID()), NULL, NULL,&Width, &Height);
	Width = Width / 10;

	Explosion* e = new Explosion(_Enemies[i]->GetExplosionID(), _Enemies[i]->GetX(),
		_Enemies[i]->GetY(), Width, Height, 10, 40);
	_ExplosionDestroy.push_back(e);

	// phát âm thanh nổ
	sound->playSound(-1, 3);
	_Enemies.erase(_Enemies.begin() + i); // xóa enemy thứ i 
} 
void Enemies::DeleteBulletEnemy(Enemy* enemy, int i)
{
	enemy->DeleteBullet(i);
}

void Enemies::DoBulletEnemies() // hành đọng của đạn 
{
	for (unsigned int i = 0; i < _Enemies.size(); i++) // vẽ enemies
	{
		_Enemies[i]->DoBullet();
	}
}

void Enemies::DrawExplosion() // vẽ vụ nổ lên màng hình
{
	for (unsigned int i = 0; i < _ExplosionDestroy.size(); i++) // vẽ vụ nổ nếu enemies bị tiêu diệt
	{
		_ExplosionDestroy[i]->Draw();
	}
	for (unsigned int i = 0; i < _Enemies.size(); i++) // vẽ vụ nổ nếu đạn bắn trúng đối tượng
	{
		_Enemies[i]->Draw_ExplosionBullet();
	}
}

void Enemies::Reset(int level)
{
	if (_Enemies.size() > 0) _Enemies.clear();
	// khởi tạo enemeies
	for (unsigned int i = 0; i < 3; i++)
	{
		Enemy_1* enemy;
		enemy = new Enemy_1(level);
		_Enemies.push_back(enemy);
	}

	for (unsigned int i = 0; i < 3; i++)
	{
		Enemy_2* enemy;
		enemy = new Enemy_2(level);
		_Enemies.push_back(enemy);
	}

	for (unsigned int i = 0; i < 3; i++)
	{
		Enemy_3* enemy;
		enemy = new Enemy_3(level);
		_Enemies.push_back(enemy);
	}
}
