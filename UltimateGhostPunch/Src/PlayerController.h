#pragma once
#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <UserComponent.h>
#include <GameObject.h>

#include "Movement.h"
#include "GhostMovement.h"

class PlayerController : public UserComponent
{
private:
	int playerIndex;
	bool usingKeyboard;

	bool ghost;

	Movement* movement;
	GhostMovement* ghostMovement;

public:
	PlayerController(GameObject* gameObject);

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);
};

#endif