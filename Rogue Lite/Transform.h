#pragma once
#ifndef Transform_h
#define Transform_h

#include <iostream>
#include "Engine.h"
using namespace std;

// lớp tọa độ của đối tượng

class Transform
{
private:
	float X, Y; // tọa độ (x;y)
public:
	Transform(float x = 0, float y = 0) : X(x), Y(y) {};
	void SetX(float x); // thay đổi vị trí x
	void SetY(float y); // thay đổi vị trí y
	float GetX(); // lấy tọa độ x
	float GetY(); // lấy tọa độ y

	void Log(string mag = ""); // in trang thai

	inline void TranslateX(float x) { X += x; }; // tăng, giảm (tùy vào dấu) của tọa độ x
	inline void TranslateY(float y) { Y += y; }; // tăng, giảm (tùy vào dấu) của tọa độ y
	inline void Translate(float x, float y) { X += x; Y += y; } // kết hợp tăng (giảm) x và y
};

#endif