
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
#include "Obstacle.h"

#ifdef _DEBUG
int main()
#else
#include <Windows.h>
int WINAPI
WinMain(HINSTANCE hinstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
	ComponentManager::GetInstance()->registerComponent<GhostMovement>("GhostMovement");
	ComponentManager::GetInstance()->registerComponent<Attack>("Attack");
	ComponentManager::GetInstance()->registerComponent<Movement>("Movement");
	ComponentManager::GetInstance()->registerComponent<PlayerController>("PlayerController");
	ComponentManager::GetInstance()->registerComponent<Health>("Health");
	ComponentManager::GetInstance()->registerComponent<Jump>("Jump");
	ComponentManager::GetInstance()->registerComponent<GhostManager>("GhostManager");
	ComponentManager::GetInstance()->registerComponent<PlayerUI>("PlayerUI");
	ComponentManager::GetInstance()->registerComponent<UltimateGhostPunch>("UltimateGhostPunch");
	ComponentManager::GetInstance()->registerComponent<Obstacle>("Obstacle");

	GaiaCore* g = new GaiaCore();
	g->init();
	g->run();
	g->close();
	
	delete g;

	return 0;
}