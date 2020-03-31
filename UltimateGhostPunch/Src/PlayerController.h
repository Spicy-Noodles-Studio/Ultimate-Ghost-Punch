#pragma once
#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <UserComponent.h>

class InputSystem;
class GhostMovement;
class Movement;
class Attack;
class GhostManager;
class Jump;
class UltimateGhostPunch;
class PlayerUI;
class Vector3;

class PlayerController : public UserComponent
{
private:
	int controllerIndex;
	bool usingKeyboard;
	bool charge = false;

	int playerIndex;
	Vector3 dir;

	Movement* movement;
	GhostMovement* ghostMovement;
	Jump* jump;
	Attack* attack;
	UltimateGhostPunch* ghostPunch;

	InputSystem* inputSystem;
	GhostManager* ghost;

	PlayerUI* playerUI;

public:
	PlayerController(GameObject* gameObject);

	virtual void start();
	virtual void update(float deltaTime);
	virtual void fixedUpdate(float deltaTime);
	virtual void handleData(ComponentData* data);

	int getPlayerIndex() const;
	void setPlayerIndex(int index);

	void setControllerIndex(int index);

	void setUsingKeyboard(bool usingKeyboard);
};

#endif