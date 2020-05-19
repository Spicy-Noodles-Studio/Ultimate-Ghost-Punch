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

	interfaceSystem->clearControllerMenuInput();
	interfaceSystem->initControllerMenuInput(&optionsMenu);

	buttonClick(buttonSound);

	return false;
}

bool PauseMenu::exitButtonClick()
{
	gameManager->setPaused(false);
	gameManager->pauseAllSounds();
	gameManager->emptyKnights();

	songManager->stopSong(gameManager->getSong().first);

	return Menu::backButtonClick();
}

PauseMenu::PauseMenu(GameObject* gameObject) : Menu(gameObject), countdown(nullptr), pauseMenu(NULL), pausePanel(NULL), optionsMenu(NULL)
{
	interfaceSystem->registerEvent("resumeButtonClick", UIEvent("ButtonClicked", [this]() {setPaused(false); return false; }));
	interfaceSystem->registerEvent("pauseOptionsButtonClick", UIEvent("ButtonClicked", [this]() {optionsButtonClick(); return false; }));
	interfaceSystem->registerEvent("pauseExitButtonClick", UIEvent("ButtonClicked", [this]() {return exitButtonClick(); }));
}

PauseMenu::~PauseMenu()
{
	interfaceSystem->unregisterEvent("resumeButtonClick");
	interfaceSystem->unregisterEvent("pauseOptionsButtonClick");
	interfaceSystem->unregisterEvent("pauseExitButtonClick");
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
	if (options != nullptr)
		optionsMenu = options->getComponent<UILayout>()->getRoot();

	countdown = findGameObjectWithName("Countdown")->getComponent<Countdown>();
}

void PauseMenu::preUpdate(float deltaTime)
{
	if (!countdown->isCounting() && (inputSystem->getKeyPress("ESCAPE") || checkControllersInput()) && !optionsMenu.isVisible())
		setPaused(!GameManager::GetInstance()->isPaused());
}

bool PauseMenu::checkControllersInput()
{
	bool result = false;

	int i = 0;
	while (i < 4 && !result)
	{
		if (inputSystem->getButtonPress(i, "START") || (inputSystem->getButtonPress(i, "B") && pauseMenu.isVisible()))
			result = true;

		i++;
	}

	return result;
}

void PauseMenu::setPaused(bool paused)
{
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