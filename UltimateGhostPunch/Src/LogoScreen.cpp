#include "LogoScreen.h"
#include <ComponentRegister.h>
#include <InputSystem.h>
#include <InterfaceSystem.h>
#include <SceneManager.h>

REGISTER_FACTORY(LogoScreen);

bool LogoScreen::checkControllersInput()
{
	bool result = false;

	int i = 0;
	while (i < 4 && !result)
	{
		if (inputSystem != nullptr && inputSystem->getButtonPress(i, "A") || inputSystem->getButtonPress(i, "START"))
			result = true;

		i++;
	}

	return result;
}

LogoScreen::LogoScreen(GameObject* gameObject) : Menu(gameObject), time(3)
{

}

LogoScreen::~LogoScreen()
{

}

void LogoScreen::start()
{
	Menu::start();
}

void LogoScreen::update(float deltaTime)
{
	if ((inputSystem != nullptr && inputSystem->getKeyPress("ESCAPE") || inputSystem->getMouseButtonClick('l')) || checkControllersInput() || time <= 0)
	{
		if (sceneManager->getCurrentScene()->getName() == "EngineLogo")
			sceneManager->changeScene("StudioLogo");
		else
			sceneManager->changeScene("MainMenu");
	}

	time -= deltaTime;
}