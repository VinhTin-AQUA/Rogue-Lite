#include <cstdlib>
#include <ctime>
#include "Engine.h"
#include "Sound.h"


int main(int argc, char* argv[])
{
	// từ phương thức tĩnh gọi bất kỳ tới phương thức nào trong class đó
	Engine::GetInstance()->Init();

	Engine::GetInstance()->Menu();

	while (Engine::GetInstance()->IsRunning())
	{
		Engine::GetInstance()->Update();
		Engine::GetInstance()->Event();
		Engine::GetInstance()->Render();
		Engine::GetInstance()->Reset();
	}

	Engine::GetInstance()->Clean();



	return 1;
}