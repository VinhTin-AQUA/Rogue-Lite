#include "Engine.h"
#include "Input.h"
#include "TextureManager.h"
#include "Player.h"
#include "MapParser.h"
#include "Enemies.h"
#include "HitFire.h"
#include "Sound.h"
#include <cstdlib>
#include <SDL_ttf.h>
#include <ctime>

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
	TextureManager::GetInstance()->Load("Idle", "images\\player\\Idle.png"); // tải hoạt ảnh đưng im của player
	TextureManager::GetInstance()->Load("Attack", "images\\player\\Attack.png");
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

	// tutorial bg, socre bg
	TextureManager::GetInstance()->Load("tutorialBG", "images\\background\\tutorialBG.png");
	TextureManager::GetInstance()->Load("scoreBG", "images\\background\\scoreBG.png");

	// back
	TextureManager::GetInstance()->Load("back", "images\\background\\back.png");
	TextureManager::GetInstance()->Load("backCLick", "images\\background\\backClick.png");

	// menu
	TextureManager::GetInstance()->Load("menu", "images\\background\\menu.png");
	TextureManager::GetInstance()->Load("menuClick", "images\\background\\menuClick.png");

	// exit
	TextureManager::GetInstance()->Load("exit", "images\\background\\exit.png");
	TextureManager::GetInstance()->Load("exitClick", "images\\background\\exitClick.png");

	// input name
	TextureManager::GetInstance()->Load("inputName", "images\\background\\InputName.png");

	// enter
	TextureManager::GetInstance()->Load("enter", "images\\background\\ENTER.png");
	TextureManager::GetInstance()->Load("enterClick", "images\\background\\enterClick.png");

	// tạo player
	player = new Player("Idle", 500, 200, 29, 80);

	// khởi tạo enemy
	enemies = new Enemies();

	name = "Name Player";
	level = 1;
	score = 0;
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
			score++; // tăng điểm số
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
		Running = false;
	}
	else if(enemies->GetSize() == 0) // enemies ngủm hết
	{
		MapID++;

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
			level++;
			m_LevelMap = MapParser::GetInstance()->GetMap("MAP1");
		}
		enemies->Reset(level);
		player->ResetHealth();
	}
}

void Engine::Menu()
{
	if (player->GetAlive() == false)
	{
		level = 1;
		score = 0;
		name = "Name Player";
		MapID = 1;
		m_LevelMap = MapParser::GetInstance()->GetMap("MAP1");
		player->SetPlayer("Idle", 500, 200, 27, 80, 10);
		enemies->Reset(level);
	}

	int check = Input();
	while (check != 0)
	{
		if (check == 1)
		{
			Tutorial();
		}
		else if (check == 2)
		{
			Score();
		}
		check = Input();
	}

	// reset game stage
	Running = true;
}

int Engine::Input()
{
	// xóa kết xuất cũ
	SDL_RenderClear(Renderer);

	TextureManager::GetInstance()->Draw("bgMenu", 0, -40, 1280, 640);

	TextureManager::GetInstance()->Draw("play", 540, 100, 200, 64);
	TextureManager::GetInstance()->Draw("tutorial", 540, 200, 200, 64);
	TextureManager::GetInstance()->Draw("score", 540, 300, 200, 64);
	TextureManager::GetInstance()->Draw("quit", 540, 400, 200, 64);

	SDL_Event event;
	while (true)
	{
		SDL_Delay(10); // dừng lại cpu nghỉ ngơi
		SDL_PollEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			exit(0);
			Running = false;
			return 0;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				// play
				if (event.button.x >= 540 && event.button.x <= 740 &&
					event.button.y >= 100 && event.button.y <= 164)
				{
					TextureManager::GetInstance()->Draw("play", 540, 100, 200, 64);
					return InputName();
				}
				// tutorial
				else if (event.button.x >= 540 && event.button.x <= 740 &&
					event.button.y >= 200 && event.button.y <= 264)
				{
					TextureManager::GetInstance()->Draw("tutorial", 540, 200, 200, 64);
					return 1;
				}
				// score
				else if (event.button.x >= 540 && event.button.x <= 740 &&
					event.button.y >= 300 && event.button.y <= 364)
				{
					TextureManager::GetInstance()->Draw("score", 540, 300, 200, 64);
					return 2;
				}
				// quit
				else if (event.button.x >= 540 && event.button.x <= 740 &&
					event.button.y >= 400 && event.button.y <= 464)
				{
					TextureManager::GetInstance()->Draw("quit", 540, 400, 200, 64);
					exit(0);
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				// play
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
				}
			}
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
			{
				exit(0);
				return 0;
			}
			break;
		}
		SDL_RenderPresent(Renderer);
	}
}

