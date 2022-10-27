#include "Input.h"
#include "Engine.h"
#include <iostream>

Input* Input::s_Instance = nullptr;

Input* Input::GetInstance()
{
	return s_Instance = (s_Instance != nullptr) ? s_Instance : new Input();
}

Input::Input()
{
	m_KeyStages = SDL_GetKeyboardState(nullptr);
}
void Input::Listen() // lắng nghe sự kiện bàn phím
{
	SDL_Event event; // biến xử lý sự kiện
	SDL_Delay(10);  // 
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			Engine::GetInstance()->Quit();
			break;
		case SDL_KEYDOWN:
			KeyDown();
			break;
		case SDL_KEYUP:
			KeyUp();
			break;
		}
	}
}
bool Input::GetKeyDown(SDL_Scancode key) // sự kiện nhấn phím
{
	return (m_KeyStages[key] == 1);
}

void Input::KeyUp()
{
	m_KeyStages = SDL_GetKeyboardState(nullptr);
}
void Input::KeyDown()
{
	m_KeyStages = SDL_GetKeyboardState(nullptr);

	if (GetKeyDown(SDL_SCANCODE_ESCAPE) == true)
	{
		Engine::GetInstance()->Quit();
	}
}
