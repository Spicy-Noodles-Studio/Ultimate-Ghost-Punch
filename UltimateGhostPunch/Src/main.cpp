
#include <iostream>
#include "GaiaCore.h"

#include <ComponentManager.h>

#include "Attack.h"
#include "PlayerController.h"
#include "Health.h"
#include "Jump.h"
#include "Movement.h"
#include "GhostMovement.h"
#include "GhostManager.h"
#include "PlayerUI.h"
#include "UltimateGhostPunch.h"

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