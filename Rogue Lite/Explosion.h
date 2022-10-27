#pragma once
#ifndef Explosion_h
#define Explosion_h

#include <string>
#include <SDL.h>
#include "Engine.h"
#include "Animation.h"


using namespace std;

class Explosion
{
private:
	float x; // vị trí của vụ nổ
	float y; // vị trí của vụ nổ
	int width, height;
	int r, g, b; // hiệu ứng: màu sắc, vụ nổ kết thúc
	int a; // thời gian hoạt ảnh được hiển thị
	Animation* _Animation;
	string texture;
public:
	Explosion(string id, float _x, float _y, int _width, int _height, int frameCount, int _a);
	~Explosion();
	float GetX();
	float GetY();
	int GetR();
	int GetG();
	int GetB();
	int GetA();

	int CheckA(); // kiểm tra thời gian hoạt ảnh và giảm dần
	void Draw(bool check=true); // vẽ hoạt ảnh
	void Update(); // cập nhật hoạt ảnh
};

#endif