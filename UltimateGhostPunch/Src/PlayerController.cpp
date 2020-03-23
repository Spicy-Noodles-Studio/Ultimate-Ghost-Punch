#include "PlayerController.h"
#include <sstream>
#include <GameObject.h>

#include "InputSystem.h"
#include "Movement.h"
#include "GhostMovement.h"
#include "Attack.h"
#include "Jump.h"
#include "GhostManager.h"

// BORRAR
#include "Health.h"

PlayerController::PlayerController(GameObject* gameObject) : UserComponent(gameObject)
{

}

void PlayerController::start()
{
	inputSystem = InputSystem::GetInstance();

	movement = gameObject->getComponent<Movement>();
	ghostMovement = gameObject->getComponent<GhostMovement>();
	ghost = gameObject->getComponent<GhostManager>();

	std::vector<GameObject*> aux = gameObject->findChildrenWithTag("groundSensor");
	if(aux.size() >0) jump = aux[0]->getComponent<Jump>();

	aux = gameObject->findChildrenWithTag("attackSensor");
	if (aux.size() > 0) attack = aux[0]->getComponent<Attack>();
}

void PlayerController::update(float deltaTime)
{
	UserComponent::update(deltaTime);

	Vector3 dir = Vector3(0, 0, 0);

	if (usingKeyboard)
	{
		// BORRAR
		if (inputSystem->isKeyPressed("F"))
			gameObject->getComponent<Health>()->receiveDamage(4);

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
			attack->quickAttack();
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
		}

		if (inputSystem->getButtonPress(playerIndex, "X")) {
			if (attack != nullptr) attack->quickAttack();
		}

		else if (inputSystem->getButtonPress(playerIndex, "Y")) {
			if (attack != nullptr) attack->strongAttack();
		}
		else if (InputSystem::GetInstance()->isButtonPressed(playerIndex, "A"))
			if(jump != nullptr) jump->salta();
	}

	if (!ghost) {
		if (movement != nullptr) movement->move(dir);
	}
	else
		if (ghostMovement != nullptr) ghostMovement->move(dir);
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

int PlayerController::getPlayerIndex () const
{
	return playerIndex;
}
