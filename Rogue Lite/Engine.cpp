#include "Engine.h"
#include "Input.h"
#include "TextureManager.h"
#include "Player.h"
#include "MapParser.h"
#include "Enemies.h"
#include "HitFire.h"
#include "Sound.h"

#include <SDL_ttf.h>


Engine* Engine::Instance = nullptr;
Player* player;
Enemies* enemies;

Engine* Engine::GetInstance()
{
	return Instance = (Instance != nullptr) ? Instance : new Engine();
}

bool Engine::Init() // hàm khởi tạo trò chơi, nếu khởi tạo thành công thì trả về true, ngược lại false
{
	srand((int)time(0));
	Running = true;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0 && IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG))
	{
		SDL_Log("Fail to initialize SDL: %s", SDL_GetError()); // thông màng hình console
		return Running = false;
	}

	Window = SDL_CreateWindow("ROGUE LITE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (Window == nullptr)
	{
		SDL_Log("Fail to initialize Window: %s", SDL_GetError());
		return Running = false;
	}

	/* tham số thứ 3 lad cách trình kết xuất (renderer) hoạt động
		đồng bộ nó với tần số của hệ thống
	*/
	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (Renderer == nullptr)
	{
		SDL_Log("Fail to initialize renderer: %s", SDL_GetError());
		Running = false;
		return Running;
	}
	
	TTF_Init();

	MapID = 1; // chỉ số map ban đầu

	// lấy map
	m_LevelMap = MapParser::GetInstance()->GetMap("MAP1"); // MAP1, MAP2, MAP3

	TextureManager::GetInstance()->Load("bg", "images\\background.png"); // tải hình ảnh back ground

	// player
	TextureManager::GetInstance()->Load("Idle", "images\\player\\Player.png"); // tải hoạt ảnh đưng im của player
	TextureManager::GetInstance()->Load("Bullet", "images\\player\\Bullet.png"); // tải ảnh đạn player

	// enemy 1
	TextureManager::GetInstance()->Load("Enemy_1", "images\\enemy_1\\Enemy.png"); // tải ảnh enemy 1
	TextureManager::GetInstance()->Load("Bullet_Enemy_1", "images\\enemy_1\\Bullet_Enenmy_1.png"); // tải ảnh đạn enemy 1

	// enemy 2
	TextureManager::GetInstance()->Load("Enemy_2", "images\\enemy_2\\enemy_2.png"); // tải ảnh enemy 1
	TextureManager::GetInstance()->Load("Bullet_Enemy_2", "images\\enemy_2\\Bullet_enemy_2.png"); // tải ảnh đạn enemy 2

	// enemy 3
	TextureManager::GetInstance()->Load("Enemy_3", "images\\enemy_3\\Enemy_3.png"); // tải ảnh enemy 3
	TextureManager::GetInstance()->Load("Bullet_Enemy_3", "images\\enemy_3\\Bullet_Enemy_3.png"); // tải ảnh đạn enemy 3
	TextureManager::GetInstance()->Load("Red_Circle", "images\\enemy_3\\red_circle.png"); // tải ảnh đạn enemy 3

	// Explosion
	TextureManager::GetInstance()->Load("Explosion_1", "images\\Exploision\\Explosion.png"); // tải vụ nổ khi enemies bị tiêu diệt
	TextureManager::GetInstance()->Load("ExplosionBullet", "images\\Exploision\\ExploisionBullet.png"); // tải vụ nổ khi đạn bắn trúng đối tượng
	
	// health
	TextureManager::GetInstance()->Load("health_0", "images\\health\\0.png");
	TextureManager::GetInstance()->Load("health_2", "images\\health\\20.png");
	TextureManager::GetInstance()->Load("health_4", "images\\health\\40.png");
	TextureManager::GetInstance()->Load("health_6", "images\\health\\60.png");
	TextureManager::GetInstance()->Load("health_8", "images\\health\\80.png");
	TextureManager::GetInstance()->Load("health_10", "images\\health\\100.png");

	// menu
	TextureManager::GetInstance()->Load("bgMenu", "images\\background\\bgMenu.jpg");
	TextureManager::GetInstance()->Load("play", "images\\background\\play.png");
	TextureManager::GetInstance()->Load("tutorial", "images\\background\\tutorial.png");
	TextureManager::GetInstance()->Load("score", "images\\background\\score.png");
	TextureManager::GetInstance()->Load("quit", "images\\background\\quit.png");

	//menu click
	TextureManager::GetInstance()->Load("playClick", "images\\background\\playClick.png");
	TextureManager::GetInstance()->Load("tutorialClick", "images\\background\\tutorialClick.png");
	TextureManager::GetInstance()->Load("scoreClick", "images\\background\\scoreClick.png");
	TextureManager::GetInstance()->Load("quitClick", "images\\background\\quitClick.png");

	// tạo player
	player = new Player("Idle", 500, 200, 27, 80);

	// khởi tạo enemy
	enemies = new Enemies();
	return Running;
}
void Engine::Render()
{
	// xóa kết xuất cũ
	SDL_RenderClear(Renderer);
	
	// vẽ background
	TextureManager::GetInstance()->Draw("bg", 0, 0, 1280, 640);
	// kết xuất map
	m_LevelMap->Render();

	if (player->GetAlive() == true)
	{
		player->Draw(); // nếu player còn sống thì vẽ player
		player->Draw_ExplosionBullet();
	}
	// vẽ enemies
	if (enemies->GetSize() > 0) enemies->DrawEnemies();

	// vẽ đạn của enemies
	enemies->DrawBulletsEnemies();
	enemies->DrawExplosion(); // vẽ vụ nổ

	// cập nhật thanh máu
	switch (player->GetHealth())
	{
	case 0:
		TextureManager::GetInstance()->Draw("health_0", 0, 0, 300, 40);
		break;
	case 1:
	case 2:
		TextureManager::GetInstance()->Draw("health_2", 0, 0, 300, 40);
		break;
	case 3:
	case 4:
		TextureManager::GetInstance()->Draw("health_4", 0, 0, 300, 40);
		break;
	case 5:
	case 6:
		TextureManager::GetInstance()->Draw("health_6", 0, 0, 300, 40);
		break;
	case 7:
	case 8:
		TextureManager::GetInstance()->Draw("health_8", 0, 0, 300, 40);
		break;
	case 9:
	case 10:
		TextureManager::GetInstance()->Draw("health_10", 0, 0, 300, 40);
		break;
	}

	SDL_RenderPresent(Renderer);
}
void Engine::Update()
{
	m_LevelMap->Update();
	player->Update(enemies->GetEnemies());

	// nếu có đạn player bắn trúng enemy thì trừ máu enemy đó
	vector<int> _CheckEnmies = HitFire::GetInstance()->BulletHitEnemies(player->GetBullets(), enemies->GetEnemies());
	if (_CheckEnmies.size() != 0)
	{
		player->DeleteBullet(_CheckEnmies[0]); // xóa đạn có chỉ số t[0]
		enemies->SetHealthEnemy(_CheckEnmies[1]); // giảm máu enemy có chỉ số t[1]

		// khi enemies máu <= 0 thì xóa enemies đó
		if (enemies->GetHealthEnemy(_CheckEnmies[1]) <= 0)
		{
			// xóa enemies khỏi danh sách 
			enemies->DeleteEnemy(_CheckEnmies[1]);
		}
	}

	// nếu có đạn enemies bắn trúng player thì trừ máu player
	vector<Enemy*> _enemies = enemies->GetEnemies();
	unsigned int n = _enemies.size();
	for (unsigned int i = 0; i < n; i++)
	{
		vector<Bullet*> bullets = _enemies[i]->GetBullets();
		int m = bullets.size();
		
		int _CheckPlayer = HitFire::GetInstance()->BulletHitPlayer(bullets, player);// 
		if (_CheckPlayer != -1)
		{
			player->SetHealth(); // trừ máu
			enemies->DeleteBulletEnemy(enemies->GetEnemy(i), _CheckPlayer); // xóa đạn của enemy
		}
	}

	enemies->Update(player, player->GetAlive());
}

