#include <cstdlib>
#include <ctime>
#include "Engine.h"
#include "Sound.h"


int main(int argc, char* argv[])
{
	bool check = true;
	// từ phương thức tĩnh gọi bất kỳ tới phương thức nào trong class đó
	Engine::GetInstance()->Init();
	do
	{
		Engine::GetInstance()->Menu();
		
		while (Engine::GetInstance()->IsRunning())
		{
			Engine::GetInstance()->Update();
			Engine::GetInstance()->Event();
			Engine::GetInstance()->Render();
			Engine::GetInstance()->Reset();
		}
		
	} while (Engine::GetInstance()->Output());

	Engine::GetInstance()->Clean();
	return 0;
}