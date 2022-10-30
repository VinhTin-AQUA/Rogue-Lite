#include <cstdlib>
#include <ctime>
#include "Engine.h"
#include "Sound.h"


int main(int argc, char* argv[])
{
	int t = 0;
	// từ phương thức tĩnh gọi bất kỳ tới phương thức nào trong class đó
	Engine::GetInstance()->Init();
	do
	{
		if(t != 2) Engine::GetInstance()->Menu();
		else
		{
			int check = Engine::GetInstance()->InputName();
			if(check == -1) Engine::GetInstance()->Menu();
		}
		
		while (Engine::GetInstance()->IsRunning())
		{
			Engine::GetInstance()->Update();
			Engine::GetInstance()->Event();
			Engine::GetInstance()->Render();
		}
		t = Engine::GetInstance()->Output();
	} while (t != 0);



	Engine::GetInstance()->Clean();
	return 0;
}