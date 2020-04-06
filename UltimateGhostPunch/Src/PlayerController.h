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
	int controllerIndex; //From 0 to 3 included for controllers, 4 for keyboard

	// Initial position of the player
	Vector3 initialPosition;

	// Player direction
	Vector3 direction; // TODO: no se para que se usa, probablemente sobre

	// Components
	Movement* movement;
	Attack* attack;
	Jump* jump;
	Health* health;

	GhostManager* ghost;
	GhostMovement* ghostMovement;
	UltimateGhostPunch* ghostPunch;

	PlayerUI* playerUI; // TODO: sobra, esto solo trabaja con input

	// Input
	InputSystem* inputSystem;

	// Will ignore input if frozen is true
	bool frozen; //TODO: deberia de servir un setActive(false) desde fuera

	// Damage taken when falling out of the world
	float fallDamage; // TODO: sacar de aqui, hacer componentes DeadZone

public:
	PlayerController(GameObject* gameObject);
	~PlayerController();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void fixedUpdate(float deltaTime);
	virtual void handleData(ComponentData* data);

	// Manages player's input and generates a movement direction
	void checkInput(Vector3 &dir); //TODO: dir sobra, unificar controllers con keyboard

	// Respawn and damage the player if it falls out of the world
	// and returns true in that case
	bool checkOutsideLimits(); //TODO: componente DeadZone
	void respawn(const Vector3& respawnPos); //TODO: componente que se encargue del Respawn

	int getPlayerIndex() const;
	void setPlayerIndex(int index);
	void setControllerIndex(int index);

	Vector3 getInitialPosition() const;

	// Freezes / Reenables the movement 
	void setFrozen(bool freeze); //TODO: SOBRAAAAA
};

#endif