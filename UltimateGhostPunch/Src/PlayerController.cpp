#include "PlayerController.h"
#include <ComponentRegister.h>
#include <InputSystem.h>
#include <GameObject.h>
#include <Scene.h>
#include <Camera.h>
#include <Light.h>
#include <sstream>

#include "PlayerUI.h"
#include "Movement.h"
#include "Attack.h"
#include "Dodge.h"
#include "Jump.h"
#include "Grab.h"
#include "Block.h"
#include "Health.h"
#include "GhostManager.h"
#include "GhostMovement.h"
#include "SoundManager.h"
#include "UltimateGhostPunch.h"
#include "GameManager.h"
#include "PlayerState.h"

REGISTER_FACTORY(PlayerController);

PlayerController::PlayerController(GameObject* gameObject) : UserComponent(gameObject), inputSystem(nullptr), playerIndex(nullptr), movement(nullptr), attack(nullptr), dodge(nullptr),
jump(nullptr), grab(nullptr), block(nullptr), health(nullptr), ghostManager(nullptr), ghostMovement(nullptr), ghostPunch(nullptr),
direction(Vector3::ZERO), controllerIndex(1), grabed(false)
{

}

PlayerController::~PlayerController()
{

}

void PlayerController::start()
{
	inputSystem = InputSystem::GetInstance();

	movement = gameObject->getComponent<Movement>();
	health = gameObject->getComponent<Health>();
	dodge = gameObject->getComponent<Dodge>();

	ghostMovement = gameObject->getComponent<GhostMovement>();
	ghostManager = gameObject->getComponent<GhostManager>();
	ghostPunch = gameObject->getComponent<UltimateGhostPunch>();

	std::vector<GameObject*> aux = gameObject->findChildrenWithTag("groundSensor");
	if (aux.size() > 0)
		jump = aux[0]->getComponent<Jump>();

	if (aux.size() > 0)
		block = aux[0]->getComponent<Block>();

	aux = gameObject->findChildrenWithTag("attackSensor");
	if (aux.size() > 0)
		attack = aux[0]->getComponent<Attack>();

	aux = gameObject->findChildrenWithTag("grabSensor");
	if (aux.size() > 0)
		grab = aux[0]->getComponent<Grab>();
}

void PlayerController::update(float deltaTime)
{
	if (gameObject->getComponent<PlayerState>() != nullptr && gameObject->getComponent<PlayerState>()->isIgnoringInput())
	{
		direction = Vector3::ZERO;
		return; 
	}
	checkInput();
}

void PlayerController::fixedUpdate(float deltaTime)
{
	if (movement != nullptr)
		movement->move(direction);
}

