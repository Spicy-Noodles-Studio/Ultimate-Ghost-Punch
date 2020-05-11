#include "PauseMenu.h"
#include <ComponentRegister.h>
#include <InputSystem.h>
#include <InterfaceSystem.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <UILayout.h>

#include "GameManager.h"

REGISTER_FACTORY(PauseMenu);

bool PauseMenu::optionsButtonClick()
{
	pauseMenu.setVisible(false);
	pauseMenu.setAlwaysOnTop(false);

	optionsMenu.setVisible(true);
	optionsMenu.setAlwaysOnTop(true);
	optionsMenu.setEnabled(true);

	InterfaceSystem::GetInstance()->clearControllerMenuInput();
	InterfaceSystem::GetInstance()->initControllerMenuInput(&optionsMenu);

	return false;
}

bool PauseMenu::exitButtonClick()
{
	GameManager::GetInstance()->setPaused(false);
	SceneManager::GetInstance()->changeScene("MainMenu");

	return false;
}

PauseMenu::PauseMenu(GameObject* gameObject) : UserComponent(gameObject), inputSystem(nullptr), pauseMenu(NULL), pauseText(NULL), optionsMenu(NULL)
{
	inputSystem = InputSystem::GetInstance();

	InterfaceSystem::GetInstance()->registerEvent("resumeButtonClick", UIEvent("ButtonClicked", [this]() {setPaused(false); return false; }));
	InterfaceSystem::GetInstance()->registerEvent("pauseOptionsButtonClick", UIEvent("ButtonClicked", [this]() {optionsButtonClick(); return false; }));
	InterfaceSystem::GetInstance()->registerEvent("pauseExitButtonClick", UIEvent("ButtonClicked", [this]() {return exitButtonClick(); }));
}

PauseMenu::~PauseMenu()
{
	InterfaceSystem::GetInstance()->unregisterEvent("resumeButtonClick");
	InterfaceSystem::GetInstance()->unregisterEvent("pauseOptionsButtonClick");
	InterfaceSystem::GetInstance()->unregisterEvent("pauseExitButtonClick");
}

void PauseMenu::start()
{
	UILayout* cameraLayout = findGameObjectWithName("MainCamera")->getComponent<UILayout>();
	optionsMenu = findGameObjectWithName("OptionsMenuScreen")->getComponent<UILayout>()->getRoot();

	if (cameraLayout != nullptr)
	{
		pauseMenu = cameraLayout->getRoot().getChild("PauseBackground");
		pauseText = cameraLayout->getRoot().getChild("PauseText");
	}
}

void PauseMenu::preUpdate(float deltaTime)
{
	if ((inputSystem->getKeyPress("ESCAPE") || checkControllersInput()) && !optionsMenu.isVisible())
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
	if (paused == GameManager::GetInstance()->isPaused())
		return;

	pauseMenu.setVisible(paused);
	pauseMenu.setAlwaysOnTop(paused);

	pauseText.setVisible(paused);

	GameManager::GetInstance()->setPaused(paused);
}

bool PauseMenu::isVisible()
{
	return pauseMenu.isVisible();
}