#include "PlayerController.h"
#include <InputSystem.h>
#include <sstream>

PlayerController::PlayerController(GameObject* gameObject) : UserComponent(gameObject)
{

}

void PlayerController::start()
{
	movement = gameObject->getComponent<Movement>();
}

void PlayerController::update(float deltaTime)
{
	UserComponent::update(deltaTime);

	Vector3 dir = Vector3(0, 0, 0);

	if (usingKeyboard)
	{
		if (InputSystem::GetInstance()->isKeyPressed("A"))
			dir = Vector3(-1, 0, 0);
		else if (InputSystem::GetInstance()->isKeyPressed("D"))
			dir = Vector3(1, 0, 0);
	}
	else
	{
		if (InputSystem::GetInstance()->getLeftJoystick(playerIndex).first < 0 || InputSystem::GetInstance()->isButtonPressed(playerIndex, "Left"))
			dir = Vector3(-1, 0, 0);
		else if (InputSystem::GetInstance()->getLeftJoystick(playerIndex).first > 0 || InputSystem::GetInstance()->isButtonPressed(playerIndex, "Right"))
			dir = Vector3(1, 0, 0);
	}

	movement->move(dir);
}

void PlayerController::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "keyboard")
		{
			ss >> usingKeyboard;
		}
		if (prop.first == "index")
		{
			ss >> playerIndex;
		}
	}
}