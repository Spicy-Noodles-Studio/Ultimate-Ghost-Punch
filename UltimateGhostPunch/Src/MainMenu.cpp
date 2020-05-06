#include "MainMenu.h"
#include <ComponentRegister.h>
#include <InterfaceSystem.h>
#include <RenderSystem.h>
#include <SceneManager.h>
#include <WindowManager.h>
#include <GameObject.h>
#include <SoundEmitter.h>

#include "GameManager.h"

REGISTER_FACTORY(MainMenu);

bool MainMenu::singlePlayerButtonClick()
{
	SceneManager::GetInstance()->changeScene("ConfigurationMenu");
	buttonClick("button4");
	return false;
}

bool MainMenu::multiplayerButtonClick()
{
	SceneManager::GetInstance()->changeScene("ConfigurationMenu");
	buttonClick("button4");
	return false;
}

bool MainMenu::optionsButtonClick()
{
	SceneManager::GetInstance()->changeScene("OptionsMenu");
	buttonClick("button4");
	return false;
}

bool MainMenu::exitButtonClick()
{
	WindowManager::GetInstance()->closeWindow();
	buttonClick("back");
	return false;
}

void MainMenu::buttonClick(const std::string& sound)
{
	if (soundEmitter != nullptr) soundEmitter->playSound(sound);
}

MainMenu::MainMenu(GameObject* gameObject) : UserComponent(gameObject), inputSystem(nullptr)
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

void MainMenu::start()
{
	GameObject* mainCamera = findGameObjectWithName("MainCamera");
	if (mainCamera != nullptr)
		soundEmitter = mainCamera->getComponent<SoundEmitter>();
}
