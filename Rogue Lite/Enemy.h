#pragma once
#ifndef Enemy_h
#define Enemy_h

#include <SDL.h>
#include "Character.h"
#include "Bullet.h"
#include "TextureManager.h"

#define FPS 100 // điều chỉnh tốc độ của enemies

class Enemy : public Character
{
protected:
	SDL_Texture* texture;
	string ExplosionID; 
public:
	Enemy();
	virtual float GetX();
	virtual float GetY();
	virtual int GetW(); 
	virtual int GetH();
	virtual SDL_Rect GetBox(); 
	virtual void SetHealth();
	virtual int GetHealth(); 
	virtual void SetBulletID(string _BulletID);
	virtual string GetBulletID(); 

	virtual Collider* GetCollider();
	string GetExplosionID();

	// xử lý thời gian chờ
	virtual void DecReload(); 
	virtual int GetReload(); 
	virtual void ResetReload(); 

	void ChangeDirection(); 
	void Moving(); 
	
	virtual void Draw();
	void Update();

	// tính đa hình ở hàm vẽ đạn, vì mỗi loại enemy có cách tạo đạn, bắn đạn khác nhau
	virtual vector<Bullet*> GetBullets(); // lấy danh sách đạn
	virtual void AddBullet(string _BulletTextureID, float x_E, float y_E, int w_E, int h_E, float x_P, float y_P, int w_P, int h_P){};
	virtual void DeleteBullet(int i);
	virtual void DoBullet(){};
	virtual void DrawBullet(){};
	virtual void SetPosition(); // thay đổi tọa độ của enemies => di chuyển enemies

	// xử lý vụ nổ khi đạn bắn trúng đôi tượng
	virtual void Add_ExplosionBullet(float x, float y); // thêm vụ nổ
	virtual void Draw_ExplosionBullet(); // vẽ vụ nổ
	virtual void Update_ExplosionBullet(); // up date vụ nổ
};

#endif
