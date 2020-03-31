#include "PlayerController.h"
#include <sstream>
#include <GameObject.h>

#include "InputSystem.h"
#include "Scene.h"
#include "Camera.h"
#include "Movement.h"
#include "GhostMovement.h"
#include "Attack.h"
#include "Jump.h"
#include "GhostManager.h"
#include "UltimateGhostPunch.h"

#include "PlayerUI.h"

PlayerController::PlayerController(GameObject* gameObject) : UserComponent(gameObject)
{

}

void PlayerController::start()
{
	inputSystem = InputSystem::GetInstance();

	movement = gameObject->getComponent<Movement>();
	ghostMovement = gameObject->getComponent<GhostMovement>();
	ghost = gameObject->getComponent<GhostManager>();
	ghostPunch = gameObject->getComponent<UltimateGhostPunch>();
	playerUI = gameObject->getComponent<PlayerUI>();

	std::vector<GameObject*> aux = gameObject->findChildrenWithTag("groundSensor");
	if (aux.size() > 0) jump = aux[0]->getComponent<Jump>();

	aux = gameObject->findChildrenWithTag("attackSensor");
	if (aux.size() > 0) attack = aux[0]->getComponent<Attack>();
}

void PlayerController::update(float deltaTime)
{
	UserComponent::update(deltaTime);

	Vector3 dir = Vector3(0, 0, 0);
	Vector3 punchDir;
	if (usingKeyboard)
	{

		if (inputSystem->getKeyPress("ESCAPE"))
			playerUI->setPauseMenuVisible(!playerUI->isPauseMenuVisible());

		if (inputSystem->isKeyPressed("A"))
			dir = Vector3(-1, 0, 0);
		else if (inputSystem->isKeyPressed("D"))
			dir = Vector3(1, 0, 0);

		if (ghost != nullptr && ghost->isGhost()) {
			if (inputSystem->isKeyPressed("W"))
				dir += Vector3(0, 1, 0);
			else if (inputSystem->isKeyPressed("S"))
				dir += Vector3(0, -1, 0);
		}

		if (inputSystem->getMouseButtonClick('l')) {
			if (ghost == nullptr || !ghost->isGhost())
				attack->quickAttack();
			else
			{
				if (ghostPunch != nullptr && ghostPunch->isAvailable())
				{
					charge = true;
				}
			}
		}

		if (inputSystem->getMouseButtonHold('l'))
		{
			if (ghost != nullptr || ghost->isGhost())
				if (ghostPunch != nullptr && ghostPunch->isAvailable() && charge)
				{
					std::pair<int, int> mousePos = inputSystem->getMousePosition();

					Vector3* thisOnScreen = &gameObject->getScene()->getMainCamera()->worldToScreenPixel(gameObject->getComponent<Transform>()->getPosition());

					//std::cout << thisOnScreen->x << " " << thisOnScreen->y << " " << std::endl;


					punchDir.x = mousePos.first - thisOnScreen->x;
					punchDir.y = thisOnScreen->y - mousePos.second;
					punchDir.z = 0;

					punchDir.normalize();

				}
		}
		if (inputSystem->getMouseButtonRelease('l'))
		{
			if (ghost != nullptr || ghost->isGhost())
				if (ghostPunch != nullptr && ghostPunch->isAvailable() && charge)
				{
					std::pair<int, int> mousePos = inputSystem->getMousePosition();

					Vector3* thisOnScreen = &gameObject->getScene()->getMainCamera()->worldToScreenPixel(gameObject->getComponent<Transform>()->getPosition());

					//std::cout << thisOnScreen->x << " " << thisOnScreen->y << " " << std::endl;


					punchDir.x = mousePos.first - thisOnScreen->x;
					punchDir.y = thisOnScreen->y - mousePos.second;
					punchDir.z = 0;

					punchDir.normalize();
					ghostPunch->ghostPunch(punchDir);
					charge = false;
				}

		}


		else if (inputSystem->getMouseButtonClick('r')) {
			if (attack != nullptr) attack->strongAttack();
		}
		else if (InputSystem::GetInstance()->isKeyPressed("Space"))
			if (jump != nullptr) jump->salta();
	}
	else
	{
		if (inputSystem->getLeftJoystick(controllerIndex).first < 0 || inputSystem->isButtonPressed(controllerIndex, "Left"))
			dir = Vector3(-1, 0, 0);
		else if (inputSystem->getLeftJoystick(controllerIndex).first > 0 || inputSystem->isButtonPressed(controllerIndex, "Right"))
			dir = Vector3(1, 0, 0);

		if (ghost != nullptr && ghost->isGhost()) {
			if (inputSystem->getLeftJoystick(controllerIndex).second < 0 || inputSystem->isButtonPressed(controllerIndex, "Up"))
				dir += Vector3(0, 1, 0);
			else if (inputSystem->getLeftJoystick(controllerIndex).second > 0 || inputSystem->isButtonPressed(controllerIndex, "Down"))
				dir += Vector3(0, -1, 0);

			if (inputSystem->getRightJoystick(controllerIndex).second != 0 || inputSystem->getRightJoystick(controllerIndex).first != 0)
			{
				charge = true;

				punchDir.x = inputSystem->getRightJoystick(controllerIndex).first;
				punchDir.y = -inputSystem->getRightJoystick(controllerIndex).second;
				punchDir.z = 0;

				punchDir.normalize();

				if (inputSystem->getButtonPress(controllerIndex, "X"))
				{
					ghostPunch->ghostPunch(punchDir);
					charge = false;
				}
			}
		}

		if (inputSystem->getButtonPress(controllerIndex, "X")) {
			if (ghost == nullptr || !ghost->isGhost())
				if (attack != nullptr) attack->quickAttack();
		}

		else if (inputSystem->getButtonPress(controllerIndex, "Y")) {
			if (attack != nullptr) attack->strongAttack();
		}
		else if (InputSystem::GetInstance()->isButtonPressed(controllerIndex, "A"))
			if (jump != nullptr) jump->salta();
	}

	if (!ghost->isGhost()) {
		if (movement != nullptr) movement->move(dir);
	}
	else {
		if (ghostMovement != nullptr) {

			if (ghostPunch != nullptr && !ghostPunch->isPunching() && !charge)
				ghostMovement->move(dir);
			else if (ghostPunch == nullptr)
				ghostMovement->move(dir);
		}
	}
}

void PlayerController::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "keyboard")
		{
			if (!(ss >> usingKeyboard))
				LOG("PLAYER CONTROLLER: Invalid property with name \"%s\"", prop.first.c_str());
		}
		if (prop.first == "index")
		{
			if (!(ss >> controllerIndex))
				LOG("PLAYER CONTROLLER: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("PLAYER CONTROLLER: Invalid property name \"%s\"", prop.first.c_str());
	}
}

int PlayerController::getPlayerIndex() const
{
	return playerIndex;
}

void PlayerController::setPlayerIndex(int index)
{
	playerIndex = index;
}

void PlayerController::setControllerIndex(int index)
{
	controllerIndex = index;
}

void PlayerController::setUsingKeyboard(bool usingKeyboard)
{
	this->usingKeyboard = usingKeyboard;
}
