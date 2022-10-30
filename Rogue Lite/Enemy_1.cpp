#include "Enemy_1.h"
#include <SDL.h>
#include "CollisionHandler.h"
#include "cmath"

Enemy_1::Enemy_1(int level, SDL_RendererFlip flip)
{
	float d[] = { -3,3 }; // mảng định hướng di chuyển cho enemy

	// lấy kết cấu enemy
	//TextureID = "Enemy_1";
	TextureID = "Enemy_1";
	texture = TextureManager::GetInstance()->GetTexture(TextureID);

	SDL_QueryTexture(texture, NULL, NULL, &Width, &Height); // gán độ dài và rộng của kết cấu vào bullet->w và bullet->h
	FrameCount = 14;
	Width = Width / FrameCount;
	
	// cài đặt hoạt ảnh
	_Animation = new Animation();
	_Animation->SetProps(TextureID, 0, FrameCount, 80);

	// lấy chuỗi kết cấu đạn enemy
	BulletID = "Bullet_Enemy_1";
	frameBullet = 26;

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

	reload = rand() % (500 - level*30) ; // tốc độ ra đạn của eneemy
	Health = level*2 +10; // health ban đầu của enemy
	ExplosionID = "Explosion_1"; // chuỗi hoạt ảnh vụ nổ
}

void Enemy_1::DecReload() { reload--; }
int Enemy_1::GetReload() { return reload; }
void Enemy_1::ResetReload() { reload = rand() % 350; }

vector<Bullet*> Enemy_1::GetBullets() // lấy danh sách đạn
{
	return _Bullets;
}
void Enemy_1::AddBullet(string _BulletTextureID, float x_E, float y_E, int w_E, int h_E, float x_P, float y_P, int w_P, int h_P )
{
	/* ========= cài đặt enemy luôn bắn đạn về phía player */
	// tọa độ trung tâm của đạn enemy
	float x_Enemy = x_E + w_E / 2;
	float y_Enemy = y_E + h_E / 2;

	// tọa độ trung tâm của player
	float x_Player = x_P + w_P / 2;
	float y_Player = y_P + h_P / 2;

	// khoản cách từ enemy đến player
	float distance = (float)sqrt(pow(x_Enemy - x_Player, 2) + pow(y_Enemy - y_Player, 2));


	// khoản di chuyển đạn của enemy
	float _dx = (x_Player - x_Enemy) / (ALIEN_BULLET_SPEED * distance / 2.5f);
	float _dy = (y_Player - y_Enemy) / (ALIEN_BULLET_SPEED * distance / 2.5f);

	// tính góc xoay
	float h = abs(y_Player - y_Enemy); 
	float pi = h / distance;
	float Deg = float(acos(pi)*180/3.14); // góc xoay theo độ

	if (y_Player - y_Enemy > 0) // điều chỉnh lại góc nếu lớn hơn 90
	{
		Deg = 180 - Deg;
	}

	if (x_Player - x_Enemy < 0) // xoay ngược chiều kim đồng hồ nếu giá trị âm và ngược lại
	{
		Deg = -Deg;
	}

	// tạo đạn cho enemy
	Bullet* bullet = new Bullet(BulletID, x_E, y_E, w_E, h_E, frameBullet, _dx, _dy);
	bullet->SetAngle(Deg);

	_Bullets.push_back(bullet);

	// cài đặt khoản di chuyển
	bullet->SetDx(_dx);
	bullet->SetDy(_dy);
}
void Enemy_1::DeleteBullet(int i)
{
	Add_ExplosionBullet(_Bullets[i]->GetPosition().GetX(), _Bullets[i]->GetPosition().GetY());
	_Bullets.erase(_Bullets.begin() + i);
}
void Enemy_1::DoBullet()
{
	for (unsigned int i = 0; i < _Bullets.size(); i++) {

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

void Enemy_1::DrawBullet()
{
	for (unsigned int i = 0; i < _Bullets.size(); i++) // vẽ đạn
	{

		_Bullets[i]->drawBullet();
		//_Animation->Draw(Position.GetX(), Position.GetY(), _Bullets[i]->GetW(), _Bullets[i]->GetH());
	}
}

void Enemy_1::SetPosition()
{
	if (dx < 0) _Animation->SetFlip(SDL_FLIP_HORIZONTAL);
	else _Animation->SetFlip(SDL_FLIP_NONE);
	_Animation->SetProps(TextureID, 0, 14, 80, _Animation->GetFlip());

	Position.TranslateX(dx);
	Position.TranslateY(dy);

	float x = float(Position.GetX() + Width * 0.25);
	float y = float(Position.GetY() + Height * 0.45);
	int w = int(Width - Width * 0.5);
	int h = int(Height - Height * 0.5);

	m_Collider->Set(x, y, w, h);
}
