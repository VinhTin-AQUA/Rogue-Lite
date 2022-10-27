#include "TextureManager.h"

TextureManager* TextureManager::s_Instance = nullptr;

bool TextureManager::Load(string id, string filename) // tải 1 kết cấu
{
	SDL_Surface* surface = IMG_Load(filename.c_str());
	if (surface == nullptr)
	{
		SDL_Log("Fail to load texture: %s, %s", filename.c_str(), SDL_GetError());
		return false;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(Engine::GetInstance()->GetRenderer(), surface);
	if (texture == nullptr)
	{
		SDL_Log("Fail to load texture from surface: %s", SDL_GetError());
		return false;
	}
	m_TextureMap[id] = texture; // Map / gán giá trị kết cấu

	return true;
}

SDL_Texture* TextureManager::GetTexture(string id) // lấy kết cấu từ id
{
	return m_TextureMap[id]; // ??
}

void TextureManager::Draw(string id, float x, float y, int width, int height, SDL_RendererFlip flip)
{
	SDL_Rect srcRect = { 0, 0, width, height };// phần của hình ảnh mà ta muốn hiển thị
	SDL_Rect dstRect = { (int)x, (int)y, width, height };// vị trí của hình ảnh trên màng hình
	SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[id], &srcRect, &dstRect, 0, nullptr, flip);
}

void TextureManager::DrawFrame(string id, float x, float y, int width, int height, int row, int frame, SDL_RendererFlip flip, float _Angle)
{
	SDL_Rect srcRect = { width * frame, height * row, width, height };// phần của hình ảnh mà ta muốn hiển thị
	SDL_Rect dstRect = { (int)x, (int)y, width, height };// vị trí của hình ảnh trên màng hình
	SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[id], &srcRect, &dstRect, _Angle, nullptr, flip);
}
void TextureManager::DrawTile(string tilesetID, int tileSize, float x, float y, int row, int frame, SDL_RendererFlip flip)
{
	SDL_Rect dstRect = { (int)x, (int)y, tileSize, tileSize };
	SDL_Rect srcRect = { tileSize * frame, tileSize * row, tileSize, tileSize };
	SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[tilesetID], &srcRect, &dstRect, 0, 0, flip);
}
void TextureManager::DrawEntity(SDL_Texture* texture, float x, float y)
{
	SDL_Rect dest; // hình chữ nhật
	 
	dest.x = (int)x; // set tọa độ x của hình vuông
	dest.y = (int)y; // set tọa độ y của hình vuông
	// dest.w // chiều rộng của hình vuông
	// dest.h // chiều cao của hình vuông

	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h); // lấy và lưu chiều rộng và chiều cao

	SDL_RenderCopy(Engine::GetInstance()->GetRenderer(), texture, NULL, &dest);
}

void TextureManager::Drop(string id) // xóa 1 kết cấu khỏi danh sách
{
	SDL_DestroyTexture(m_TextureMap[id]); // chỉ mới giải phóng thành con trỏ null chứ chưa xóa nó khỏi danh sách
	m_TextureMap.erase(id); // xóa nó khỏi danh sách
}

void TextureManager::Clean() // giải phóng toàn bộ kết cấu
{
	map<string, SDL_Texture*>::iterator it;
	for (it = m_TextureMap.begin(); it != m_TextureMap.end(); it++) // xóa kết cấu trong danh sách
	{
		SDL_DestroyTexture(it->second);
	}
	m_TextureMap.clear(); // xóa danh sách
	SDL_Log("Texture map cleaned");
}