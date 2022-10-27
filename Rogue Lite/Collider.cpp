#include "Collider.h"



void Collider::SetLastPositionX(float x)
{
	LastPosition.SetX(x);
}
void Collider::SetLastPositionY(float y)
{
	LastPosition.SetY(y);
}

Transform Collider::GetLastPosition() { return LastPosition; }
void Collider::Set(float x, float y, int w, int h)
{
	m_Box = {
		(int)x,
		(int)y,
		w,
		h
	};
}
