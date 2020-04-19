#pragma once
#ifndef AI_STATE_MACHINE_H
#define AI_STATE_MACHINE_H
#include "StateMachine.h"

class Movement;
class Dodge;
class Jump;

enum class ActionInput {
	MOVE_RIGHT, MOVE_LEFT, JUMP, CANCEL_JUMP, DODGE, STOP
};

class AIStateMachine :	public StateMachine
{
public:
private:
	/* COMPONENTS NEEDED */
	Movement* movement;
	Dodge* dodge;
	Jump* jump;
	Vector3 dir;

public:
	AIStateMachine(GameObject* gameObject);
	virtual ~AIStateMachine();

	virtual void start();
	virtual void fixedUpdate(float deltaTime);

private:
	virtual void processActionInput();

	/* Call in start function */
	void createMovingPlatformsAction();
};

#endif