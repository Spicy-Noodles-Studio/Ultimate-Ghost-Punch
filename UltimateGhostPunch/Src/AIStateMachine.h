#pragma once
#ifndef AI_STATE_MACHINE_H
#define AI_STATE_MACHINE_H
#include "StateMachine.h"

class Movement;
class Dodge;
class Jump;

class PlatformNavigation;

enum class ActionInput {
	MOVE_RIGHT, MOVE_LEFT, JUMP, CANCEL_JUMP, DODGE, STOP
};

class AIStateMachine :	public StateMachine
{
public:
private:
	std::vector<GameObject*>* knights;
	/* Knight it would be focused */
	GameObject* target;

	/* COMPONENTS NEEDED */
	Movement* movement;
	Dodge* dodge;
	Jump* jump;
	Vector3 dir;

	/* States */
	PlatformNavigation* platformNavigation;

	/* Auxiliar variables */
	float timeTargetChange;
	float timerTargetChange;

public:
	AIStateMachine(GameObject* gameObject);
	virtual ~AIStateMachine();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void fixedUpdate(float deltaTime);

private:
	virtual void processActionInput();

	/* Call in start function */
	void createMovingPlatformsAction();

	/* Funcs */
	void changeTarget();
};

#endif