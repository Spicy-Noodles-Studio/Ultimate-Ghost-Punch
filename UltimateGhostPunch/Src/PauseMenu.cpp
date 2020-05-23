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

	if (notNull(interfaceSystem)) {
		interfaceSystem->clearControllerMenuInput();
		interfaceSystem->initControllerMenuInput(&optionsMenu);
	}

	buttonClick(buttonSound);

	return false;
}

bool PauseMenu::exitButtonClick()
{
	if (notNull(gameManager)) {
		gameManager->setPaused(false);
		gameManager->pauseAllSounds();
		gameManager->emptyKnights();
		if (notNull(songManager))
			songManager->stopSong(gameManager->getSong().first);
	}

	return Menu::backButtonClick();
}

PauseMenu::PauseMenu(GameObject* gameObject) : Menu(gameObject), countdown(nullptr), pauseMenu(NULL), pausePanel(NULL), optionsMenu(NULL)
{
	if (notNull(interfaceSystem)) {
		interfaceSystem->registerEvent("resumeButtonClick", UIEvent("ButtonClicked", [this]() {setPaused(false); return false; }));
		interfaceSystem->registerEvent("pauseOptionsButtonClick", UIEvent("ButtonClicked", [this]() {optionsButtonClick(); return false; }));
		interfaceSystem->registerEvent("pauseExitButtonClick", UIEvent("ButtonClicked", [this]() {return exitButtonClick(); }));
	}
}

PauseMenu::~PauseMenu()
{
	if (notNull(interfaceSystem)) {
		interfaceSystem->unregisterEvent("resumeButtonClick");
		interfaceSystem->unregisterEvent("pauseOptionsButtonClick");
		interfaceSystem->unregisterEvent("pauseExitButtonClick");
	}
}

void PauseMenu::start()
{
	Menu::start();

	if (notNull(mainCamera))
	{
		UILayout* cameraLayout = mainCamera->getComponent<UILayout>();
		if (notNull(cameraLayout))
		{
			pauseMenu = cameraLayout->getRoot().getChild("PauseBackground");
			pausePanel = cameraLayout->getRoot().getChild("Pause");
		}
	}

	GameObject* options = findGameObjectWithName("OptionsMenuScreen");
	if (notNull(options)) {
		UILayout* optionsLayout = options->getComponent<UILayout>();
		if (notNull(optionsLayout))
			optionsMenu = optionsLayout->getRoot();
	}

	GameObject* countdownObject = findGameObjectWithName("Countdown");
	if (notNull(countdownObject))
		countdown = countdownObject->getComponent<Countdown>();
}

void PauseMenu::preUpdate(float deltaTime)
{
	if (notNull(countdown) && countdown->hasStarted() && !countdown->isCounting() && notNull(gameManager) && notNull(inputSystem) && (inputSystem->getKeyPress("ESCAPE") || checkControllersInput()) && !optionsMenu.isVisible())
		setPaused(!gameManager->isPaused());
}

bool PauseMenu::checkControllersInput()
{
	bool result = false;

	int i = 0;
	while (i < 4 && !result)
	{
		if (notNull(inputSystem) && inputSystem->getButtonPress(i, "START") || (inputSystem->getButtonPress(i, "B") && pauseMenu.isVisible()))
			result = true;

		i++;
	}

	return result;
}

void PauseMenu::setPaused(bool paused)
{
	checkNullAndBreak(gameManager);
	checkNullAndBreak(songManager);

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