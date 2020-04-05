#pragma once
#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <UserComponent.h>

class InputSystem;
class Movement;
class Attack;
class Jump;
class Health;
class GhostManager;
class GhostMovement;
class UltimateGhostPunch;
class PlayerUI;
class Vector3;

class PlayerController : public UserComponent
{
private:
	// Indexes
	int playerIndex;
	int controllerIndex;
	bool usingKeyboard;

	// Initial position of the player
	Vector3 iniPosition;

	// Player direction
	Vector3 direction;

	// Components
	Movement* movement;
	Attack* attack;
	Jump* jump;
	Health* health;

	GhostManager* ghost;
	GhostMovement* ghostMovement;
	UltimateGhostPunch* ghostPunch;

	PlayerUI* playerUI;

	// Input
	InputSystem* inputSystem;

	// Will ignore input if frozen is true
	bool frozen;

	// Damage taken when falling out of the world
	float fallDamage;

public:
	PlayerController(GameObject* gameObject);

	virtual void start();
	virtual void update(float deltaTime);
	virtual void fixedUpdate(float deltaTime);
	virtual void handleData(ComponentData* data);

	// Manages player's input and generates a movement direction
	void checkInput(Vector3 &dir);

	// Respawn and damage the player if it falls out of the world
	// and returns true in that case
	bool checkOutsideLimits();
	void respawn(const Vector3& respawnPos);

	int getPlayerIndex() const;
	void setPlayerIndex(int index);
	void setControllerIndex(int index);
	void setUsingKeyboard(bool keyboard);

	Vector3 getInitialPosition() const;

	// Freezes / Reenables the movement 
	void setFrozen(bool freeze);
};

#endif