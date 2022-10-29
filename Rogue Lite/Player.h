#pragma once

#include <SDL.h>
#include <string>
#include "Character.h"
#include "Animation.h"
#include "Transform.h"
#include "Collider.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Sound.h"
#include "Explosion.h"

using namespace std;

const int PLAYER_SPEED = 10; // tốc độ chạy của player, mỗi lần di chuển 5 pixel
const int FPS_BULLET = 5; //  tốc độ bắn của player

class Player : public Character
{
private:
	int Speed;
	bool Alive;
	Sound* sound; 
public:
	Player(string id, float x, float y, int frameCount, int speed, SDL_RendererFlip flip = SDL_FLIP_NONE);
	~Player();

	virtual float GetX();
	virtual float GetY(); 
	virtual int GetW();
	virtual int GetH(); 
	virtual SDL_Rect GetBox();
	virtual void SetHealth();
	virtual int GetHealth();
	void ResetHealth();
	virtual void SetBulletID(string _BulletID); 
	virtual string GetBulletID(); 
	virtual Collider* GetCollider(); 

	// bullets
	vector<Bullet*> GetBullets(); 
	void DeleteBullet(int i);

	// alive
	bool GetAlive(); 
	
	// animation
	void SetPlayer(string id, float x, float y, int frameCount, int speed, int health, SDL_RendererFlip flip = SDL_FLIP_NONE);
	virtual void Draw(); // vẽ animation
	void Update(vector<Enemy*> Enemies); //  xử lý sự kiện trong này

	// bullet
	void doBullets(); // thiết lập di chuyển và kiểm tra va chạm

	// xử lý vụ nổ khi bắn trúng đối tượng
	virtual void Add_ExplosionBullet(float x, float y); // thêm vụ nổ
	virtual void Draw_ExplosionBullet(); // vẽ vụ nổ
	virtual void Update_ExplosionBullet(); // up date vụ nổ, sau 1 thời gian hiển thị, xóa

	// reset player
	void Reset();
};

