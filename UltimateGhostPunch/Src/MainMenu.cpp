#include "MainMenu.h"
#include <InterfaceSystem.h>
#include <RenderSystem.h>
#include <SceneManager.h>
#include <GameObject.h>

#include "GameManager.h"

#include <ComponentRegister.h>

REGISTER_FACTORY(MainMenu);

bool MainMenu::singlePlayerButtonClick()
{
	SceneManager::GetInstance()->changeScene("mainScene");
	return false;
}

bool MainMenu::multiplayerButtonClick()
{
	SceneManager::GetInstance()->changeScene("fightConfig");
	return false;
}

bool MainMenu::optionsButtonClick()
{
	SceneManager::GetInstance()->changeScene("options");
	return false;
}

bool MainMenu::exitButtonClick()
{
	RenderSystem::GetInstance()->closeWindow();
	return false;
}

MainMenu::MainMenu(GameObject* gameObject) : UserComponent(gameObject)
{
	InterfaceSystem* interfaceSystem = InterfaceSystem::GetInstance();

	interfaceSystem->registerEvent("singlePlayerButtonClick", UIEvent("ButtonClicked", [this]() {return singlePlayerButtonClick(); }));
	interfaceSystem->registerEvent("multiplayerButtonClick", UIEvent("ButtonClicked", [this]() {return multiplayerButtonClick(); }));
	interfaceSystem->registerEvent("optionsButtonClick", UIEvent("ButtonClicked", [this]() {return optionsButtonClick(); }));
	interfaceSystem->registerEvent("exitButtonClick", UIEvent("ButtonClicked", [this]() {return exitButtonClick(); }));
}

MainMenu::~MainMenu()
{
	InterfaceSystem* interfaceSystem = InterfaceSystem::GetInstance();

	interfaceSystem->unregisterEvent("singlePlayerButtonClick");
	interfaceSystem->unregisterEvent("multiplayerButtonClick");
	interfaceSystem->unregisterEvent("optionsButtonClick");
	interfaceSystem->unregisterEvent("exitButtonClick");
}