void Engine::Quit() // khi bấm tắt trò chơi
{
	Running = false;
}
void Engine::Event() // sự kiện chung của game
{
	Input::GetInstance()->Listen();
}

void Engine::Clean() // dọn dẹp vùng nhớ
{
	if (Running == false)
	{
		TTF_Quit();
		SDL_DestroyRenderer(Renderer);
		SDL_DestroyWindow(Window);

		IMG_Quit();
		SDL_Quit();

		SDL_Log("everything has been deleted");
	}
}

void Engine::Reset() // reset màng mới
{
	if (player->GetAlive() == false) // player ngủm
	{
		// trả về giao diện kết thúc game, replay, exit
		
	}
	else if(enemies->GetSize() == 0) // enemies ngủm hết
	{
		MapID++;

		//
		// 
		// giao diện chuyển map
		// 
		// 
		
		// chuyen map
		if (MapID == 1) // load map 1
		{
			m_LevelMap = MapParser::GetInstance()->GetMap("MAP1");
		}
		else if (MapID == 2) // load map 2
		{
			m_LevelMap = MapParser::GetInstance()->GetMap("MAP2");
		}
		else if (MapID == 3) // load map 3
		{
			m_LevelMap = MapParser::GetInstance()->GetMap("MAP3");
		}
		else if (MapID > 3) // tro ve map 1
		{
			MapID = 1;
			m_LevelMap = MapParser::GetInstance()->GetMap("MAP1");
		}

		enemies->Reset();
	}
}

