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
class UltimateGhostPunch;

class PlayerController : public UserComponent
{
private:
	int playerIndex;
	bool usingKeyboard;
	bool charge = false;


	Movement* movement;
	GhostMovement* ghostMovement;
	Jump* jump;
	Attack* attack;
	UltimateGhostPunch* ghostPunch;

	InputSystem* inputSystem;
	GhostManager* ghost;

	// Will ignore input if frozen is true
	bool frozen = false;

public:
	PlayerController(GameObject* gameObject);

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

	int getPlayerIndex() const;
	void respawn(const Vector3& respawnPos);

	// Freezes / Reenables the movement 
	void setFrozen(bool freeze);
};

#endif