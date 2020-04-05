#include "PlayerController.h"

#include <GameObject.h>
#include <sstream>

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
#include "UltimateGhostPunch.h"

#include "ComponentRegister.h"
#include "GameManager.h"
#include "PlayerUI.h"

REGISTER_FACTORY(PlayerController);

PlayerController::PlayerController(GameObject* gameObject) : UserComponent(gameObject)
{

}

void PlayerController::start()
{
	inputSystem = InputSystem::GetInstance();

	movement = gameObject->getComponent<Movement>();
	health = gameObject->getComponent<Health>();

	ghostMovement = gameObject->getComponent<GhostMovement>();
	ghost = gameObject->getComponent<GhostManager>();
	ghostPunch = gameObject->getComponent<UltimateGhostPunch>();

	playerUI = gameObject->getComponent<PlayerUI>();

	std::vector<GameObject*> aux = gameObject->findChildrenWithTag("groundSensor");
	if (aux.size() > 0) jump = aux[0]->getComponent<Jump>();

	aux = gameObject->findChildrenWithTag("attackSensor");
	if (aux.size() > 0) attack = aux[0]->getComponent<Attack>();

	iniPosition = gameObject->transform->getPosition();
	frozen = false;
}

void PlayerController::update(float deltaTime)
{
	if (playerUI != nullptr)
	{
		if (health != nullptr && health->isAlive())
		{
			if (health->isInvencible())
				playerUI->updateState("Invencible");
			else
				playerUI->updateState("Alive");
		}

		playerUI->updateHealth();
	}

	// Ignore input
	if (!frozen)
	{
		checkInput(direction);

		if (ghost != nullptr && ghost->isGhost())
		{
			if (ghostPunch == nullptr || (ghostPunch->getState() != UltimateGhostPunch::State::CHARGING && ghostPunch->getState() != UltimateGhostPunch::State::PUNCHING))
				if (ghostMovement != nullptr) ghostMovement->move(direction);
		}
	}
}

