#pragma once
#ifndef Bullet_h
#define Bullet_h

#include <SDL.h>
#include <vector>
#include "Collider.h"
#include "Transform.h"
#include "Animation.h"

using namespace std;

const float BULLET_SPEED = 15; // tốc độ di chuyển của đạn
const int POWER_BULLET = 1; // sức mạnh của đạn

class Bullet
{
private:
	Transform BulletPosition;
	int w; 
	int h; 
	float dx;
	float dy; 
	int health;
	string TextureID;
	SDL_Texture* texture;
	Collider* m_BulletCollider; 
	Animation* _Animation;
	float Angle; 

public:


	Bullet(string id, float _x, float _y, int _w, int _h, int frame,
		float _dx = BULLET_SPEED, float _dy = 0);
	Transform GetPosition();
	
	int GetW();
	int GetH(); 
	float GetDx();
	void SetDx(float _dx);

	float GetDy();
	void SetDy(float _dy);
	void SetAngle(float _Angle);
	float GetAngle();

	void SetCollider(); 
	void SetPosition();
	
	SDL_Rect GetBox();

	void drawBullet(); 
	bool bulletHitWall(); 

	void Update(); 


};

#endif
