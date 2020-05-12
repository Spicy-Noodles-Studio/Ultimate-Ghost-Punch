#pragma once
#ifndef PLAYER_STATE_H
#define PLAYER_STATE_H

#include <UserComponent.h>

class Attack;
class Block;
class Dodge;
class Grab;
class Movement;
class Jump;
class Health;
class GhostManager;
class UltimateGhostPunch;
class Respawn;

class PlayerState : public UserComponent
{
private:
	Attack* attack;
	Block* block;
	Dodge* dodge;
	Grab* grab;
	Movement* movement;
	Jump* jump;
	Health* health;
	GhostManager* ghostManager;
	UltimateGhostPunch* ghostPunch;
	Respawn* respawn;

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

	bool isMoving() const;
	bool isJumping() const;
	bool isGrounded() const;
	bool isGrabbing() const;
	bool isHurt() const;
	bool isBlocking() const;
	bool isStunned() const;
	bool isDodging() const;
	bool isHeavyAttacking() const;
	bool isQuickAttacking() const;
	bool isPunching() const;
	bool isAiming() const;
	bool punchSucceeded() const;
	bool isGhost() const;
	bool isRespawning() const;

	bool hasBlocked() const;
	bool hasLanded() const;
	bool hasHit() const;
	bool hasBlockedGrab() const;
	bool hasDroppedGrab() const;
	bool hasMissedGrab() const;
	bool hasGhostSucceeded() const;
	bool hasPunchSucceeded() const;
	bool hasGhostDied() const;
	bool hasKnightDied() const;
};

#endif