void PlayerController::fixedUpdate(float deltaTime)
{
	// Don't move or player has fallen off limits
	if (!frozen && !checkOutsideLimits())
	{
		//Movimiento
		if (ghost == nullptr || !ghost->isGhost())
			if (movement != nullptr) movement->move(direction);
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
		else if (prop.first == "index")
		{
			if (!(ss >> controllerIndex))
				LOG("PLAYER CONTROLLER: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "fallDamage")
		{
			if (!(ss >> fallDamage))
				LOG("PLAYER CONTROLLER: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("PLAYER CONTROLLER: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void PlayerController::checkInput(Vector3& dir)
{
	dir = Vector3(0, 0, 0);
	Vector3 punchDir;

	//Controles con teclado y raton
	if (usingKeyboard)
	{
		if (inputSystem->getKeyPress("ESCAPE"))
			/*playerUI->setPauseMenuVisible(!playerUI->isPauseMenuVisible())*/;

		if (inputSystem->isKeyPressed("A"))
		{
			dir = Vector3(-1, 0, 0);
			gameObject->transform->setRotation({ 0,-90,0 });
		}
		else if (inputSystem->isKeyPressed("D"))
		{
			dir = Vector3(1, 0, 0);
			gameObject->transform->setRotation({ 0,90,0 });
		}

		if (ghost != nullptr && ghost->isGhost())
		{
			if (inputSystem->isKeyPressed("W"))
				dir += Vector3(0, 1, 0);
			else if (inputSystem->isKeyPressed("S"))
				dir += Vector3(0, -1, 0);

			else if (inputSystem->getMouseButtonHold('l'))
			{
				if (ghostPunch != nullptr && ghostPunch->getState() == UltimateGhostPunch::State::CHARGING)
				{
					std::pair<int, int> mousePos = inputSystem->getMousePosition();
					Vector3* thisOnScreen = &gameObject->getScene()->getMainCamera()->worldToScreenPixel(gameObject->transform->getPosition());
					ghostPunch->aim(mousePos.first - thisOnScreen->x, thisOnScreen->y - mousePos.second);
				}
			}
			else if (inputSystem->getMouseButtonRelease('l'))
			{
				if (ghostPunch != nullptr && ghostPunch->getState() == UltimateGhostPunch::State::CHARGING)
				{
					std::pair<int, int> mousePos = inputSystem->getMousePosition();
					Vector3* thisOnScreen = &gameObject->getScene()->getMainCamera()->worldToScreenPixel(gameObject->transform->getPosition());
					ghostPunch->aim(mousePos.first - thisOnScreen->x, thisOnScreen->y - mousePos.second);

					ghostPunch->ghostPunch();
				}
			}
		}

		if (inputSystem->getMouseButtonClick('l'))
		{
			if (ghost == nullptr || !ghost->isGhost())
				attack->quickAttack();
			else if (ghostPunch != nullptr && ghostPunch->getState() == UltimateGhostPunch::State::AVAILABLE)
				ghostPunch->charge();
		}
		else if (inputSystem->getMouseButtonClick('r'))
		{
			if (ghost == nullptr || !ghost->isGhost())
				if (attack != nullptr) attack->strongAttack();
		}
		else if (InputSystem::GetInstance()->isKeyPressed("Space"))
			if (ghost == nullptr || !ghost->isGhost())
				if (jump != nullptr) jump->jump();
	}

	//Controles con mando
	else
	{
		if (inputSystem->getLeftJoystick(controllerIndex).first < 0 || inputSystem->isButtonPressed(controllerIndex, "Left"))
		{
			dir = Vector3(-1, 0, 0);
			gameObject->transform->setRotation({ 0,-90,0 });
		}
		else if (inputSystem->getLeftJoystick(controllerIndex).first > 0 || inputSystem->isButtonPressed(controllerIndex, "Right"))
		{
			dir = Vector3(1, 0, 0);
			gameObject->transform->setRotation({ 0,90,0 });
		}

		if (ghost != nullptr && ghost->isGhost())
		{
			if (inputSystem->getLeftJoystick(controllerIndex).second < 0 || inputSystem->isButtonPressed(controllerIndex, "Up"))
				dir += Vector3(0, 1, 0);
			else if (inputSystem->getLeftJoystick(controllerIndex).second > 0 || inputSystem->isButtonPressed(controllerIndex, "Down"))
				dir += Vector3(0, -1, 0);

			if (inputSystem->getRightJoystick(controllerIndex).first != 0 || inputSystem->getRightJoystick(controllerIndex).second != 0)
			{
				if (ghostPunch != nullptr)
				{
					if (ghostPunch->getState() == UltimateGhostPunch::State::AVAILABLE) ghostPunch->charge();
					else if (ghostPunch->getState() == UltimateGhostPunch::State::CHARGING)
					{
						ghostPunch->aim(inputSystem->getRightJoystick(controllerIndex).first, -inputSystem->getRightJoystick(controllerIndex).second);

						if (inputSystem->getButtonPress(controllerIndex, "X"))
							ghostPunch->ghostPunch();
					}
				}
			}
		}

		if (inputSystem->getButtonPress(controllerIndex, "X"))
		{
			if (ghost == nullptr || !ghost->isGhost())
				if (attack != nullptr) attack->quickAttack();
		}
		else if (inputSystem->getButtonPress(controllerIndex, "Y"))
		{
			if (ghost == nullptr || !ghost->isGhost())
				if (attack != nullptr) attack->strongAttack();
		}
		else if (InputSystem::GetInstance()->isButtonPressed(controllerIndex, "A"))
			if (ghost == nullptr || !ghost->isGhost())
				if (jump != nullptr) jump->jump();
	}

	if (ghost != nullptr && ghost->isGhost())
	{
		if (ghostPunch == nullptr || (ghostPunch->getState() != UltimateGhostPunch::State::PUNCHING))
			if (ghostMovement != nullptr) ghostMovement->move(dir);
	}
}

bool PlayerController::checkOutsideLimits()
{
	bool out = gameObject->transform->getPosition().y <= GameManager::GetInstance()->getBottomLimit();

	if (out)
	{
		if (health != nullptr) health->receiveDamage(fallDamage);

		// If the player is alive after falling it respawns
		// else, it appears as a ghost at initial position
		if (health != nullptr)
		{
			if (health->getHealth() > 0)
				respawn(iniPosition);
			else
			{
				// Sets the initial position as the respawn if the player resurrects
				ghost->setDeathPosition(iniPosition);

				gameObject->setActive(false);
				gameObject->transform->setPosition(iniPosition);

				//if (playerUI != nullptr) playerUI->updateState("Dead");
				//findGameObjectWithName("FightManager")->getComponent<FightManager>()->playerDie();
			}
		}
	}

	return out;
}

void PlayerController::respawn(const Vector3& respawnPos)
{
	if (health != nullptr)
	{
		health->setHealth(health->getResurrectionHealth());
		health->setInvencible(true);
		health->setTime(health->getInvResTime());

		if (playerUI != nullptr)
		{
			playerUI->updateHealth();
			playerUI->updateState("Respawning");
		}
	}

	gameObject->transform->setPosition(respawnPos);
	LOG("{%f, %f, %f}\n", respawnPos.x, respawnPos.y, respawnPos.z);
	movement->stop();
	setFrozen(false);
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

void PlayerController::setUsingKeyboard(bool keyboard)
{
	usingKeyboard = keyboard;
}

Vector3 PlayerController::getInitialPosition() const
{
	return iniPosition;
}

void PlayerController::setFrozen(bool freeze)
{
	frozen = freeze;
}