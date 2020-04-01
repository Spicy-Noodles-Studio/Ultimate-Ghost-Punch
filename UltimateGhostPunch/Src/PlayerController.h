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

	// Initial position of the player
	Vector3 iniPosition = {0,0,0};

	Movement* movement;
	GhostMovement* ghostMovement;
	Jump* jump;
	Attack* attack;
	UltimateGhostPunch* ghostPunch;

	InputSystem* inputSystem;
	GhostManager* ghost;

	// Will ignore input if frozen is true
	bool frozen = false;

	// Limit of the world in the Y axis (HAY QUE METERLO EN EL GAMEMANAGER)
	float bottomLimit = -500.0f;

	// Damage taken when falling out of the world
	float fallDamage = 2.0f;
	PlayerUI* playerUI;

public:
	PlayerController(GameObject* gameObject);

	virtual void start();
	virtual void update(float deltaTime);
	virtual void fixedUpdate(float deltaTime);
	virtual void handleData(ComponentData* data);

	// Manages player's input and generates a movement direction
	void checkInput(Vector3 &dir);
	// Respawn and damage the player if it falls out of the world,
	// and returns true in that case
	bool checkOutsideLimits();

	int getPlayerIndex() const;
	Vector3 getInitialPosition() const;

	void respawn(const Vector3& respawnPos);
	// Freezes / Reenables the movement 
	void setFrozen(bool freeze);
	void setPlayerIndex(int index);

	void setControllerIndex(int index);

	void setUsingKeyboard(bool usingKeyboard);
};

#endif