#pragma once
#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <UserComponent.h>

class GameObject;
class InputSystem;
class GhostMovement;
class Movement;
class Attack;
class GhostManager;
class Jump;

class PlayerController : public UserComponent
{
private:
	int playerIndex;
	bool usingKeyboard;

	Movement* movement;
	GhostMovement* ghostMovement;
	Jump* jump;
	Attack* attack;

	InputSystem* inputSystem;
	GhostManager* ghost;

public:
	PlayerController(GameObject* gameObject);

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

	int getPlayerIndex() const;
};

#endif