#pragma once
#ifndef AI_STATE_MACHINE_H
#define AI_STATE_MACHINE_H
#include "StateMachine.h"

class Movement;
class Dodge;
class Jump;
class GhostMovement;
class UltimateGhostPunch;

class PlatformGraph;
class PlatformNavigation;
class PlatformMovement;
class GhostNavigation;
class GhostManager;
class Attack;
class Block;
class FightingState;

enum class ActionInput {
	MOVE_RIGHT, MOVE_LEFT, JUMP, CANCEL_JUMP, DODGE, STOP, GHOST_MOVE, GHOST_PUNCH, QUICK_ATTACK, STRONG_ATTACK, BLOCK, UNBLOCK
};

class AIStateMachine :	public StateMachine
{
public:
private:
	std::vector<GameObject*>* knights;
	/* Knight it would be focused */
	GameObject* target;

	/* GRAPH INFO */
	PlatformGraph* platformGraph;

	/* COMPONENTS NEEDED */
	Movement* movement;
	Dodge* dodge;
	Jump* jump;
	GhostMovement* ghostMovement;
	UltimateGhostPunch* ghostPunch;
	GhostManager* ghostManager;
	Attack* attack;
	Block* block;

	/* States */
	PlatformNavigation* platformNavigation;
	PlatformMovement* platformMovement;
	GhostNavigation* ghostNavigation;
	FightingState* fightingState;

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

	void startPlatformNavigation();
	void startPlatformMovement();
	void startGhostNavigation();
	void startFightingState();
	void startFleeingState(GameObject* fleeTarget);

	void changeTarget();

private:
	virtual void processActionInput();

	/* Call in start function */
	void createPlatformNavigation();
	void createPlatformMovement();
	void createGhostNavigation();
	void createFightingState();

	/* Funcs */
	void updateState();
};

#endif