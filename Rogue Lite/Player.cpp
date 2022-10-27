#include "Player.h"
#include "TextureManager.h"
#include "Input.h"
#include "CollisionHandler.h"
#include "HitFire.h"


// thiết lập ban đầu của player
Player::Player(string id, float x, float y, int frameCount, int speed, SDL_RendererFlip flip)
{
	FrameCount = frameCount; // số lượng khung hình
	Speed = speed; // tốc độ của khung hình

	// thiet lap ban dau cua player
	TextureID = id;  // lưu kết cấu (hoạt ảnh player)
	Position.SetX(x); Position.SetY(y); // vị trí mặc định của player khi vào game

	SDL_QueryTexture(TextureManager::GetInstance()->GetTexture(id), NULL, NULL, &Width, &Height); // gán độ dài và rộng của kết cấu vào bullet->w và bullet->h

	Width = Width / frameCount;

	// cài đặt hoạt ảnh
	_Animation = new Animation();
	_Animation->SetProps(TextureID, 0, frameCount, speed, flip);

	// thiet lap hình vuông xung quanh player
	m_Collider = new Collider();
	m_Collider->Set(Position.GetX(), Position.GetY(), Width, Height); // set tọa độ của hình chữ nhật khớp với player

	Health = 10; // máu của player

	reload = FPS_BULLET; // tốc độ bắn của player

	Alive = true; // cờ hiệu kiểm tra player còn sống k

	sound = new Sound(); // biến khởi tạo âm thanh
}

int Player::GetW() { return Width; }
int Player::GetH() { return Height; }
SDL_Rect Player::GetBox() { return m_Collider->Get(); }
void Player::SetHealth() { Health--; }
int Player::GetHealth() { return Health; }
float Player::GetX() { return Position.GetX(); } // lấy tọa độ x
float Player::GetY() { return Position.GetY(); } // lấy tọa độ y
void Player::SetBulletID(string _BulletID) { BulletID = _BulletID; }; // cài đặt chuỗi 
string Player::GetBulletID() { return BulletID; }; //
Collider* Player::GetCollider() { return m_Collider; } // 
vector<Bullet*> Player::GetBullets() { return _Bullets; } // trả về vectỏ bullets

// xoá đạn có chỉ số i
void Player::DeleteBullet(int i) // xóa đạn 
{
	Add_ExplosionBullet(_Bullets[i]->GetPosition().GetX(), _Bullets[i]->GetPosition().GetY());
	_Bullets.erase(_Bullets.begin() + i);

} 
bool Player::GetAlive() { return Alive; } // lấy cờ hiệu

Player::~Player()
{
	delete _Animation; _Animation = nullptr;
	delete m_Collider; m_Collider = nullptr;
	delete sound; sound = nullptr;
	TextureManager::GetInstance()->Drop(TextureID);
}

// thiết lập lại thuộc tính của player
void Player::SetPlayer(string id, float x, float y, int frameCount, int speed, SDL_RendererFlip flip)
{
	TextureID = id; // lấy kết cấu
	Position.SetX(x); Position.SetY(y); // cài đặt lại vị trí player
	_Animation->SetProps(TextureID, 0, frameCount, speed, flip);
}

// vẽ player
void Player::Draw()
{
	if (Alive == true)
	{
		_Animation->Draw(Position.GetX(), Position.GetY(), Width, Height);
		// ve o vuong bao boc player
		/*SDL_Rect box = m_Collider->Get();
		SDL_SetRenderDrawColor(Engine::GetInstance()->GetRenderer(), 255, 255, 255, 255);
		SDL_RenderDrawRect(Engine::GetInstance()->GetRenderer(), &box);*/

		// vẽ đạn
		for (unsigned int i = 0; i < _Bullets.size(); i++)
		{
			_Bullets[i]->drawBullet();
		}
	}
}

