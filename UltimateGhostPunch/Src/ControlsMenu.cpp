#include "ControlsMenu.h"
#include <ComponentRegister.h>
#include <InputSystem.h>
#include <InterfaceSystem.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <UILayout.h>

REGISTER_FACTORY(ControlsMenu);

bool ControlsMenu::checkControllersInput()
{
	bool result = false;

	int i = 0;
	while (i < 4 && !result)
	{
		if (inputSystem != nullptr && inputSystem->getButtonPress(i, "B"))
			result = true;

		i++;
	}

	return result;
}

ControlsMenu::ControlsMenu(GameObject* gameObject) : Menu(gameObject)
{
	Menu::start();

	if (interfaceSystem != nullptr)
		interfaceSystem->registerEvent("backButtonClick", UIEvent("ButtonClicked", [this]() {return backButtonClick(); }));


	actions = { "ACTIONS\n\n", "Movement:\n\n", "Quick Attack:\n\n", "Strong Attack:\n\n", "Jump:\n\n", "Grab:\n\n", "Block:\n\n", "Dash:\n\n", "Ghost Punch:" };

	keyboard = { "KEYBOARD\n\n", "A/D\n\n", "Left Click\n\n", "Right Click\n\n", "SPACE/W\n\n", "E\n\n", "S\n\n", "Shift + A/D\n\n", "Aim with mouse + Left Click" };

	controller = { "CONTROLLER\n\n", "Left Joystick/Pads\n\n", "X\n\n", "Y\n\n", "A/UP\n\n", "LB\n\n", "B\n\n", "RB\n\n", "Aim with Right Joystick + RB" };
}

ControlsMenu::~ControlsMenu()
{
	if (interfaceSystem != nullptr)
		interfaceSystem->unregisterEvent("backButtonClick");
}

void ControlsMenu::start()
{
	GameObject* camera = findGameObjectWithName("MainCamera");
	if (camera != nullptr)
	{
		UILayout* layout = camera->getComponent<UILayout>();
		if (layout != nullptr)
		{
			std::string aux1 = "";
			std::string aux2 = "";
			std::string aux3 = "";

			for (int i = 0; i < actions.size(); i++)
			{
				aux1 += actions[i];
				aux2 += keyboard[i];
				aux3 += controller[i];
			}

			layout->getRoot().getChild("Background").getChild("Actions").setText(aux1);
			layout->getRoot().getChild("Background").getChild("Keyboard").setText(aux2);
			layout->getRoot().getChild("Background").getChild("Controller").setText(aux3);
		}
	}
}

void ControlsMenu::update(float deltaTime)
{
	if ((inputSystem != nullptr && inputSystem->getKeyPress("ESCAPE")) || checkControllersInput())
		backButtonClick();
}