#pragma once
#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <UserComponent.h>
#include <Vector3.h>

class InputSystem;
class PlayerIndex;
class SoundManager;
class Movement;
class Attack;
class Dodge;
class Jump;
class Grab;
class Block;
class Health;
class PlayerState;
class GhostManager;
class GhostMovement;
class UltimateGhostPunch;

class PlayerController : public UserComponent
{
private:
	Vector3 direction;
	int controllerIndex; //From 0 to 3 included for controllers, 4 for keyboard

	// Components
	PlayerState* playerState;
	PlayerIndex* playerIndex;
	Movement* movement;
	Attack* attack;
	Dodge* dodge;
	Jump* jump;
	Grab* grab;
	Block* block;
	Health* health;

	SoundManager* soundManager;
	GhostManager* ghostManager;
	GhostMovement* ghostMovement;
	UltimateGhostPunch* ghostPunch;

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

protected:
	virtual void start();
	virtual void update(float deltaTime);
	virtual void fixedUpdate(float deltaTime);
	virtual void handleData(ComponentData* data);

public:
	PlayerController(GameObject* gameObject);
	virtual ~PlayerController();


	// Manages player's input and generates a movement direction
	void checkInput();

	int getControllerIndex()const;
	void setControllerIndex(int index);
};

#endif