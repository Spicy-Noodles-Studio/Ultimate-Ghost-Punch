#include "PauseMenu.h"
#include <ComponentRegister.h>
#include <InputSystem.h>
#include <InterfaceSystem.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <UILayout.h>
#include <SoundEmitter.h>

#include "GameManager.h"

REGISTER_FACTORY(PauseMenu);

bool PauseMenu::backButtonClick()
{
	GameManager::GetInstance()->pauseGame(false);
	return Menu::backButtonClick();
}

bool PauseMenu::optionsButton()
{
	pauseMenu.setVisible(false);
	pauseMenu.setAlwaysOnTop(false);

	optionsMenu.setVisible(true);
	optionsMenu.setAlwaysOnTop(true);
	optionsMenu.setEnabled(true);

	InterfaceSystem::GetInstance()->clearControllerMenuInput();
	InterfaceSystem::GetInstance()->initControllerMenuInput(&optionsMenu);

	buttonClick(buttonSound);

	return false;
}

bool PauseMenu::resumeButton()
{
	setPaused(false);
	buttonClick(buttonSound);
	return false;
}

PauseMenu::PauseMenu(GameObject* gameObject) : Menu(gameObject), inputSystem(nullptr), pauseMenu(NULL), optionsMenu(NULL)
{
	InterfaceSystem::GetInstance()->registerEvent("resumeButtonClick", UIEvent("ButtonClicked", [this]() {; return resumeButton(); }));
	InterfaceSystem::GetInstance()->registerEvent("pauseBackButtonClick", UIEvent("ButtonClicked", [this]() {return backButtonClick(); }));
	InterfaceSystem::GetInstance()->registerEvent("pauseOptionsButtonClick", UIEvent("ButtonClicked", [this]() {optionsButton(); return false; }));
}

PauseMenu::~PauseMenu()
{
	InterfaceSystem::GetInstance()->unregisterEvent("resumeButtonClick");
	InterfaceSystem::GetInstance()->unregisterEvent("pauseBackButtonClick");
	InterfaceSystem::GetInstance()->unregisterEvent("pauseOptionsButtonClick");
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

	inputSystem = InputSystem::GetInstance();
}

void PauseMenu::preUpdate(float deltaTime)
{
	if (inputSystem->getKeyPress("ESCAPE"))
		setPaused(!GameManager::GetInstance()->gameIsPaused());
}

void PauseMenu::setPaused(bool paused)
{
	if (paused == GameManager::GetInstance()->gameIsPaused()) return;

	pauseMenu.setVisible(paused);
	pauseMenu.setAlwaysOnTop(paused);
	GameManager::GetInstance()->pauseGame(paused);
}

bool PauseMenu::isVisible()
{
	return pauseMenu.isVisible();
}