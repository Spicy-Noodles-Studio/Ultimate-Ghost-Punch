#include "MainMenu.h"
#include <ComponentRegister.h>
#include <InterfaceSystem.h>
#include <RenderSystem.h>
#include <SceneManager.h>
#include <WindowManager.h>
#include <GameObject.h>

#include "SongManager.h"

REGISTER_FACTORY(MainMenu);

bool MainMenu::playButtonClick()
{
	buttonClick(buttonSound);
	if (sceneManager != nullptr)
		sceneManager->changeScene("ConfigurationMenu");
	return false;
}

bool MainMenu::optionsButtonClick()
{
	buttonClick(buttonSound);
	if (sceneManager != nullptr)
		sceneManager->changeScene("OptionsMenu");
	return false;
}

bool MainMenu::exitButtonClick()
{
	buttonClick(backSound);
	if (windowManager != nullptr)
		windowManager->closeWindow();
	return false;
}

bool MainMenu::controlsButtonClick()
{
	buttonClick(buttonSound);
	if (sceneManager != nullptr)
		sceneManager->changeScene("ControlsMenu");
	return false;
}

bool MainMenu::creditsButtonClick()
{
	buttonClick(buttonSound);
	if (sceneManager != nullptr)
		sceneManager->changeScene("Credits");
	return false;
}

MainMenu::MainMenu(GameObject* gameObject) : Menu(gameObject)
{
	if (interfaceSystem != nullptr) {
		interfaceSystem->registerEvent("playButtonClick", UIEvent("ButtonClicked", [this]() {return playButtonClick(); }));
		interfaceSystem->registerEvent("optionsButtonClick", UIEvent("ButtonClicked", [this]() {return optionsButtonClick(); }));
		interfaceSystem->registerEvent("exitButtonClick", UIEvent("ButtonClicked", [this]() {return exitButtonClick(); }));

		interfaceSystem->registerEvent("controlsButtonClick", UIEvent("ButtonClicked", [this]() {return controlsButtonClick(); }));
		interfaceSystem->registerEvent("creditsButtonClick", UIEvent("ButtonClicked", [this]() {return creditsButtonClick(); }));
	}
}

MainMenu::~MainMenu()
{
	if (interfaceSystem != nullptr) {
		interfaceSystem->unregisterEvent("playButtonClick");
		interfaceSystem->unregisterEvent("optionsButtonClick");
		interfaceSystem->unregisterEvent("exitButtonClick");

		interfaceSystem->unregisterEvent("controlsButtonClick");
		interfaceSystem->unregisterEvent("creditsButtonClick");
	}
}

void MainMenu::start()
{
	Menu::start();

	windowManager = WindowManager::GetInstance();

	if (songManager != nullptr)
		songManager->playMenuSong();
}