#pragma once
#ifndef Collider_h
#define Collider_h

#include <SDL.h>
#include "Transform.h"

class Collider // tạo 1 hình chữ nhật bao quanh đối tượng
{
private:
	SDL_Rect m_Box; // hình chữ nhật bao quanh đối tượng (đạn, player, enymies)

	Transform LastPosition; // 5;6 66;7

public:
	inline SDL_Rect Get() { return m_Box; }
	
	void SetLastPositionX(float x);
	void SetLastPositionY(float y);

	Transform GetLastPosition();
	void Set(float x, float y, int w, int h);

};

#endif
