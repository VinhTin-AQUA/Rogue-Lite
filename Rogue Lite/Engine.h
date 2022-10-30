#pragma once

#include <SDL.h>
#include <iostream>
#include <fstream>
#include "GameMap.h"
#include "Sound.h"

using namespace std;


#define SCREEN_WIDTH 1280 // 40
#define SCREEN_HEIGHT 640 // 20

class Engine
{
private:
	static Engine* Instance; //
	bool Running; // kiểm tra game đang chạy
	SDL_Window* Window; // cửa sổ
	SDL_Renderer* Renderer; // kết xuất
	GameMap* m_LevelMap; // chứa map
	int MapID; // chỉ số map
	
	string name;
	int level;
	int score;

	Sound sound;

public:
	static Engine* GetInstance();

	bool Init(); // khởi tạo
	void Reset(); // reset màng mới
	void Render(); // kết xuất ra màng hình
	void Update(); // cập nhật
	void Quit(); // thoát game
	void Event(); // xử lí sự kiện chung của game
	void Clean(); // dọn dẹp bộ nhớ trước khi thoát game

	inline bool IsRunning() { return Running; };
	inline SDL_Window* GetWindow() { return Window; }
	inline SDL_Renderer* GetRenderer() { return Renderer; }
	inline GameMap* GetMap() { return m_LevelMap; }

	// reset stage
	void ResetStage();
	// menu
	void Menu();
	// input - output
	int Input();
	int Output();
	// tutorial
	void Tutorial();
	void Score();
	// inpunt name
	int InputName();
	// date and time
	string DateTime();
	// write score
	void WriteInfor();
};

