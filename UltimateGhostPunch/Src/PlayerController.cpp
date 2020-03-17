#include "PlayerController.h"
#include <sstream>
#include <GameObject.h>

#include "InputSystem.h"
#include "Movement.h"
#include "GhostMovement.h"
#include "Attack.h"
#include "GhostManager.h"

PlayerController::PlayerController(GameObject* gameObject) : UserComponent(gameObject)
{

}

void PlayerController::start()
{
	inputSystem = InputSystem::GetInstance();

	movement = gameObject->getComponent<Movement>();
	ghostMovement = gameObject->getComponent<GhostMovement>();
	ghost = gameObject->getComponent<GhostManager>();

	GameObject* aux = gameObject->findChildrenWithTag("attackCollider")[0];
	if (aux != nullptr) attack = aux->getComponent<Attack>();
}

void PlayerController::update(float deltaTime)
{
	UserComponent::update(deltaTime);

	Vector3 dir = Vector3(0, 0, 0);

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
			attack->quickAttack();
		}

		else if (inputSystem->getMouseButtonClick('r')) {
			if (attack != nullptr) attack->strongAttack();
		}
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
			ss >> usingKeyboard;
		}
		if (prop.first == "index")
		{
			ss >> playerIndex;
		}
	}
}