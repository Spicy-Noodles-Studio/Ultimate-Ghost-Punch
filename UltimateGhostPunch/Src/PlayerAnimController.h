#pragma once
#ifndef PLAYER_ANIM_CONTROLLER_H
#define PLAYER_ANIM_CONTROLLER_H

#include <UserComponent.h>
#include <queue>

class InputSystem;
class Animator;
class MeshRenderer;
class RigidBody;
class Jump;
class Grab;
class Block;
class GhostManager;
class PlayerFX;
class PlayerState;

class PlayerAnimController : public UserComponent
{
private:
	// Input
	InputSystem* inputSystem;
	// MeshRenderer
	MeshRenderer* mesh;
	// Animator
	Animator* anim;
	// RigidBody
	RigidBody* body;
	// Jump
	Jump* jump;
	// Grab
	Grab* grab;
	// Block
	Block* block;
	// Ghost Manager
	GhostManager* ghostManag;
	// Player FX
	PlayerFX* playerFX;
	// PlayerState
	PlayerState* playerState;

	enum PlayerAnimState
	{
		IDLE, RUN, JUMP, FALL, BLOCKING, GRABBING, GRABBED, STUNNED, NOT_LOOPING_STATE, CHARGING_UGP, UGP, TEST
	};

	struct DelayedAnimation
	{
		std::string name;
		float delayTime;
	};

	// Current player state
	PlayerAnimState state;

	// Variables for state change:

	// -- Threshold for -> RUN transition
	float runThreshold;
	// -- Threshold for -> FALL transition
	float fallThreshold;

	// Checks if there has been a change of state and updates the state if that is the case
	void updateState();
	// For the current state of the player, plays the appropriate animation
	void handleState();

	enum SwordState
	{
		HAND, SHEATHED
	};

	// Indicates the current position of the sword
	SwordState swordState;
	
	// Delayed animation
	std::queue<DelayedAnimation> delayedAnimations;

	// Delay for the thrown animation
	float thrownDelay;

	/****************/
	/*	  GHOST		*/
	/****************/

	std::string ghostMeshId, ghostMeshName;
	std::string aliveMeshId, aliveMeshName;

public:
	PlayerAnimController(GameObject* gameObject);
	virtual ~PlayerAnimController();

	virtual void start();
	virtual void postUpdate(float deltaTime);
	virtual void handleData(ComponentData* data);

	void manageAnimations();

	void manageTransitionAnimations();
	void manageKnightAnimations();
	void manageGhostAnimations();

	bool manageGroundedAnimations();
	bool manageAirAnimations();

	// GROUNDED ANIMATIONS
	bool manageJumpAnimation();
	bool manageRunAnimation();
	bool manageIdleAnimation();
	bool manageLandAnimation();
	bool manageGroundedAttackAnimation();
	bool manageBlockAnimations();
	bool manageBlockAttackAnimation();
	bool manageBlockGrabAnimation();
	bool manageGrabAnimations(); // Solo empezar a agarrar o terminar (fail)
	bool manageGrabIdleAnimation();
	bool manageGrabRunAnimation();
	bool manageGrabJumpAnimation();
	bool manageGrabLandAnimation();
	bool manageStunnedAnimations();
	bool manageTauntAnimation();

	// NOT GROUNDED ANIMATIONS
	bool manageFallAnimation();
	bool manageGrabFallAnimation();
	bool manageAirAttackAnimation();

	// BOTH GROUNDED AND NOT ANIMATIONS
	bool manageDashAnimation();
	bool manageGrabDashAnimation();
	bool manageHurtAnimation();
	bool manageGrabbedAnimations();

	// GHOST ANIMATION
	bool manageChargeAnimation();
	bool managePunchingAnimation();
	bool managePunchSuccessAnimation();
	bool managePunchFailAnimation();
	bool manageGhostMoveAnimation();
	bool manageGhostIdleAnimation();

	void jumpAnimation();
	void dashAnimation();
	void tauntAnimation();
	void hurtAnimation();
	void stunnedAnimation();
	void resurrectAnimation();

	void quickAttackAnimation();
	void strongAttackAnimation();

	void grabAnimation();
	void grabFailedAnimation();
	void grabbedByEnemyAnimation();

	void blockAnimation();
	void blockedAttackAnimation();
	void blockedEnemyGrabAnimation();
	void enemyBlockedMyGrabAnimation();

	void throwEnemyAnimation();
	void thrownAwayAnimation();

	// Play a not looping animation
	void notLoopAnimation(std::string name);

	// 0 for QUICK | 1 for STRONG
	void attackAnimation(int type);

	// Play animation "name" after a delay of "delay" seconds
	void playAnimationWithDelay(std::string name, float delay);

	/****************/
	/*	  GHOST		*/
	/****************/

	enum PlayerMode
	{
		ALIVE, GHOST
	};

	void enterMode(PlayerMode mode);
	void chargingGhostAnimation();
	void punchingGhostAnimation();
	void punchSuccessAnimation();

private:
	bool checkIdle();
	bool checkStartedFalling();

	// Each of these functions checks if the state should transition to another one and makes the transition
	void updateIdle();		// IDLE
	void updateRun();		// RUN
	void updateJump();		// JUMP
	void updateFall();		// FALL
	void updateGrabbing();  // GRABBING
	void updateGrabbed();   // GRABBED
	void updateBlocking();  // BLOCKING
	void updateStunned();	// STUNNED

	void updateNotLoopingState(); // NOT LOOPING STATE (block, attacks...)

	// Play delayed animations
	void updateDelayedAnimations(float deltaTime);

	/****************/
	/*	  GHOST		*/
	/****************/

	PlayerMode currentMode;
};

#endif