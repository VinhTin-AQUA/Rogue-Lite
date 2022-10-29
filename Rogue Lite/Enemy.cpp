#include "Enemy.h"
#include "CollisionHandler.h"

Enemy::Enemy()
{
	texture = nullptr; // lưu kết cấu (hình ảnh) của enemies
	ExplosionID = ""; // tên chuỗi hoạt ảnh vụ nổ
}


SDL_Rect Enemy::GetBox() { return m_Collider->Get(); }
void Enemy::SetHealth() { Health--; }
int Enemy::GetHealth() { return Health; }
int Enemy::GetW() 
{ 
	SDL_Rect box = GetBox();
	return box.w;
}
int Enemy::GetH() 
{ 
	SDL_Rect box = GetBox();
	return box.h;
}
float Enemy::GetX()
{
	SDL_Rect box = GetBox();
	return (float)box.x;
}
float Enemy::GetY() 
{ 
	SDL_Rect box = GetBox();
	return (float)box.y;
}
void Enemy::SetBulletID(string _BulletID) { BulletID = _BulletID; };
string Enemy::GetBulletID() { return BulletID; };
Collider* Enemy::GetCollider() { return m_Collider; }

string Enemy::GetExplosionID() { return ExplosionID; } // chuỗi ID vụ nổ trong kết cấu

void Enemy::DecReload() { reload--; } // thời gian delay
int Enemy::GetReload() { return reload; } // lấy thời gian delay
void Enemy::ResetReload() { reload = 5; } // reset thời gian delay

vector<Bullet*> Enemy::GetBullets() { return _Bullets; } // lấy danh sách đạn

void Enemy::Draw() // vẽ enemies trên màng hình
{
	// vẽ enemies
	_Animation->Draw(Position.GetX(), Position.GetY(), Width, Height);

	// vẽ ô vuông xung quanh enemy
	/*SDL_Rect box = GetBox();
	SDL_SetRenderDrawColor(Engine::GetInstance()->GetRenderer(), 255, 255, 255, 255);
	SDL_RenderDrawRect(Engine::GetInstance()->GetRenderer(), &box);*/
}
void Enemy::Update() // Updat hoạt ảnh enemy
{
	_Animation->Update();
}

void Enemy::ChangeDirection() // thay đổi hướng di chuyển
{
	dx = -dx;
	dy = -dy;
}
void Enemy::Moving() // di chuyển ngẫu nhiên của enemíes
{
	float d[] = { -2,2 };

	int w = rand() % SCREEN_WIDTH - 400; 
	int h = rand() % SCREEN_HEIGHT - 300;
	
	if (dx == 0)
	{
		if (Position.GetY() < 110 || Position.GetY() > (SCREEN_HEIGHT - h))
		{
			dy = 0;
			int i = 0 + rand() % 2;
			dx = d[i];
		}
	}
	else if (dy == 0)
	{
		if (Position.GetX() < 110 || Position.GetX() > (SCREEN_WIDTH - w))
		{
			dx = 0;
			int i = 0 + rand() % 2;
			dy = d[i];
		}
	}
}

void Enemy::SetPosition(){}// thay đổi tọa độ của enemies => di chuyển enemies

void Enemy::DeleteBullet(int i)
{
	_Bullets.erase(_Bullets.begin() + i);
}

void Enemy::Add_ExplosionBullet(float x, float y) // thêm vụ nổ
{
	// thêm vụ nổ
	int Width = 0, Height = 0;
	SDL_QueryTexture(TextureManager::GetInstance()->GetTexture("ExplosionBullet"), NULL, NULL,
		&Width, &Height);
	Width = Width / 8;

	Explosion* e = new Explosion("ExplosionBullet", x, y, Width, Height, 8, 35);
	_ExplosionBullet.push_back(e);
}
void Enemy::Draw_ExplosionBullet() // vẽ vụ nổ
{
	for (unsigned int i = 0; i < _ExplosionBullet.size(); i++)
	{
		_ExplosionBullet[i]->Draw();
	}
}
void Enemy::Update_ExplosionBullet() // update vụ nổ
{
	for (unsigned int i = 0; i < _ExplosionBullet.size(); i++)
	{
		_ExplosionBullet[i]->Update();
		if (_ExplosionBullet[i]->CheckA() <= 0) // thời gian vụ nổ về 0 thi xóa vụ nổ đó
		{
			_ExplosionBullet.erase(_ExplosionBullet.begin() + i);
		}
	}
}
