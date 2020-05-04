#include "PauseMenu.h"

#include <InputSystem.h>
#include <InterfaceSystem.h>
#include <GameObject.h>

#include "GameManager.h"
#include "UILayout.h"

#include <SceneManager.h>

#include "ComponentRegister.h"
#include "GameManager.h"

REGISTER_FACTORY(PauseMenu);

bool PauseMenu::backButtonClick()
{
	GameManager::GetInstance()->pauseGame(false);
	SceneManager::GetInstance()->changeScene("mainMenu");
	return false;
}

bool PauseMenu::optionsButton()
{
	pauseMenu.setVisible(false);
	pauseMenu.setAlwaysOnTop(false);
	optionsMenu.setVisible(true);
	optionsMenu.setAlwaysOnTop(true);
	optionsMenu.setEnabled(true);

	return false;
}

PauseMenu::PauseMenu(GameObject* gameObject) : UserComponent(gameObject), pauseMenu(NULL),optionsMenu(NULL)
{
	InterfaceSystem::GetInstance()->registerEvent("resumeButtonClick", UIEvent("ButtonClicked", [this]() {setPaused(false); return false;}));
	InterfaceSystem::GetInstance()->registerEvent("pauseBackButtonClick", UIEvent("ButtonClicked", [this]() {return backButtonClick(); }));
	InterfaceSystem::GetInstance()->registerEvent("pauseOptionsButtonClick", UIEvent("ButtonClicked", [this]() {optionsButton(); return false; }));
}

PauseMenu::~PauseMenu()
{
	InterfaceSystem::GetInstance()->unregisterEvent("backButtonClick");
	InterfaceSystem::GetInstance()->unregisterEvent("resumeButtonClick");
	InterfaceSystem::GetInstance()->unregisterEvent("pauseOptionsButtonClick");
}

void PauseMenu::start()
{
	UILayout* cameraLayout = findGameObjectWithName("MainCamera")->getComponent<UILayout>();
	optionsMenu = findGameObjectWithName("OptionsMenuScreen")->getComponent<UILayout>()->getRoot();
	if (cameraLayout != nullptr)
		pauseMenu = cameraLayout->getRoot().getChild("PauseBackground");

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