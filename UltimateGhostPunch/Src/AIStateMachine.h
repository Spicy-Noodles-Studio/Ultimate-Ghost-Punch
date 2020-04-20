#pragma once
#ifndef AI_STATE_MACHINE_H
#define AI_STATE_MACHINE_H
#include "StateMachine.h"

class Movement;
class Dodge;
class Jump;
class GhostMovement;
class UltimateGhostPunch;

class PlatformNavigation;
class GhostNavigation;
class GhostManager;

enum class ActionInput {
	MOVE_RIGHT, MOVE_LEFT, JUMP, CANCEL_JUMP, DODGE, STOP, GHOST_MOVE, GHOST_PUNCH
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
	GhostMovement* ghostMovement;
	UltimateGhostPunch* ghostPunch;
	GhostManager* ghostManager;

	/* States */
	PlatformNavigation* platformNavigation;
	GhostNavigation* ghostNavigation;

	/* Auxiliar variables */
	Vector3 dir;
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
	void createGhostAction();

	/* Funcs */
	void changeTarget();
	void updateState();
};

#endif