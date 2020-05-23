#pragma once
#ifndef PLATFORM_MOVEMENT_H
#define PLATFORM_MOVEMENT_H

#include "StateAction.h"

#include <Vector3.h>

#include "PlatformGraph.h"

class GameObject;

class PlatformMovement : public StateAction
{
private:
	PlatformGraph* platformGraph;
	GameObject* character;	// Object been controlled
	Vector3 targetPosition;	// Considering only X axis
	float leftLimit;
	float rightLimit;

protected:
	virtual void update(float deltaTime);

public:
	PlatformMovement(StateMachine* stateMachine);
	virtual ~PlatformMovement();


	void setPlatformGraph(PlatformGraph* platformGraph);
	void setCharacter(GameObject* character);
	void setTargetPosition(const Vector3& position);
	void setLimits(float left, float right);

	bool differentPlatforms();

};

#endif