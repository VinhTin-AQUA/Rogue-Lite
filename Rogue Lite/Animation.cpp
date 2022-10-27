#include "Animation.h"
#include "TextureManager.h"

Animation::Animation()
{
	m_Flip = SDL_FLIP_NONE;
}

void Animation::SetFlip(SDL_RendererFlip flip) { m_Flip = flip; };
SDL_RendererFlip Animation::GetFlip() { return m_Flip; }


void Animation::SetProps(string textureID, int spritRow, int frameCount, int animSpeed, SDL_RendererFlip flip)
{
	m_TextureID = textureID;
	m_SpriteRow = spritRow;
	m_FrameCount = frameCount;
	m_AnimSpeed = animSpeed;
	m_Flip = flip;
}
void Animation::Update() 
{
	m_SpriteFrame = (SDL_GetTicks() / m_AnimSpeed) % m_FrameCount;
}
void Animation::Draw(float x, float y, int spritWidth, int spritHeight, float Angle)
{
	TextureManager::GetInstance()->DrawFrame(m_TextureID, x, y, spritWidth, spritHeight, m_SpriteRow, m_SpriteFrame, m_Flip, Angle);
}
