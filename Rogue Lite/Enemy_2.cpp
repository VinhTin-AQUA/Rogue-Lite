#include <SDL.h>
#include "Enemy_2.h"
#include "CollisionHandler.h"
#include "cmath"

Enemy_2::Enemy_2(int level, SDL_RendererFlip flip)
{
	float d[] = { -3,3 }; // mảng định hướng cho enemy

	// lấy kết cấu enemy
	//TextureID = "Enemy_1";
	TextureID = "Enemy_2";
	texture = TextureManager::GetInstance()->GetTexture(TextureID);

	SDL_QueryTexture(texture, NULL, NULL, &Width, &Height); // gán độ dài và rộng của kết cấu vào bullet->w và bullet->h

	FrameCount = 16; ///////////////////////////////////////// số khung hình enemy
	Width = Width / FrameCount;

	// cài đặt hoạt ảnh
	_Animation = new Animation();
	_Animation->SetProps(TextureID, 0, FrameCount, 80);

	// lấy chuỗi kết cấu đạn enemy
	BulletID = "Bullet_Enemy_2";
	frameBullet = 34; /////////////////////////////////// số khung hình đạn enemy

	//lấy vị trí ngẫu nhiên sao cho không trùng với tường
	do
	{
		// vị trí ngẫu nhiên của enemies 
		Position.SetX((float)(rand() % SCREEN_WIDTH));
		Position.SetY((float)(rand() % SCREEN_HEIGHT));

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
	reload = rand() % (500 - level*30); // tốc độ ra đạn của eneemy
	Health = level*2 + HEALTH_ENEMY; // health ban đầu của enemy
	ExplosionID = "Explosion_1"; // chuỗi hoạt ảnh vụ nổ

}

void Enemy_2::DecReload() { reload--; }
int Enemy_2::GetReload() { return reload; }
void Enemy_2::ResetReload() { reload = rand() % 100 + 200; }

vector<Bullet*> Enemy_2::GetBullets() // lấy danh sách đạn
{
	return _Bullets;
}
void Enemy_2::AddBullet(string _BulletTextureID, float x_E, float y_E, int w_E, int h_E, float x_P, float y_P, int w_P, int h_P)
{
	srand((int)time(0));
	int _rand = rand() % 2; // cách bắn ngẫu nhiên

	float BULLET_SPEED = float(rand() % 5 + 1); // tốc độ bắn ngẫu nhiên

	// tọa độ trung tâm của enemey
	float x_Enemy = x_E + w_E / 2;
	float y_Enemy = y_E + h_E / 2;

	float _dx = 0;
	float _dy = 0;

	// 1 lần thêm 4 viên đạn để bắn ra 4 hướng

	if (_rand == 0) // bắn theo hình dấu +
	{
		// bắn lên
		_dx = 0;
		_dy = -BULLET_SPEED;
		Bullet* bullet1 = new Bullet(BulletID, x_E, y_E, w_E, h_E, frameBullet, _dx, _dy);
		_Bullets.push_back(bullet1);

		// bắn xuống
		_dx = 0;
		_dy = +BULLET_SPEED;
		Bullet* bullet2 = new Bullet(BulletID, x_E, y_E, w_E, h_E, frameBullet, _dx, _dy);
		_Bullets.push_back(bullet2);

		// bắng trái
		_dx = -BULLET_SPEED;
		_dy = 0;
		Bullet* bullet3 = new Bullet(BulletID, x_E, y_E, w_E, h_E, frameBullet, _dx, _dy);
		_Bullets.push_back(bullet3);

		// bắn phải
		_dx = BULLET_SPEED;
		_dy = 0;
		Bullet* bullet4 = new Bullet(BulletID, x_E, y_E, w_E, h_E, frameBullet, _dx, _dy);
		_Bullets.push_back(bullet4);
	}
	else if (_rand == 1) // bắn theo hình dấu X
	{
		_dx = -BULLET_SPEED;
		_dy = -BULLET_SPEED;
		Bullet* bullet1 = new Bullet(BulletID, x_E, y_E, w_E, h_E, frameBullet, _dx, _dy);
		_Bullets.push_back(bullet1);

		_dx = BULLET_SPEED;
		_dy = BULLET_SPEED;
		Bullet* bullet2 = new Bullet(BulletID, x_E, y_E, w_E, h_E, frameBullet, _dx, _dy);
		_Bullets.push_back(bullet2);

		// bắng trái
		_dx = -BULLET_SPEED;
		_dy = BULLET_SPEED;
		Bullet* bullet3 = new Bullet(BulletID, x_E, y_E, w_E, h_E, frameBullet, _dx, _dy);
		_Bullets.push_back(bullet3);

		// bắn phải
		_dx = BULLET_SPEED;
		_dy = -BULLET_SPEED;
		Bullet* bullet4 = new Bullet(BulletID, x_E, y_E, w_E, h_E, frameBullet, _dx, _dy);
		_Bullets.push_back(bullet4);
	}
}
void Enemy_2::DeleteBullet(int i)
{
	Add_ExplosionBullet(_Bullets[i]->GetPosition().GetX(), _Bullets[i]->GetPosition().GetY());
	_Bullets.erase(_Bullets.begin() + i);
}
void Enemy_2::DoBullet()
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
	}
}

void Enemy_2::DrawBullet()
{
	for (unsigned int i = 0; i < _Bullets.size(); i++) // vẽ đạn
	{
		_Bullets[i]->drawBullet();
		//_Animation->Draw(Position.GetX(), Position.GetY(), _Bullets[i]->GetW(), _Bullets[i]->GetH());
	}
}

void Enemy_2::SetPosition()
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

	m_Collider->Set(x, y, w, h);
}