void PlayerController::handleData(ComponentData* data)
{
	if (data == nullptr) return;
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "controllerIndex")
		{
			setInt(controllerIndex);
		}
		else
			LOG("PLAYER CONTROLLER: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void PlayerController::checkInput()
{
	if (inputSystem == nullptr) return;
	//Movement
	direction = Vector3::ZERO;
	direction += Vector3(getHorizontalAxis(), 0, 0);

	//Acctions if the player isn't in ghostManager mode
	if (ghostManager == nullptr || !ghostManager->isGhost())
	{
		//Attack
		if (attack != nullptr)
		{
			//Quick attack
			if ((controllerIndex == 4 && inputSystem->getMouseButtonClick('l')) || getButtonDown("X"))
				attack->quickAttack();
			//Strong attack
			else if ((controllerIndex == 4 && inputSystem->getMouseButtonClick('r')) || getButtonDown("Y"))
				attack->strongAttack();
		}

		//Dodge
		if (dodge != nullptr)
			if (getKeyDown("LEFT SHIFT") || getButtonDown("RB"))
				dodge->dodge();

		//Jump
		if (jump != nullptr)
		{
			if (getKey("Space") || getButton("A"))
				jump->jump();
			else if (getKeyUp("Space") || getButtonUp("A"))
				jump->cancelJump();
		}

		//Grab
		if (grab != nullptr)
		{
			if (getKey("E") || getButton("LB"))
				grab->grab();
			else if (getKeyUp("E") || getButtonUp("LB"))
				grab->drop();
		}

		//Block
		if (block != nullptr)
		{
			if (getKeyDown("S") || getButtonDown("B"))
				block->block();

			if (block->isBlocking() && (getKeyUp("S") || getButtonUp("B")))
				block->unblock();
		}

		//Taunt
		if (getKeyDown("T") || getButtonDown("BACK")) {
			if(gameObject->getComponent<SoundManager>() != nullptr)
				gameObject->getComponent<SoundManager>()->playTaunt();
		}
	}

	//Actions if the player is in ghost mode
	else if (ghostManager != nullptr && ghostManager->isGhost())
	{
		direction += Vector3(0, getVerticalAxis(), 0);

		//GhostPunch
		if (ghostPunch != nullptr)
		{
			int horizontal = getControllerHorizontalRightAxis(), vertical = getControllerVerticalRightAxis();

			//Charge
			if (controllerIndex == 4 && inputSystem->getMouseButtonClick('l') || getButtonDown("RB"))
				ghostPunch->charge();

			//Aim
			if (controllerIndex == 4)
				ghostPunchMouseAim();
			else
				ghostPunch->aim(horizontal, -vertical);

			//Ghost Punch
			if (controllerIndex == 4 && inputSystem->getMouseButtonRelease('l') || getButtonUp("RB"))
				ghostPunch->ghostPunch();
		}

		//Ghost Movement
		if (ghostMovement != nullptr) ghostMovement->move(direction);
	}
}

int PlayerController::getControllerIndex() const
{
	return controllerIndex;
}

void PlayerController::setControllerIndex(int index)
{
	controllerIndex = index;
}

bool PlayerController::getKeyDown(const std::string& key)
{
	return controllerIndex == 4 && inputSystem->getKeyPress(key);
}

bool PlayerController::getKeyUp(const std::string& key)
{
	return controllerIndex == 4 && inputSystem->getKeyRelease(key);
}

bool PlayerController::getKey(const std::string& key)
{
	return controllerIndex == 4 && inputSystem->isKeyPressed(key);
}

bool PlayerController::getButtonDown(const std::string& button)
{
	return controllerIndex < 4 && inputSystem->getButtonPress(controllerIndex, button);
}

bool PlayerController::getButtonUp(const std::string& button)
{
	return controllerIndex < 4 && inputSystem->getButtonRelease(controllerIndex, button);
}

bool PlayerController::getButton(const std::string& button)
{
	return controllerIndex < 4 && inputSystem->isButtonPressed(controllerIndex, button);
}

int PlayerController::getControllerHorizontalLeftAxis()
{
	if (controllerIndex < 4)
		return inputSystem->getLeftJoystick(controllerIndex).first;
	else return 0;
}

int PlayerController::getControllerHorizontalRightAxis()
{
	if (controllerIndex < 4)
		return inputSystem->getRightJoystick(controllerIndex).first;
	else return 0;
}

int PlayerController::getControllerVerticalLeftAxis()
{
	if (controllerIndex < 4)
		return inputSystem->getLeftJoystick(controllerIndex).second;
	else return 0;
}

int PlayerController::getControllerVerticalRightAxis()
{
	if (controllerIndex < 4)
		return inputSystem->getRightJoystick(controllerIndex).second;
	else return 0;
}

int PlayerController::getHorizontalAxis()
{
	if (getKey("A") || getControllerHorizontalLeftAxis() < 0 || getButton("Left"))
		return -1;
	else if (getKey("D") || getControllerHorizontalLeftAxis() > 0 || getButton("Right"))
		return 1;
	else
		return 0;
}

int PlayerController::getVerticalAxis()
{
	if (getKey("W") || getControllerVerticalLeftAxis() < 0 || getButton("Up"))
		return 1;
	else if (getKey("S") || getControllerVerticalLeftAxis() > 0 || getButton("Down"))
		return -1;
	else
		return 0;
}

void PlayerController::ghostPunchMouseAim()
{
	std::pair<int, int> mousePos = inputSystem->getMousePosition();
	if (gameObject->getScene()->getMainCamera() != nullptr && gameObject->transform != nullptr && ghostPunch != nullptr) {
		Vector3* thisOnScreen = &gameObject->getScene()->getMainCamera()->worldToScreenPixel(gameObject->transform->getPosition());
		ghostPunch->aim(mousePos.first - thisOnScreen->x, thisOnScreen->y - mousePos.second);
	}
}