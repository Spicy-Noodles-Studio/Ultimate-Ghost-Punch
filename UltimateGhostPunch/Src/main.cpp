#include "GaiaCore.h"

#include "InterfaceSystem.h"
#include "SceneManager.h"
#include "GameManager.h"
	/***GameManager::GetInstance()->pauseGame(false);backButtonClick***/

#ifdef _DEBUG
int main()
#else
#include <Windows.h>
int WINAPI
WinMain(HINSTANCE hinstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
	GaiaCore* g = new GaiaCore();
	g->init();
	g->run();
	g->close();
	
	delete g;

	return 0;
}