bool Engine::Output()  
{
	SDL_RenderClear(Renderer);
	TextureManager::GetInstance()->Draw("scoreBG", 0, 0, 1280, 640);
	TextureManager::GetInstance()->Draw("menu", 520, 400, 70, 53);
	TextureManager::GetInstance()->Draw("exit", 620, 400, 70, 53);

	TTF_Font* font = TTF_OpenFont("font\\m5x7.ttf", 30);
	if (font == nullptr)
	{
		SDL_Log("Fail to initialize Window: %s", SDL_GetError());
	}
	SDL_Color fg = { 118, 247, 83 };

	SDL_Surface* surface = nullptr;
	SDL_Texture* texture = nullptr;

	surface = TTF_RenderText_Solid(font, "============ GAME OVER ============", fg);
	texture = SDL_CreateTextureFromSurface(Renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect srcRest;
	SDL_Rect desRect;
	TTF_SizeText(font, "============ GAME OVER ============", &srcRest.w, &srcRest.h);

	srcRest.x = 0;
	srcRest.y = 0;
	desRect.x = 500;
	desRect.y = 200;
	desRect.w = srcRest.w;
	desRect.h = srcRest.h;

	SDL_RenderCopy(Renderer, texture, &srcRest, &desRect);
	SDL_RenderPresent(Renderer);

	// =======================  in thong tin ======================
	// name
	string _name = "NAME: " + name;

	surface = TTF_RenderText_Solid(font, _name.c_str(), fg);
	texture = SDL_CreateTextureFromSurface(Renderer, surface);
	TTF_SizeText(font, _name.c_str(), &srcRest.w, &srcRest.h);

	srcRest.x = 0;
	srcRest.y = 0;
	desRect.x = 540;
	desRect.y = 240;

	desRect.w = srcRest.w;
	desRect.h = srcRest.h;

	SDL_RenderCopy(Renderer, texture, &srcRest, &desRect);
	SDL_RenderPresent(Renderer);

	// score
	string _score = "SCORE: " + to_string(score);
	surface = TTF_RenderText_Solid(font, _score.c_str(), fg);
	texture = SDL_CreateTextureFromSurface(Renderer, surface);
	TTF_SizeText(font, _score.c_str(), &srcRest.w, &srcRest.h);

	srcRest.x = 0;
	srcRest.y = 0;
	desRect.x = 540;
	desRect.y = 280;

	desRect.w = srcRest.w;
	desRect.h = srcRest.h;

	SDL_FreeSurface(surface);

	SDL_RenderCopy(Renderer, texture, &srcRest, &desRect);
	SDL_RenderPresent(Renderer);

	// time
	string _time = "TIME: " + DateTime();
	surface = TTF_RenderText_Solid(font, _time.c_str(), fg);
	texture = SDL_CreateTextureFromSurface(Renderer, surface);
	TTF_SizeText(font, _time.c_str(), &srcRest.w, &srcRest.h);

	srcRest.x = 0;
	srcRest.y = 0;
	desRect.x = 540;
	desRect.y = 320;

	desRect.w = srcRest.w;
	desRect.h = srcRest.h;

	SDL_FreeSurface(surface);
	SDL_RenderCopy(Renderer, texture, &srcRest, &desRect);
	SDL_RenderPresent(Renderer);

	SDL_DestroyTexture(texture);

	// ==============================================
	// luu du lieu cu tam thoi
	ifstream ifs("data\\scores.txt");
	if (!ifs.is_open()) {
		cerr << "Could not open the file - '"
			<< "data\\scores.txt" << "'" << endl;
		return EXIT_FAILURE;
	}
	name = name + "\n" + to_string(score) + '\n' + DateTime() + +"\n---------\n";
	string temp = "";
	int i = 1;
	while (getline(ifs, temp)) 
	{
		name += temp + '\n';
		i++;
		if (i == 16) break;
	}
	ifs.close();

	// luu du lieu moi
	ofstream ofs("data\\scores.txt");
	ofs << name;
	ofs.close();
	// ============== run
	SDL_Event event;
	while(true)
	{
		SDL_PollEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			exit(0);
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				// menu
				if (event.button.x >= 500 && event.button.x <= 570 &&
					event.button.y >= 400 && event.button.y <= 453)
				{
					TextureManager::GetInstance()->Draw("menu", 520, 400, 70, 53);
						
					return true;
				}
				// exit
				else if (event.button.x >= 600 && event.button.x <= 670 &&
					event.button.y >= 400 && event.button.y <= 453)
				{
					TextureManager::GetInstance()->Draw("exit", 620, 400, 70, 53);
					exit(0);
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				// menu
				if (event.button.x >= 500 && event.button.x <= 570 &&
					event.button.y >= 400 && event.button.y <= 453)
				{
					TextureManager::GetInstance()->Draw("menuClick", 520, 400, 70, 53);
				}
				// exit
				else if (event.button.x >= 600 && event.button.x <= 670 &&
					event.button.y >= 400 && event.button.y <= 453)
				{
					TextureManager::GetInstance()->Draw("exitClick", 620, 400, 70, 53);
				}
			}
			break;
		case SDL_KEYDOWN: // ESC
			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
			{
				exit(0);
			}
		}
		SDL_RenderPresent(Renderer);
	}
}

