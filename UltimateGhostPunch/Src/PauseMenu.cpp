#include "PauseMenu.h"
#include <ComponentRegister.h>
#include <InputSystem.h>
#include <InterfaceSystem.h>
#include <SceneManager.h>
#include <GameObject.h>

#include "GameManager.h"
#include "UILayout.h"

REGISTER_FACTORY(PauseMenu);

bool PauseMenu::backButtonClick()
{
	GameManager::GetInstance()->pauseGame(false);
	SceneManager::GetInstance()->changeScene("mainMenu");
	return false;
}

PauseMenu::PauseMenu(GameObject* gameObject) : UserComponent(gameObject), inputSystem(nullptr), pauseMenu(NULL)
{
	InterfaceSystem::GetInstance()->registerEvent("resumeButtonClick", UIEvent("ButtonClicked", [this]() {setPaused(false); return false; }));
	InterfaceSystem::GetInstance()->registerEvent("pauseBackButtonClick", UIEvent("ButtonClicked", [this]() {return backButtonClick(); }));
}

PauseMenu::~PauseMenu()
{
	InterfaceSystem::GetInstance()->unregisterEvent("backButtonClick");
	InterfaceSystem::GetInstance()->unregisterEvent("resumeButtonClick");
}

void PauseMenu::start()
{
	UILayout* cameraLayout = findGameObjectWithName("MainCamera")->getComponent<UILayout>();

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
	GameManager::GetInstance()->pauseGame(paused);
}

bool PauseMenu::isVisible()
{
	return pauseMenu.isVisible();
}