#include "PlayerController.h"

#include <sstream>
#include <ComponentRegister.h>
#include <GameObject.h>

#include <InputSystem.h>
#include <Scene.h>
#include <Camera.h>
#include <Light.h>

#include "Movement.h"
#include "Attack.h"
#include "Jump.h"
#include "Health.h"
#include "GhostManager.h"
#include "GhostMovement.h"
#include "Dodge.h"
#include "UltimateGhostPunch.h"
#include "PlayerAnimController.h"
#include "Grab.h"
#include "Block.h"
#include "GameManager.h"
#include "PlayerUI.h"

REGISTER_FACTORY(PlayerController);

PlayerController::PlayerController(GameObject* gameObject) : UserComponent(gameObject), inputSystem(nullptr), movement(nullptr), ghostManager(nullptr), ghostMovement(nullptr), ghostPunch(nullptr),
health(nullptr), jump(nullptr), attack(nullptr), direction(Vector3()), playerIndex(1), controllerIndex(1), isBlocking(false), animController(nullptr)
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

	ghostMovement = gameObject->getComponent<GhostMovement>();
	ghostManager = gameObject->getComponent<GhostManager>();
	ghostPunch = gameObject->getComponent<UltimateGhostPunch>();
	dodge = gameObject->getComponent<Dodge>();
	animController = gameObject->getComponent<PlayerAnimController>();

	std::vector<GameObject*> aux = gameObject->findChildrenWithTag("groundSensor");
	if (aux.size() > 0) jump = aux[0]->getComponent<Jump>();
	if (aux.size() > 0) block = aux[0]->getComponent<Block>();

	aux = gameObject->findChildrenWithTag("attackSensor");
	if (aux.size() > 0) attack = aux[0]->getComponent<Attack>();

	aux = gameObject->findChildrenWithTag("grabSensor");
	if (aux.size() > 0) grab = aux[0]->getComponent<Grab>();
}

void PlayerController::update(float deltaTime)
{
	checkInput();
}

void PlayerController::fixedUpdate(float deltaTime)
{
	//Movement
	if (ghostManager == nullptr || !ghostManager->isGhost())
		if (movement != nullptr) movement->move(direction);
}

void PlayerController::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "index")
		{
			if (!(ss >> controllerIndex))
				LOG("PLAYER CONTROLLER: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("PLAYER CONTROLLER: Invalid property name \"%s\"", prop.first.c_str());
	}
}



void PlayerController::checkInput()
{
	direction = Vector3(0, 0, 0);

	if (block == nullptr || !block->blocking()) {
		//Movement
		direction += Vector3(getHorizontalAxis(), 0, 0);
		//Character rotation
		if (direction.x != 0)
			gameObject->transform->setRotation({ 0,90 * direction.x,0 });
	}

	//Acctions if the player isn�t in ghostManager mode
	if (ghostManager == nullptr || !ghostManager->isGhost()) {

		//If we are not blocking
		if (block == nullptr || !block->blocking()) {
			//Attack
			if (attack != nullptr) {
				//Quick attack
				if ((controllerIndex == 4 && inputSystem->getMouseButtonClick('l')) || getButtonDown("X")) {
					attack->quickAttack();
					animController->quickAttackAnimation();
				}
				//Strong attack
				else if ((controllerIndex == 4 && inputSystem->getMouseButtonClick('r')) || getButtonDown("Y"))
				{
					attack->strongAttack();
					animController->strongAttackAnimation();
				}
			}
			//Dodge
			if (dodge != nullptr)
				if (getKeyDown("LEFT SHIFT") || getButtonDown("RB"))
				{
					dodge->dodge();
					animController->dashAnimation();
				}

			//Jump
			if (jump != nullptr)
			{
				if (getKey("Space") || getButton("A"))
				{
					jump->jump();
					animController->jumpAnimation();
				}
			}

			//Grab
			if (grab != nullptr) {
				if (getKey("E") || getButton("LB"))
				{
					grab->grab();
					animController->grabAnimation();
				}
				else if (getKeyUp("E") || getButtonUp("LB")) grab->drop();
			}
		}

		//Block
		if (block != nullptr) {
			if (getKeyDown("S") || getButtonDown("B"))
			{
				block->block();
				animController->blockAnimation();
			}
			if (isBlocking && (getKeyUp("S") || getButtonUp("B"))) block->unblock();
		}

		//Taunt
		if (getKeyDown("T") || getButtonDown("BACK"))
		{
			animController->tauntAnimation();
		}
	}

	//Actions if the player is in ghostManager mode
	else if (ghostManager != nullptr && ghostManager->isGhost())
	{
		direction += Vector3(0, getVerticalAxis(), 0);

		//GhostPunch
		if (ghostPunch != nullptr)
		{
			int horizontal = getControllerHorizontalRightAxis(), vertical = getControllerVerticalRightAxis();
			//Charge
			if (ghostPunch->getState() == UltimateGhostPunch::State::AVAILABLE) {
				if ((controllerIndex == 4 && inputSystem->getMouseButtonClick('l')) || (vertical != 0 || horizontal != 0))
					ghostPunch->charge();
			}
			else if (ghostPunch->getState() == UltimateGhostPunch::State::CHARGING) {
				//Aim
				if (controllerIndex == 4) ghostPunchMouseAim();
				else ghostPunch->aim(horizontal, -vertical);

				//Ghost Punch
				if (controllerIndex == 4 && inputSystem->getMouseButtonRelease('l') || getButtonDown("X"))
					ghostPunch->ghostPunch();
			}
		}

		//Ghost Movement
		if (ghostPunch == nullptr || (ghostPunch->getState() != UltimateGhostPunch::State::PUNCHING))
			if (ghostMovement != nullptr) ghostMovement->move(direction);
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

void PlayerController::setBlocking(bool _block)
{
	isBlocking = _block;
}

void PlayerController::ghostPunchMouseAim()
{
	std::pair<int, int> mousePos = inputSystem->getMousePosition();
	Vector3* thisOnScreen = &gameObject->getScene()->getMainCamera()->worldToScreenPixel(gameObject->transform->getPosition());
	ghostPunch->aim(mousePos.first - thisOnScreen->x, thisOnScreen->y - mousePos.second);
}