// cập nhật, xử lý sự kiện player
void Player::Update(vector<Enemy*> Enemies)
{
	if (Alive == true)
	{
		_Animation->SetProps("Idle", 0, FrameCount, Speed, _Animation->GetFlip());
		dx = 0; dy = 0;

		if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_LEFT) == true) // di chuyen sang trai
		{
			dx -= PLAYER_SPEED; // giảm tọa độ x
			// thay đổi animation
			_Animation->SetFlip(SDL_FLIP_HORIZONTAL);
			_Animation->SetProps("Idle", 0, FrameCount, Speed, _Animation->GetFlip());
		}

		if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_RIGHT) == true) // di chuyen sang phai
		{
			dx += PLAYER_SPEED; // tăng tọa độ x
			// thay đổi animation
			_Animation->SetFlip(SDL_FLIP_NONE);
			_Animation->SetProps("Idle", 0, FrameCount, Speed, _Animation->GetFlip());
		}

		if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_DOWN) == true) // di chuyen sang xuong duoi
		{
			dy += PLAYER_SPEED; // tăng tọa độ y
			// thay đổi animation
			_Animation->SetProps("Idle", 0, FrameCount, Speed, _Animation->GetFlip());
		}

		if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_UP) == true) // di chuyen sang len tren
		{
			dy -= PLAYER_SPEED; // giảm tọa độ y
			// thay đổi animation
			_Animation->SetProps("Idle", 0, FrameCount, Speed, _Animation->GetFlip());
		}

		// fire
		if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_SPACE) == true) // bắn đạn
		{
			if (reload > 0) reload--; // thời gian bắn của player
			else
			{
				sound->playSound(0, 0);
				if (Enemies.size() > 0)
				{
					// tìm enemies gần nhất
					float min = 2000;

					float _dxB = 1;
					float _dyB = 1;

					// tọa độ trung tâm của player
					float x_Player = Position.GetX() + (float)Width / 2;
					float y_Player = Position.GetY() + (float)Height / 2;

					for (unsigned int i = 0; i < Enemies.size(); i++)
					{
						// tọa độ trung tâm của enemy
						float x_Enemy = Enemies[i]->GetX() + (float)Enemies[i]->GetW() / 2;
						float y_Enemy = Enemies[i]->GetY() + (float)Enemies[i]->GetH() / 2;

						// tính khoản cách giữa các enemies và player
						float dist = (float)sqrt(pow(x_Enemy - x_Player, 2) + pow(y_Enemy - y_Player, 2));

						// kiểm tra enemy gần player nhất
						if (dist < min)
						{
							min = dist;

							// tốc độ đạn
							_dxB = (x_Enemy - x_Player) / (dist / 17); 
							_dyB = (y_Enemy - y_Player) / (dist / 17);

							if (x_Enemy - x_Player < 0)
							{
								_Animation->SetFlip(SDL_FLIP_HORIZONTAL);
							}
						}
					}
					Bullet* bullet = new Bullet("Bullet", Position.GetX(), Position.GetY(), Width, Height, 4, _dxB, _dyB);
					_Bullets.push_back(bullet);
					reload = FPS_BULLET;
				}
				else
				{
					Bullet* bullet = new Bullet("Bullet", Position.GetX(), Position.GetY(), Width, Height, 4);
					_Bullets.push_back(bullet);
					reload = FPS_BULLET;
				}
			}
		}
		doBullets();

		/* sau khi di chuyển thì cập nhập vị trí mới và vị trí cuối cùng */
		// cập nhật vị trí mới và vị trí cuối cùng
		m_Collider->SetLastPositionX(Position.GetX());
		Position.TranslateX(dx);

		// cập nhật vị trí mới và vị trí cuối cùng
		m_Collider->SetLastPositionY(Position.GetY());
		Position.TranslateY(dy);

		float x = float(Position.GetX() + Width * 0.15);
		float y = float(Position.GetY() + Height * 0.15);
		int w = int(Width - Width * 0.25);
		int h = int(Height - Height * 0.25);

		m_Collider->Set(x, y, w, h);

		// xử lí va chạm
		// nếu player chạm tường thì k cho đi nữa
		if (CollisionHandler::GetInstance()->MapCollision(m_Collider->Get()) == true)
		{
			// cập nhập lại vị trí trước đó
			Position.SetX(m_Collider->GetLastPosition().GetX());
			Position.SetY(m_Collider->GetLastPosition().GetY());
		}

		// cập nhật hoạt ảnh player
		_Animation->Update();

		// nếu player hết máu thì xóa hình vuông xung quanh player để tránh va chạm
		if (Health <= 0)
		{
			Alive = false;
			delete m_Collider;
		}

		//update hoạt ảnh đạn bắn trúng đôi tượng và xóa nó sau 1 khoản thời gian
		Update_ExplosionBullet();
	}
}
void Player::doBullets() // quản lý chuyển động của đạn do player bắn ra 
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
			DeleteBullet(i); // xóa đạn khi va chạm (tránh tràn bộ nhớ)
		}
	}
}
void Player::Add_ExplosionBullet(float x, float y) // thêm vụ nổ của đạn
{
	// thêm vụ nổ
	int Width = 0, Height = 0;
	SDL_QueryTexture(TextureManager::GetInstance()->GetTexture("ExplosionBullet"), NULL, NULL,
		&Width, &Height);
	Width = Width / 8;

	Explosion* e = new Explosion("ExplosionBullet", x, y, Width, Height, 8, 35);
	_ExplosionBullet.push_back(e);
}

void Player::Draw_ExplosionBullet() // vẽ vụ nổ
{
	for (unsigned int i = 0; i < _ExplosionBullet.size(); i++)
	{
		_ExplosionBullet[i]->Draw(false);
	}
}
void Player::Update_ExplosionBullet() // up date vụ nổ
{
	for (unsigned int i = 0; i < _ExplosionBullet.size(); i++)
	{
		_ExplosionBullet[i]->Update();
		if (_ExplosionBullet[i]->CheckA() <= 0) // thời gian vụ nổ tồn tại
		{
			_ExplosionBullet.erase(_ExplosionBullet.begin() + i);
		}
	}
}
void Player::Reset() // reset player
{
	Position.SetX(200); Position.SetY(200); // vị trí mới
	m_Collider->Set(Position.GetX(), Position.GetY(), Width, Height);
	Health = 10; // máu của player
	Alive = true;
}