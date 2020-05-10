#include "PauseMenu.h"
#include <ComponentRegister.h>
#include <InputSystem.h>
#include <InterfaceSystem.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <UILayout.h>

#include "GameManager.h"
#include "SongManager.h"

REGISTER_FACTORY(PauseMenu);

bool PauseMenu::backButtonClick()
{
	gameManager->pauseGame(false);
	songManager->stopSong(gameManager->getSong());

	return Menu::backButtonClick();
}

bool PauseMenu::optionsButton()
{
	pauseMenu.setVisible(false);
	pauseMenu.setAlwaysOnTop(false);

	optionsMenu.setVisible(true);
	optionsMenu.setAlwaysOnTop(true);
	optionsMenu.setEnabled(true);

	interfaceSystem->clearControllerMenuInput();
	interfaceSystem->initControllerMenuInput(&optionsMenu);

	buttonClick(buttonSound);

	return false;
}

bool PauseMenu::resumeButton()
{
	setPaused(false);
	buttonClick(buttonSound);
	return false;
}

PauseMenu::PauseMenu(GameObject* gameObject) : Menu(gameObject), pauseMenu(NULL), optionsMenu(NULL)
{
	interfaceSystem->registerEvent("resumeButtonClick", UIEvent("ButtonClicked", [this]() {; return resumeButton(); }));
	interfaceSystem->registerEvent("pauseBackButtonClick", UIEvent("ButtonClicked", [this]() {return backButtonClick(); }));
	interfaceSystem->registerEvent("pauseOptionsButtonClick", UIEvent("ButtonClicked", [this]() {optionsButton(); return false; }));
}

PauseMenu::~PauseMenu()
{
	interfaceSystem->unregisterEvent("resumeButtonClick");
	interfaceSystem->unregisterEvent("pauseBackButtonClick");
	interfaceSystem->unregisterEvent("pauseOptionsButtonClick");
}

void PauseMenu::start()
{
	Menu::start();

	if (mainCamera != nullptr) {
		UILayout* cameraLayout = mainCamera->getComponent<UILayout>();
		if (cameraLayout != nullptr)
			pauseMenu = cameraLayout->getRoot().getChild("PauseBackground");
	}

	GameObject* options = findGameObjectWithName("OptionsMenuScreen");
	if(options!=nullptr) optionsMenu = options->getComponent<UILayout>()->getRoot();
}

void PauseMenu::preUpdate(float deltaTime)
{
	if (inputSystem->getKeyPress("ESCAPE"))
		setPaused(!gameManager->gameIsPaused());
}

void PauseMenu::setPaused(bool paused)
{
	if (paused == gameManager->gameIsPaused()) return;

	if (paused)	songManager->pauseSong(gameManager->getSong());
	else songManager->resumeSong(gameManager->getSong());

	pauseMenu.setVisible(paused);
	pauseMenu.setAlwaysOnTop(paused);
	gameManager->pauseGame(paused);
}

bool PauseMenu::isVisible()
{
	return pauseMenu.isVisible();
}