void Engine::Tutorial()
{
	// xóa kết xuất cũ
	SDL_RenderClear(Renderer);
	TextureManager::GetInstance()->Draw("tutorialBG", 0, 0, 1280, 640);
	TextureManager::GetInstance()->Draw("back", 10, 10, 70, 70);

	SDL_Event event;
	bool check = true;
	while (check == true)
	{
		SDL_Delay(10); // dừng lại cpu nghỉ ngơi
		SDL_PollEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			exit(0);
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				// back
				if (event.button.x >= 10 && event.button.x <= 80 &&
					event.button.y >= 10 && event.button.y <= 80)
				{
					TextureManager::GetInstance()->Draw("back", 10, 10, 70, 70);
					check = false;
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				// back
				if (event.button.x >= 10 && event.button.x <= 80 &&
					event.button.y >= 10 && event.button.y <= 80)
				{
					TextureManager::GetInstance()->Draw("backCLick", 10, 10, 70, 70);
				}
			}
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
			{
				exit(0);
			}
			break;
		}
		SDL_RenderPresent(Renderer);
	}
}

void Engine::Score()
{
	// xóa kết xuất cũ
	SDL_RenderClear(Renderer);
	TextureManager::GetInstance()->Draw("scoreBG", 0, 0, 1280, 640);
	TextureManager::GetInstance()->Draw("back", 10, 10, 70, 70);

	SDL_Event event;
	bool check = true;
	while (check == true)
	{
		SDL_Delay(10); // dừng lại cpu nghỉ ngơi
		SDL_PollEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			exit(0);
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				// back
				if (event.button.x >= 10 && event.button.x <= 80 &&
					event.button.y >= 10 && event.button.y <= 80)
				{
					TextureManager::GetInstance()->Draw("back", 10, 10, 70, 70);
					check = false;
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN: 
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				// back
				if (event.button.x >= 10 && event.button.x <= 80 &&
					event.button.y >= 10 && event.button.y <= 80)
				{
					TextureManager::GetInstance()->Draw("backCLick", 10, 10, 70, 70);
				}
			}
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
			{
				exit(0);
			}
			break;
		default:
			// in điểm số
			ifstream ifs("data\\scores.txt");
			if (!ifs.is_open()) {
				cerr << "Could not open the file - '"
					<< "data\\scores.txt" << "'" << endl;
			}
			string str = "";

			TTF_Font* font = TTF_OpenFont("font\\m5x7.ttf", 30);
			SDL_Color color1 = { 134, 89, 235 };
			SDL_Color color = { 92, 238, 235 };

			SDL_Surface* surface = TTF_RenderText_Solid(font, "============ SCORES ============", color1);
			SDL_Texture* fontTexture = SDL_CreateTextureFromSurface(Renderer, surface);
			int texW = 0;
			int texH = 0;
			SDL_QueryTexture(fontTexture, NULL, NULL, &texW, &texH);
			SDL_Rect dstrect = { 480, 200, texW, texH };
			SDL_RenderCopy(Renderer, fontTexture, NULL, &dstrect);

			int i = 40;
			while (getline(ifs, str)) {
				
				surface = TTF_RenderText_Solid(font, str.c_str(), color);
				fontTexture = SDL_CreateTextureFromSurface(Renderer, surface);
				SDL_FreeSurface(surface);
				int texW = 0;
				int texH = 0;
				SDL_QueryTexture(fontTexture, NULL, NULL, &texW, &texH);
				SDL_Rect dstrect = { 520, 200 + i, texW, texH };
				SDL_RenderCopy(Renderer, fontTexture, NULL, &dstrect);
				SDL_DestroyTexture(fontTexture);
				i += 20;
				if (i == 360) break;
			}
			ifs.close();
			break;
		}
		SDL_RenderPresent(Renderer);
	}
}

