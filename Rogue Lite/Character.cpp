#include "Character.h"
#include "TextureManager.h"

Character::Character()
{
	Position.SetX(0); Position.SetY(0);
	FrameCount = 0; frameBullet = 0;
	Width = 0, Height = 0;
	Health = 0;
	dx = 0; dy = 0;
	TextureID = "";
	_Animation = nullptr;
	BulletID = "";
	m_Collider = nullptr;
}
