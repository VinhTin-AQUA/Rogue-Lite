#include "Bullet.h"
#include "TextureManager.h"
#include "CollisionHandler.h"
#include <SDL_gamecontroller.h>

Bullet::Bullet(string id, float _x, float _y, int _w, int _h, int frame, float _dx, float _dy)
{
	/*
	id: key trong danh sách hoạt ảnh
	_x, _y, _w, _h, : tọa độ, kích thước của player
	_dx, _dy: hướng đạn
	*/
	TextureID = id;
	BulletPosition.SetX(_x + _w / 2); // tọa độ đầu tiên khi sinh đạn
	BulletPosition.SetY(_y + _h / 3); // tọa độ đầu tiên khi sinh đạn

	dx = _dx; // tốc độ di chuyển của đạn
	dy = _dy;

	health = 1;
	texture = TextureManager::GetInstance()->GetTexture(id); // lấy kết cấu
	SDL_QueryTexture(texture, NULL, NULL, &w, &h); // gán độ dài và rộng của kết cấu vào bullet->w và bullet->h
	w = w / frame;

	// thiet lap hình vuông xung quanh player
	m_BulletCollider = new Collider();
	m_BulletCollider->Set(BulletPosition.GetX(), BulletPosition.GetY(), w, h); // set tọa độ của hình chữ nhật khớp với player

	// hoạt ảnh đạn
	_Animation = new Animation;
	_Animation->SetProps(id, 0, frame, 100);

	
}

Transform Bullet::GetPosition() { return BulletPosition; }

int Bullet::GetW() { return w; } // lấy chiều rộng
int Bullet::GetH() { return h; } // lấy chiều cao
float Bullet::GetDx() { return dx; }
void Bullet::SetDx(float _dx) { dx = _dx; }
SDL_Rect Bullet::GetBox() { return m_BulletCollider->Get(); }

float Bullet::GetDy() { return dy; }
void Bullet::SetDy(float _dy) { dy = _dy; }
void Bullet::SetAngle(float _Angle) { Angle = _Angle; };
float Bullet::GetAngle() { return Angle; }

void Bullet::SetCollider() // cài đặt vị trí hình vuông cho khớp với đạn
{
	m_BulletCollider->Set(BulletPosition.GetX(), BulletPosition.GetY(), w, h);
}

void Bullet::SetPosition() // cài đặt vị trí đạn
{
	BulletPosition.TranslateX(dx);
	BulletPosition.TranslateY(dy);
}

void Bullet::drawBullet() // vẽ đạn
{
	_Animation->Draw(BulletPosition.GetX(), BulletPosition.GetY(), w, h, Angle);

	// vẽ hình vuông xung quanh viên đạn
	/*SDL_Rect box = m_BulletCollider->Get();
	SDL_SetRenderDrawColor(Engine::GetInstance()->GetRenderer(), 255, 255, 255, 255);
	SDL_RenderDrawRect(Engine::GetInstance()->GetRenderer(), &box);*/
}

bool Bullet::bulletHitWall() // xử lý đạn trúng tường
{
	// nếu đạn trúng tường thì đạn bị tiêu hủy
	if (CollisionHandler::GetInstance()->MapCollision(m_BulletCollider->Get()) == true)
	{
		return true;
	}
	return false;
}

void Bullet::Update() // update hoạt ảnh đạn
{
	_Animation->Update();
}

