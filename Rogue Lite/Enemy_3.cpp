#include <SDL.h>
#include "Enemy_3.h"
#include "CollisionHandler.h"
#include "cmath"


Enemy_3::Enemy_3(SDL_RendererFlip flip)
{
	float d[] = { -3,3 }; // mảng định hướng cho enemy

	// lấy kết cấu enemy
	//TextureID = "Enemy_1";
	TextureID = "Enemy_3";
	texture = TextureManager::GetInstance()->GetTexture(TextureID);

	SDL_QueryTexture(texture, NULL, NULL, &Width, &Height); // gán độ dài và rộng của kết cấu vào bullet->w và bullet->h

	FrameCount = 45; ///////////////////////////////////////// số khung hình enemy
	Width = Width / FrameCount;

	// cài đặt hoạt ảnh
	_Animation = new Animation();
	_Animation->SetProps(TextureID, 0, FrameCount, 80);

	// lấy chuỗi kết cấu đạn enemy
	BulletID = "Bullet_Enemy_3";
	frameBullet = 53; /////////////////////////////////// số khung hình đạn enemy

	//lấy vị trí ngẫu nhiên sao cho không trùng với tường
	do
	{
		// vị trí ngẫu nhiên của enemies 
		Position.SetX(float(rand() % (SCREEN_WIDTH - 500) + 210));
		Position.SetY(float(rand() % (SCREEN_HEIGHT - 500) + 210));

		m_Collider = new Collider;
		// thiết lập hình chữ nhật bao quanh enemy
		m_Collider->Set(Position.GetX(), Position.GetY(), 100, 100);
	} while (CollisionHandler::GetInstance()->MapCollision(m_Collider->Get()) == true);

	// thiết lập tốc độ ngẫu nhiên cho enemy
	dx = (float)(rand() % 6 - 2);
	dy = (float)(rand() % 6 - 2);

	// không thể dx và dy cùng lúc = 0 => để enemy di chuyển theo chiều ngang hoặc chiều dọc
	if (dx == 0)
	{
		int i = 0 + rand() % 2;
		dy = d[i];
	}
	else
	{
		dy = 0;
	}
	reload = rand() % 60; // tốc độ ra đạn của eneemy
	Health = HEALTH_ENEMY; // health ban đầu của enemy
	ExplosionID = "Explosion_1"; // chuỗi hoạt ảnh vụ nổ
	TimerBullet = 190;
}

void Enemy_3::DecReload() { reload--; }
int Enemy_3::GetReload() { return reload; }
void Enemy_3::ResetReload() { reload = rand() % 100 + 200; }

vector<Bullet*> Enemy_3::GetBullets() // lấy danh sách đạn
{
	return _Bullets;
}
void Enemy_3::AddBullet(string _BulletTextureID, float x_E, float y_E, int w_E, int h_E, float x_P, float y_P, int w_P, int h_P)
{
	srand((int)time(0));
	int _rand = rand() % 2; // cách bắn ngẫu nhiên

	float x = float(rand() % 800 + 200);
	float y = float(rand() % 450 + 100);

	Bullet* bullet1 = new Bullet(BulletID, x, y, w_E, h_E, frameBullet, 0, 0);
	_Bullets.push_back(bullet1);

	x = float(rand() % 800 + 200);
	y = float(rand() % 450 + 100);

	Bullet* bullet2 = new Bullet(BulletID, x, y, w_E, h_E, frameBullet, 0, 0);
	_Bullets.push_back(bullet2);
}
void Enemy_3::DeleteBullet(int i)
{
	Add_ExplosionBullet(_Bullets[i]->GetPosition().GetX(), _Bullets[i]->GetPosition().GetY());
	_Bullets.erase(_Bullets.begin() + i);
}
void Enemy_3::DoBullet()
{
	for (unsigned int i = 0; i < _Bullets.size(); i++)
	{
		// cập nhật vị trí đạn
		_Bullets[i]->SetPosition();
		_Bullets[i]->Update();
		// cập nhật vị trí mới cho hình vuông
		_Bullets[i]->SetCollider(); // set tọa độ của hình chữ nhật khớp với player

		// nếu đạn bắn trúng tường thì xóa đạn => tránh tràn bộ nhớ
		if (_Bullets[i]->bulletHitWall() == true || _Bullets[i]->GetPosition().GetX() < -_Bullets[i]->GetW() ||
			_Bullets[i]->GetPosition().GetY() < -_Bullets[i]->GetH() ||
			_Bullets[i]->GetPosition().GetX() > SCREEN_WIDTH || _Bullets[i]->GetPosition().GetY() > SCREEN_HEIGHT)
		{
			Add_ExplosionBullet(_Bullets[i]->GetPosition().GetX(), _Bullets[i]->GetPosition().GetY());
			_Bullets.erase(_Bullets.begin() + i); // xóa đạn khi va chạm tường (tránh tràn bộ nhớ)
		}
		else
		{
			if (TimerBullet > 0) TimerBullet--;
			else
			{
				_Bullets.erase(_Bullets.begin() + i); // xóa đạn khi va chạm tường (tránh tràn bộ nhớ)
				TimerBullet = 250;
			}
		}
	}
}

void Enemy_3::DrawBullet()
{
	for (unsigned int i = 0; i < _Bullets.size(); i++) // vẽ đạn
	{
		_Bullets[i]->drawBullet();
	}
}

void Enemy_3::SetPosition() // set vị trí của enemy
{
	if (dx < 0) _Animation->SetFlip(SDL_FLIP_HORIZONTAL);
	else _Animation->SetFlip(SDL_FLIP_NONE);
	_Animation->SetProps(TextureID, 0, FrameCount, 80, _Animation->GetFlip());

	Position.TranslateX(dx);
	Position.TranslateY(dy);

	float x = float(Position.GetX() + Width * 0.25);
	float y = float(Position.GetY() + Height * 0.15);

	int w = int(Width - Width * 0.5);
	int h = int(Height - Height * 0.25);

	m_Collider->Set(x + w/10, y + h/2, w - w/4 , h - h/2);
}
