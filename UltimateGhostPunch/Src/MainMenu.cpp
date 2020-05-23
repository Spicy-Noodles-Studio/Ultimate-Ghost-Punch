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
	if (notNull(sceneManager))
		sceneManager->changeScene("ConfigurationMenu");
	return false;
}

bool MainMenu::optionsButtonClick()
{
	buttonClick(buttonSound);
	if (notNull(sceneManager))
		sceneManager->changeScene("OptionsMenu");
	return false;
}

bool MainMenu::exitButtonClick()
{
	buttonClick(backSound);
	if (notNull(windowManager))
		windowManager->closeWindow();
	return false;
}

bool MainMenu::controlsButtonClick()
{
	buttonClick(buttonSound);
	if (notNull(sceneManager))
		sceneManager->changeScene("ControlsMenu");
	return false;
}

bool MainMenu::creditsButtonClick()
{
	buttonClick(buttonSound);
	if (notNull(sceneManager))
		sceneManager->changeScene("Credits");
	return false;
}

MainMenu::MainMenu(GameObject* gameObject) : Menu(gameObject)
{
	if (notNull(interfaceSystem)) {
		interfaceSystem->registerEvent("playButtonClick", UIEvent("ButtonClicked", [this]() {return playButtonClick(); }));
		interfaceSystem->registerEvent("optionsButtonClick", UIEvent("ButtonClicked", [this]() {return optionsButtonClick(); }));
		interfaceSystem->registerEvent("exitButtonClick", UIEvent("ButtonClicked", [this]() {return exitButtonClick(); }));

		interfaceSystem->registerEvent("controlsButtonClick", UIEvent("ButtonClicked", [this]() {return controlsButtonClick(); }));
		interfaceSystem->registerEvent("creditsButtonClick", UIEvent("ButtonClicked", [this]() {return creditsButtonClick(); }));
	}
}

MainMenu::~MainMenu()
{
	if (notNull(interfaceSystem)) {
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
	checkNull(windowManager);

	if (notNull(songManager))
		songManager->playMenuSong();
}