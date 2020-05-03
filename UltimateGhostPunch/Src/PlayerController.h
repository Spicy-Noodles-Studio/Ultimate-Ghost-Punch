#pragma once
#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <UserComponent.h>

class PlayerIndex;
class InputSystem;
class Movement;
class Attack;
class Dodge;
class Jump;
class Health;
class GhostManager;
class GhostMovement;
class UltimateGhostPunch;

class Vector3;
class Grab;
class Block;
class PlayerAnimController;


class PlayerController : public UserComponent
{
private:
	bool grabed; // Cambiar de sitio

	PlayerIndex *playerIndex;
	int controllerIndex; //From 0 to 3 included for controllers, 4 for keyboard

	Vector3 direction;

	// Components
	Movement* movement;
	Attack* attack;
	Jump* jump;
	Health* health;

	GhostManager* ghostManager;
	GhostMovement* ghostMovement;
	UltimateGhostPunch* ghostPunch;
	PlayerAnimController* animController;
	Block* block;
	Dodge* dodge;
	Grab* grab;

	// Input
	InputSystem* inputSystem;

	//Checks if the player is using a keyboard and if the key was used
	bool getKeyDown(const std::string& key);
	bool getKeyUp(const std::string& key);
	bool getKey(const std::string& key);

	//Checks if the player is using a controller and if the key was used
	bool getButtonDown(const std::string& button);
	bool getButtonUp(const std::string& button);
	bool getButton(const std::string& button);

	//Checks if the player is using a controller and if an axis was used
	//Left or Right as parameters
	int getControllerHorizontalLeftAxis();
	int getControllerHorizontalRightAxis();
	int getControllerVerticalLeftAxis();
	int getControllerVerticalRightAxis();

	int getHorizontalAxis();
	int getVerticalAxis();

	void ghostPunchMouseAim();

public:
	PlayerController(GameObject* gameObject);
	virtual ~PlayerController();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void fixedUpdate(float deltaTime);
	virtual void handleData(ComponentData* data);

	// Manages player's input and generates a movement direction
	void checkInput();

	int getControllerIndex()const;
	void setControllerIndex(int index);

	bool isGrabed();
	void setGrabed(bool grabed);
};

#endif