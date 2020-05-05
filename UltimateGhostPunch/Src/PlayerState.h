#pragma once
#ifndef PLAYER_STATE_H
#define PLAYER_STATE_H

#include <UserComponent.h>

class Attack;
class Block;
class Dodge;
class Grab;
class Movement;
class UltimateGhostPunch;
class GhostManager;

class PlayerState : public UserComponent
{
private:
	Attack* attack;
	Block* block;
	Dodge* dodge;
	Grab* grab;
	Movement* movement;
	GhostManager* ghostManager;
	UltimateGhostPunch* ghostPunch;

public:
	PlayerState(GameObject* gameObject);
	virtual ~PlayerState();

	virtual void start();

	bool canAttack() const;
	bool canBlock() const;
	bool canDodge() const;
	bool canGrab() const;
	bool canMove() const;
	bool canJump() const;
	bool canGhostMove() const;
};

#endif