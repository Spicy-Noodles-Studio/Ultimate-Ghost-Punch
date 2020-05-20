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
class GhostMovement;
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
	GhostMovement* ghostMovement;
	GhostManager* ghostManager;
	UltimateGhostPunch* ghostPunch;
	Respawn* respawn;

	bool ignoringInput;
	int thrown;
	int taunt;
	bool grabbed;

public:
	PlayerState(GameObject* gameObject);
	virtual ~PlayerState();

	virtual void start();
	virtual void postUpdate(float deltaTime);

	bool canAttack() const;
	bool canBlock() const;
	bool canDodge() const;
	bool canGrab() const;
	bool canMove() const;
	bool canJump() const;
	bool canGhostMove() const;
	bool canTaunt() const;

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
	bool isGhostMoving() const;
	bool isPunching() const;
	bool isAiming() const;
	bool isGhost() const;
	bool isRespawning() const;
	bool isIgnoringInput() const;
	bool isFalling() const;
	bool isGrabbed() const;

	bool hasBlocked() const;
	bool hasLanded() const;
	bool hasJumped() const;
	bool hasHit() const;
	bool hasBlockedGrab() const;
	bool hasDroppedGrab() const;
	bool hasMissedGrab() const;
	bool hasGhostSucceeded() const;
	bool hasPunchSucceeded() const;
	bool hasPunchFailed() const; 
	bool hasGhostDied() const;
	bool hasKnightDied() const;
	bool hasBeenThrown() const;
	bool hasTaunted() const;
	bool isResurrecting() const;
	bool isDying() const;
	bool isAppearing() const;
	bool isDisappearing() const;
	bool isDead() const;

	void setGrabbed();
	void setThrown();
	void setTaunting();
	void setIgnoringInput(bool ignoreInput);
};

#endif