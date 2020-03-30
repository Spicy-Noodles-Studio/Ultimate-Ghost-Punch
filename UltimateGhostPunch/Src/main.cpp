
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
#include "DynamicCamController.h"

#include "InterfaceSystem.h"
#include "SceneManager.h"

#include "GameManager.h"
#include "FightConfiguration.h"

#include "FightManager.h"

//------------------ EVENTS IMPLEMENTATION (esto ira en un archivo aparte!)

// GENERIC EVENTS
bool backButtonClick()
{
	SceneManager::GetInstance()->changeScene("mainMenu");
	return false;
}

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
	SceneManager::GetInstance()->changeScene("options");
	return false;
}

bool exitButtonClick()
{
	//close application
	return false;
}

// PAUSE MENU EVENTS
bool resumeButtonClick()
{
	// hide pause menu
	//findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("PauseBackground");
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
	ComponentManager::GetInstance()->registerComponent<FightConfiguration>("FightConfiguration");
	ComponentManager::GetInstance()->registerComponent<FightManager>("FightManager");
	ComponentManager::GetInstance()->registerComponent<DynamicCamController>("DynamicCamController");

	// ---Events registering

	// generic
	InterfaceSystem::GetInstance()->registerEvent("backButtonClick", UIEvent("ButtonClicked", &backButtonClick));

	// mainMenu
	InterfaceSystem::GetInstance()->registerEvent("singlePlayerButtonClick", UIEvent("ButtonClicked", &singlePlayerButtonClick));
	InterfaceSystem::GetInstance()->registerEvent("multiplayerButtonClick", UIEvent("ButtonClicked", &multiplayerButtonClick));
	InterfaceSystem::GetInstance()->registerEvent("optionsButtonClick", UIEvent("ButtonClicked", &optionsButtonClick));
	InterfaceSystem::GetInstance()->registerEvent("exitButtonClick", UIEvent("ButtonClicked", &exitButtonClick));

	// fightConfigMenu
	//InterfaceSystem::GetInstance()->registerEvent("fightButtonClick", UIEvent("ButtonClicked", &fightButtonClick));

	// pauseMenu
	InterfaceSystem::GetInstance()->registerEvent("resumeButtonClick", UIEvent("ButtonClicked", &resumeButtonClick));


	GaiaCore* g = new GaiaCore();
	g->init();
	g->run();
	g->close();
	
	delete g;

	return 0;
}