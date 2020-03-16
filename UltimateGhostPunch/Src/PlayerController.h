#pragma once
#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <UserComponent.h>
#include <GameObject.h>

#include "Movement.h"

class PlayerController : public UserComponent
{
private:
	int playerIndex;
	bool usingKeyboard;

	Movement* movement;

public:
	PlayerController(GameObject* gameObject);

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);
};

#endif