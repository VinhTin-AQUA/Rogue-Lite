#pragma once
#ifndef Character_h
#define Character_h

#include <iostream>
#include "Animation.h"
#include "Transform.h"
#include "Collider.h"
#include "Bullet.h"
#include "Explosion.h"

using namespace std;

class Character
{
protected:
	Transform Position; 
	vector<Bullet*> _Bullets;
	int reload;
	int FrameCount;
	int frameBullet;
	int Width, Height; 
	int Health; 
	float dx, dy;
	string TextureID; 
	Animation* _Animation;
	string BulletID;
	Collider* m_Collider; 
	vector<Explosion*> _ExplosionBullet;
public:
	Character();
	
	virtual float GetX()=0;
	virtual float GetY()=0; 
	virtual int GetW()=0;
	virtual int GetH()=0; 
	virtual SDL_Rect GetBox()=0; 
	virtual void SetHealth()=0;
	virtual int GetHealth()=0;
	virtual void SetBulletID(string _BulletID) = 0;
	virtual string GetBulletID()=0;
	virtual Collider* GetCollider()=0;

	virtual void Draw() = 0;


	virtual void Add_ExplosionBullet(float x, float y)=0;
	virtual void Draw_ExplosionBullet() = 0; 
	virtual void Update_ExplosionBullet() = 0;

};

#endif // !Character_h
