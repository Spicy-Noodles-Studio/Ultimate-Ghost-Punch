
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

#include "GameManager.h"

//------------------ EVENTS IMPLEMENTATION (esto ira en un archivo aparte!)


// MAIN MENU EVENTS
bool singlePlayerButtonClick()
{
	SceneManager::GetInstance()->changeScene("mainScene");
	return false;
}

bool multiplayerButtonClick()
{
	SceneManager::GetInstance()->changeScene("fightConfig");
	return false;
}

bool optionsButtonClick()
{
	SceneManager::GetInstance()->changeScene("optionsScene");
	return false;
}

bool exitButtonClick()
{
	
	return false;
}

// FIGHT CONFIG EVENTS
bool fightButtonClick()
{
	SceneManager::GetInstance()->changeScene("mainScene");
	return false;
}



//-------------

#ifdef _DEBUG
int main()
#else
#include <Windows.h>
int WINAPI
WinMain(HINSTANCE hinstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
	// ---Components registering
	ComponentManager::GetInstance()->registerComponent<GhostMovement>("GhostMovement");
	ComponentManager::GetInstance()->registerComponent<Attack>("Attack");
	ComponentManager::GetInstance()->registerComponent<Movement>("Movement");
	ComponentManager::GetInstance()->registerComponent<PlayerController>("PlayerController");
	ComponentManager::GetInstance()->registerComponent<Health>("Health");
	ComponentManager::GetInstance()->registerComponent<Jump>("Jump");
	ComponentManager::GetInstance()->registerComponent<GhostManager>("GhostManager");
	ComponentManager::GetInstance()->registerComponent<PlayerUI>("PlayerUI");
	ComponentManager::GetInstance()->registerComponent<UltimateGhostPunch>("UltimateGhostPunch");
	ComponentManager::GetInstance()->registerComponent<GameManager>("GameManager");

	// ---Events registering
	// mainMenu
	InterfaceSystem::GetInstance()->registerEvent("singlePlayerButtonClick", UIEvent("ButtonClicked", &singlePlayerButtonClick));
	InterfaceSystem::GetInstance()->registerEvent("multiplayerButtonClick", UIEvent("ButtonClicked", &multiplayerButtonClick));
	InterfaceSystem::GetInstance()->registerEvent("optionsButtonClick", UIEvent("ButtonClicked", &optionsButtonClick));
	InterfaceSystem::GetInstance()->registerEvent("exitButtonClick", UIEvent("ButtonClicked", &exitButtonClick));

	// fightConfigMenu
	InterfaceSystem::GetInstance()->registerEvent("fightButtonClick", UIEvent("ButtonClicked", &fightButtonClick));


	GaiaCore* g = new GaiaCore();
	g->init();
	g->run();
	g->close();
	
	delete g;

	return 0;
}