int Engine::InputName()
{
	SDL_RenderClear(Renderer);
	
	TTF_Font* font2 = TTF_OpenFont("font\\m5x7.ttf", 30);
	SDL_Color color{ 238,0,0,255 };

	SDL_Surface* temp = TTF_RenderText_Solid(font2, "CLICK HERE", color);
	SDL_Texture* textImage = SDL_CreateTextureFromSurface(Renderer, temp);

	SDL_Rect pos{ 540, 200, temp->w, temp->h };
	SDL_FreeSurface(temp);
	temp = nullptr;

	bool isRunning = true;
	SDL_Event ev;

	SDL_StartTextInput();
	
	int i = 10;
	while (isRunning)
	{
		SDL_Delay(10); // dừng lại cpu nghỉ ngơi
		SDL_PollEvent(&ev);
		
		switch (ev.type)
		{
		case SDL_QUIT:
			exit(0);
			break;
		case SDL_KEYDOWN:
		case SDL_TEXTINPUT:
			if (ev.key.keysym.scancode == SDL_SCANCODE_BACKSPACE && name.length() > 0)
			{
				name = name.substr(0, name.length() - 1);
			}
			else if (ev.type == SDL_TEXTINPUT)
			{
				if (name.compare("Name Player") == 0) name = "TT: ";
				if (name.length() <= 15)
				{
					name += ev.text.text;
				}
			}
			else if (ev.key.keysym.scancode == SDL_SCANCODE_RETURN)
			{
				SDL_StopTextInput();
				SDL_DestroyTexture(textImage);
				return 0;
			}
			if (textImage)
			{
				SDL_DestroyTexture(textImage);
				textImage = nullptr;
			}
			temp = TTF_RenderText_Solid(font2, name.c_str(), color);
			if (temp)
			{
				textImage = SDL_CreateTextureFromSurface(Renderer, temp);
				pos.w = temp->w;
				pos.h = temp->h;
				SDL_FreeSurface(temp);
				temp = nullptr;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (ev.button.button == SDL_BUTTON_LEFT)
			{
				// back
				if (ev.button.x >= 10 && ev.button.x <= 80 &&
					ev.button.y >= 10 && ev.button.y <= 80)
				{
					TextureManager::GetInstance()->Draw("back", 10, 10, 70, 70);
					SDL_RenderPresent(Renderer);

					SDL_StopTextInput();
					SDL_DestroyTexture(textImage);
					name = "Name Player";
					return -1;
				}
				// enter
				else if (ev.button.x >= 565 && ev.button.x <= 715 &&
					ev.button.y >= 270 && ev.button.y <= 320)
				{
					TextureManager::GetInstance()->Draw("enterClick", 565, 270, 150, 60);
					SDL_RenderPresent(Renderer);

					SDL_StopTextInput();
					SDL_DestroyTexture(textImage);
					return 0;
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (ev.button.button == SDL_BUTTON_LEFT)
			{
				// back
				if (ev.button.x >= 10 && ev.button.x <= 80 &&
					ev.button.y >= 10 && ev.button.y <= 80)
				{
					TextureManager::GetInstance()->Draw("backCLick", 10, 10, 70, 70);
					SDL_RenderPresent(Renderer);
				}
				else if (ev.button.x >= 565 && ev.button.x <= 715 &&
					ev.button.y >= 270 && ev.button.y <= 320)
				{
					TextureManager::GetInstance()->Draw("enterClick", 565, 270, 150, 60);
					SDL_RenderPresent(Renderer);
				}
			}
			break;
		}
		SDL_RenderClear(Renderer);
		TextureManager::GetInstance()->Draw("inputName", 0, 0, 1280, 640);
		TextureManager::GetInstance()->Draw("enter", 565, 270, 150, 60);
		TextureManager::GetInstance()->Draw("back", 10, 10, 70, 70);

		SDL_RenderCopy(Renderer, textImage, nullptr, &pos);
		SDL_RenderPresent(Renderer);
	}
	return -1;
}

string Engine::DateTime()
{
	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);

	
	int Day = newtime.tm_mday;
	int Month = 1 + newtime.tm_mon;
	int Year = 1900 + newtime.tm_year;

	int sec = newtime.tm_sec;
	int min = newtime.tm_min;
	int hour = newtime.tm_hour;

	string time = to_string(Day) + '/' + to_string(Month) + '/' + to_string(Year) + " - " +
		to_string(hour) + ':' + to_string(min) + ':' + to_string(sec);
	return time;
}