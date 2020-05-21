#include "Menu.h"

#include <ComponentRegister.h>
#include <SoundEmitter.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <InputSystem.h>
#include <InterfaceSystem.h>
#include <SceneManager.h>

#include "GameManager.h"
#include "SongManager.h"

Menu::Menu(GameObject* gameObject) : UserComponent(gameObject), songManager(nullptr), mainCamera(nullptr), gameManager(nullptr), interfaceSystem(nullptr), inputSystem(nullptr), sceneManager(nullptr)
{
	interfaceSystem = InterfaceSystem::GetInstance();
}

Menu::~Menu()
{
}

void Menu::start()
{
	inputSystem = InputSystem::GetInstance();
	gameManager = GameManager::GetInstance();
	songManager = SongManager::GetInstance();
	sceneManager = SceneManager::GetInstance();

	mainCamera = findGameObjectWithName("MainCamera");

	checkNull(inputSystem);
	checkNull(gameManager);
	checkNull(interfaceSystem);
	checkNull(songManager);
	checkNull(mainCamera);
	checkNull(sceneManager);
}

void Menu::buttonClick(const std::string& sound) {
	if (songManager != nullptr)
		songManager->play2DSound(sound);
}

bool Menu::backButtonClick()
{
	buttonClick(backSound);
	if (sceneManager != nullptr)
		sceneManager->changeScene("MainMenu");
	return false;
}
