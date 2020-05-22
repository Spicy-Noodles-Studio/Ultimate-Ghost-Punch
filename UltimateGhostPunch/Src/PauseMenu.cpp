#include "PauseMenu.h"
#include <ComponentRegister.h>
#include <InputSystem.h>
#include <InterfaceSystem.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <UILayout.h>

#include "GameManager.h"
#include "SongManager.h"
#include "Countdown.h"

REGISTER_FACTORY(PauseMenu);

bool PauseMenu::resumeButtonClick()
{
	setPaused(false);
	buttonClick(buttonSound);
	return false;
}

bool PauseMenu::optionsButtonClick()
{
	pauseMenu.setVisible(false);
	pauseMenu.setAlwaysOnTop(false);

	optionsMenu.setVisible(true);
	optionsMenu.setAlwaysOnTop(true);
	optionsMenu.setEnabled(true);

	if (interfaceSystem != nullptr) {
		interfaceSystem->clearControllerMenuInput();
		interfaceSystem->initControllerMenuInput(&optionsMenu);
	}

	buttonClick(buttonSound);

	return false;
}

bool PauseMenu::exitButtonClick()
{
	if (gameManager != nullptr) {
		gameManager->setPaused(false);
		gameManager->pauseAllSounds();
		gameManager->emptyKnights();
		if (songManager != nullptr)
			songManager->stopSong(gameManager->getSong().first);
	}

	return Menu::backButtonClick();
}

PauseMenu::PauseMenu(GameObject* gameObject) : Menu(gameObject), countdown(nullptr), pauseMenu(NULL), pausePanel(NULL), optionsMenu(NULL)
{
	if (interfaceSystem != nullptr) {
		interfaceSystem->registerEvent("resumeButtonClick", UIEvent("ButtonClicked", [this]() {setPaused(false); return false; }));
		interfaceSystem->registerEvent("pauseOptionsButtonClick", UIEvent("ButtonClicked", [this]() {optionsButtonClick(); return false; }));
		interfaceSystem->registerEvent("pauseExitButtonClick", UIEvent("ButtonClicked", [this]() {return exitButtonClick(); }));
	}
}

PauseMenu::~PauseMenu()
{
	if (interfaceSystem != nullptr) {
		interfaceSystem->unregisterEvent("resumeButtonClick");
		interfaceSystem->unregisterEvent("pauseOptionsButtonClick");
		interfaceSystem->unregisterEvent("pauseExitButtonClick");
	}
}

void PauseMenu::start()
{
	Menu::start();

	if (mainCamera != nullptr)
	{
		UILayout* cameraLayout = mainCamera->getComponent<UILayout>();
		if (cameraLayout != nullptr)
		{
			pauseMenu = cameraLayout->getRoot().getChild("PauseBackground");
			pausePanel = cameraLayout->getRoot().getChild("Pause");
		}
	}

	GameObject* options = findGameObjectWithName("OptionsMenuScreen");
	if (options != nullptr) {
		UILayout* optionsLayout = options->getComponent<UILayout>();
		if (optionsLayout != nullptr)
			optionsMenu = optionsLayout->getRoot();
	}

	GameObject* countdownObject = findGameObjectWithName("Countdown");
	if (countdownObject != nullptr)
		countdown = countdownObject->getComponent<Countdown>();
}

void PauseMenu::preUpdate(float deltaTime)
{
	if (countdown != nullptr && countdown->hasStarted() && !countdown->isCounting() && gameManager != nullptr && inputSystem != nullptr && (inputSystem->getKeyPress("ESCAPE") || checkControllersInput()) && !optionsMenu.isVisible())
		setPaused(!gameManager->isPaused());
}

bool PauseMenu::checkControllersInput()
{
	bool result = false;

	int i = 0;
	while (i < 4 && !result)
	{
		if (inputSystem != nullptr && inputSystem->getButtonPress(i, "START") || (inputSystem->getButtonPress(i, "B") && pauseMenu.isVisible()))
			result = true;

		i++;
	}

	return result;
}

void PauseMenu::setPaused(bool paused)
{
	if (gameManager == nullptr || songManager == nullptr) return;

	if (paused == gameManager->isPaused()) return;

	if (paused)	songManager->pauseSong(gameManager->getSong().first);
	else songManager->resumeSong(gameManager->getSong().first);

	pauseMenu.setVisible(paused);
	pauseMenu.setAlwaysOnTop(paused);
	pausePanel.setVisible(paused);
	pausePanel.setAlwaysOnTop(paused);

	gameManager->setPaused(paused);
}

bool PauseMenu::isVisible()
{
	return pauseMenu.isVisible();
}