#include <iostream>
#include <GaiaCore.h>

#include "Attack.h"
#include "Movement.h"

#ifdef _DEBUG
int main()
#else
#include <Windows.h>
int WINAPI
WinMain(HINSTANCE hinstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
	ComponentManager::GetInstance()->registerComponent<Attack>("Attack");
	ComponentManager::GetInstance()->registerComponent<Movement>("Movement");

    GaiaCore* g = new GaiaCore();
    g->init();
	g->run();
	g->close();

	delete g;

	return 0;
}