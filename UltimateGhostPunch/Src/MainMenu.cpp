#include "MainMenu.h"
#include <ComponentRegister.h>
#include <InterfaceSystem.h>
#include <RenderSystem.h>
#include <SceneManager.h>
#include <WindowManager.h>
#include <GameObject.h>

#include "GameManager.h"

REGISTER_FACTORY(MainMenu);

bool MainMenu::playButtonClick()
{
	SceneManager::GetInstance()->changeScene("ConfigurationMenu");
	return false;
}

bool MainMenu::optionsButtonClick()
{
	SceneManager::GetInstance()->changeScene("OptionsMenu");
	return false;
}

bool MainMenu::exitButtonClick()
{
	WindowManager::GetInstance()->closeWindow();
	return false;
}

MainMenu::MainMenu(GameObject* gameObject) : UserComponent(gameObject), inputSystem(nullptr)
{
	InterfaceSystem* interfaceSystem = InterfaceSystem::GetInstance();

	interfaceSystem->registerEvent("playButtonClick", UIEvent("ButtonClicked", [this]() {return playButtonClick(); }));
	interfaceSystem->registerEvent("optionsButtonClick", UIEvent("ButtonClicked", [this]() {return optionsButtonClick(); }));
	interfaceSystem->registerEvent("exitButtonClick", UIEvent("ButtonClicked", [this]() {return exitButtonClick(); }));
}

MainMenu::~MainMenu()
{
	InterfaceSystem* interfaceSystem = InterfaceSystem::GetInstance();

	interfaceSystem->unregisterEvent("playButtonClick");
	interfaceSystem->unregisterEvent("optionsButtonClick");
	interfaceSystem->unregisterEvent("exitButtonClick");
}