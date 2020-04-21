#pragma once
#ifndef PLATFORM_MOVEMENT_H
#define PLATFORM_MOVEMENT_H

#include "StateAction.h"

#include <Vector3.h>
class GameObject;

class PlatformMovement : public StateAction
{
private:
	GameObject* character;	// Object been controlled
	Vector3 targetPosition;	// Considering only X axis
	float leftLimit;
	float rightLimit;

public:
	PlatformMovement(StateMachine* stateMachine);
	~PlatformMovement();

	virtual void update(float deltaTime);

	void setCharacter(GameObject* character);
	void setTargetPosition(const Vector3& position);
	void setLimits(float left, float right);

};

#endif