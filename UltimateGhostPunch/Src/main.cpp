
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

#include "InterfaceSystem.h"
#include "SceneManager.h"

using namespace CEGUI;

bool singlePlayerClick(const EventArgs& eventArgs)
{
	SceneManager::GetInstance()->changeScene("mainScene");
	return false;
}


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

	InterfaceSystem::GetInstance()->registerEvent("singlePlayerClick", UIEvent("singlePlayerClick", &singlePlayerClick));

	GaiaCore* g = new GaiaCore();
	g->init();
	g->run();
	g->close();
	
	delete g;

	return 0;
}