void Engine::Input()
{
	// xóa kết xuất cũ
	SDL_RenderClear(Renderer);

	TextureManager::GetInstance()->Draw("bgMenu", 0, 0, 1280, 640);

	TextureManager::GetInstance()->Draw("play", 540, 100, 200, 64);
	TextureManager::GetInstance()->Draw("tutorial", 540, 200, 200, 64);
	TextureManager::GetInstance()->Draw("score", 540, 300, 200, 64);
	TextureManager::GetInstance()->Draw("quit", 540, 400, 200, 64);

	SDL_Event event;
	bool check = true;
	while (check == true)
	{
		SDL_Delay(10); // dừng lại cpu nghỉ ngơi
		SDL_PollEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			Running = false;
			check = false;
			break;
		case SDL_MOUSEBUTTONUP: // xóa hình chữ nhật được chọn khi thả nút chọn trái
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				// play
				if (event.button.x >= 540 && event.button.x <= 740 &&
					event.button.y >= 100 && event.button.y <= 164)
				{
					TextureManager::GetInstance()->Draw("play", 540, 100, 200, 64);
					check = false;
				}
				// tutorial
				else if (event.button.x >= 540 && event.button.x <= 740 &&
					event.button.y >= 200 && event.button.y <= 264)
				{
					TextureManager::GetInstance()->Draw("tutorial", 540, 200, 200, 64);


					check = false;
				}
				// score
				else if (event.button.x >= 540 && event.button.x <= 740 &&
					event.button.y >= 300 && event.button.y <= 364)
				{
					TextureManager::GetInstance()->Draw("score", 540, 300, 200, 64);



					check = false;
				}
				// quit
				else if (event.button.x >= 540 && event.button.x <= 740 &&
					event.button.y >= 400 && event.button.y <= 464)
				{
					TextureManager::GetInstance()->Draw("quit", 540, 400, 200, 64);
					check = false;
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN: // xác định hình chữ nhật được chọn
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				// pkay
				if (event.button.x >= 540 && event.button.x <= 740 &&
					event.button.y >= 100 && event.button.y <= 164)
				{
					TextureManager::GetInstance()->Draw("playClick", 540, 100, 200, 64);
				}
				// tutorial
				else if (event.button.x >= 540 && event.button.x <= 740 &&
					event.button.y >= 200 && event.button.y <= 264)
				{
					TextureManager::GetInstance()->Draw("tutorialClick", 540, 200, 200, 64);
				}
				//score
				else if (event.button.x >= 540 && event.button.x <= 740 &&
					event.button.y >= 300 && event.button.y <= 364)
				{
					TextureManager::GetInstance()->Draw("scoreClick", 540, 300, 200, 64);
				}
				// quit
				else if (event.button.x >= 540 && event.button.x <= 740 &&
					event.button.y >= 400 && event.button.y <= 464)
				{
					TextureManager::GetInstance()->Draw("quitClick", 540, 400, 200, 64);
					Running = false;
				}
			}
			break;
		}
		SDL_RenderPresent(Renderer);
	}
}
void Engine::Output()
{
	SDL_RenderClear(Renderer);
	TextureManager::GetInstance()->Draw("bgMenu", 0, 0, 1280, 640);

	TTF_Font* font = TTF_OpenFont("font\\m5x7.ttf", 30);
	if (font == nullptr)
	{
		cout << "thanh cong";
		SDL_Log("Fail to initialize Window: %s", SDL_GetError());
	}
	SDL_Color fg = { 243, 156, 18 };


	// lấy text
	string text = "Welcome you to Stdio.vn";



	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), fg);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect srcRest;
	SDL_Rect desRect;
	TTF_SizeText(font, text.c_str(), &srcRest.w, &srcRest.h);

	srcRest.x = 0;
	srcRest.y = 0;

	desRect.x = 540;
	desRect.y = 200;

	desRect.w = srcRest.w;
	desRect.h = srcRest.h;

	//Copy a portion of the texture to the current rendering target.
	SDL_RenderCopy(Renderer, texture, &srcRest, &desRect);
	//draw to screen
	SDL_RenderPresent(Renderer);

}