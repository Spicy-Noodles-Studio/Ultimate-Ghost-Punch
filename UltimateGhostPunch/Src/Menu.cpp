#include "Menu.h"

#include <ComponentRegister.h>
#include <SoundEmitter.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <InputSystem.h>
#include <InterfaceSystem.h>

#include "GameManager.h"
#include "SongManager.h"

Menu::Menu(GameObject* gameObject) : UserComponent(gameObject), songManager(nullptr), mainCamera(nullptr), gameManager(nullptr), interfaceSystem(nullptr), inputSystem(nullptr)
{
}

Menu::~Menu()
{
}

void Menu::start()
{
	inputSystem = InputSystem::GetInstance();
	gameManager = GameManager::GetInstance();
	interfaceSystem = InterfaceSystem::GetInstance();
	songManager = SongManager::GetInstance();

	mainCamera = findGameObjectWithName("MainCamera");
}

void Menu::buttonClick(const std::string& sound) {
	songManager->play2DSound(sound);
}

bool Menu::backButtonClick()
{
	SceneManager::GetInstance()->changeScene("MainMenu");
	buttonClick(backSound);
	return false;
}
