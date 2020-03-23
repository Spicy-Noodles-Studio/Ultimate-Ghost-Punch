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

	std::vector<GameObject*> aux = gameObject->findChildrenWithTag("groundSensor");
	if(aux.size() >0) jump = aux[0]->getComponent<Jump>();

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
			if(ghost == nullptr || !ghost->isGhost())
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
				if (ghostPunch != nullptr && ghostPunch->isAvailable()&& charge)
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
				if (ghostPunch != nullptr && ghostPunch->isAvailable() && charge )
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
			if(jump != nullptr) jump->salta();
	}
	else
	{
		if (inputSystem->getLeftJoystick(playerIndex).first < 0 || inputSystem->isButtonPressed(playerIndex, "Left"))
			dir = Vector3(-1, 0, 0);
		else if (inputSystem->getLeftJoystick(playerIndex).first > 0 || inputSystem->isButtonPressed(playerIndex, "Right"))
			dir = Vector3(1, 0, 0);

		if (ghost != nullptr && ghost->isGhost()) {
			if (inputSystem->getLeftJoystick(playerIndex).second < 0 || inputSystem->isButtonPressed(playerIndex, "Up"))
				dir += Vector3(0, 1, 0);
			else if (inputSystem->getLeftJoystick(playerIndex).second > 0 || inputSystem->isButtonPressed(playerIndex, "Down"))
				dir += Vector3(0, -1, 0);

			if (inputSystem->getRightJoystick(playerIndex).second != 0 || inputSystem->getRightJoystick(playerIndex).first != 0)
			{
				charge = true;

				punchDir.x = inputSystem->getRightJoystick(playerIndex).first;
				punchDir.y = -inputSystem->getRightJoystick(playerIndex).second;
				punchDir.z = 0;

				punchDir.normalize();

				if (inputSystem->getButtonPress(playerIndex, "X"))
				{
					ghostPunch->ghostPunch(punchDir);
					charge = false;
				}

			}
		}

		

		if (inputSystem->getButtonPress(playerIndex, "X")) {
			if(ghost == nullptr || !ghost->isGhost())
			if (attack != nullptr) attack->quickAttack();
		}

		else if (inputSystem->getButtonPress(playerIndex, "Y")) {
			if (attack != nullptr) attack->strongAttack();
		}
		else if (InputSystem::GetInstance()->isButtonPressed(playerIndex, "A"))
			if(jump != nullptr) jump->salta();
	}

	if (!ghost->isGhost()) {
		if (movement != nullptr) movement->move(dir);
	}
	else {
		if (ghostMovement != nullptr) {
			
			if (ghostPunch != nullptr && !ghostPunch->isPunching() &&! charge)
			ghostMovement->move(dir);
			else if(ghostPunch == nullptr)
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
			if(!(ss >> usingKeyboard))
				LOG("PLAYER CONTROLLER: Invalid property with name \"%s\"", prop.first.c_str());
		}
		if (prop.first == "index")
		{
			if(!(ss >> playerIndex))
				LOG("PLAYER CONTROLLER: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("PLAYER CONTROLLER: Invalid property name \"%s\"", prop.first.c_str());
	}
}