#pragma once
#ifndef Input_h
#define Input_h

#include <SDL.h>

// trình sử lý sự kiện
class Input
{
private:
	static Input* s_Instance;
	const Uint8* m_KeyStages; // lưu trữ trạng thái key, 1 - xảy ra sự kiện, 0 - k có điều gì xảy ra

	void KeyDown(); // xử lý sự kiện khi nhấn phím
	void KeyUp(); // xử lý sự kiện khi vừa thả phím
public:

	Input();
	static Input* GetInstance();
	void Listen(); // lắng nghe sự kiện bàn phím
	bool GetKeyDown(SDL_Scancode key); // kiểm tra phím đã được nhấn xuống hay chưa => cập nhật màng hình
};

#endif
