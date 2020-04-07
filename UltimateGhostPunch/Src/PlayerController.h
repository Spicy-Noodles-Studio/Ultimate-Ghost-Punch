#pragma once
#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <UserComponent.h>

class InputSystem;
class GhostMovement;
class Movement;
class Attack;
class GhostManager;
class Dodge;
class Jump;
class UltimateGhostPunch;
class Animator;

class Vector3;
class Grab;
class Block;


class PlayerController : public UserComponent
{
private:
	int controllerIndex;
	bool usingKeyboard;
	bool charge = false;
	bool isBlocking = false;

	int playerIndex;
	Vector3 dir;

	// Initial position of the player
	Vector3 iniPosition = {0,0,0};

	Movement* movement;
	GhostMovement* ghostMovement;
	Jump* jump;
	Attack* attack;
	UltimateGhostPunch* ghostPunch;
	Block* block;

	InputSystem* inputSystem;
	GhostManager* ghost;
	Animator* anim;

	Dodge* dodge;
	Grab* grab;
	// Will ignore input if frozen is true
	bool frozen = false;


	// Damage taken when falling out of the world
	float fallDamage = 2.0f;

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