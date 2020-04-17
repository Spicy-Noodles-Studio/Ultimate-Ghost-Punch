#pragma once
#include <UserComponent.h>

class InputSystem;
class Animator;
class RigidBody;
class Jump;
class Grab;
class Block;
class MeshRenderer;

class PlayerAnimController :
	public UserComponent
{
private:
	// MeshRenderer
	MeshRenderer* mesh;
	// Input
	InputSystem* inputSystem;
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

	enum PlayerAnimState
	{
		IDLE, RUN, JUMP, FALL, BLOCKING, GRABBING, GRABBED, STUNNED, NOT_LOOPING_STATE
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
	
public:
	PlayerAnimController(GameObject* gameObject);


	virtual void start();
	virtual void update(float deltaTime);

	void jumpAnimation();
	void hurtAnimation();
	void grabAnimation();
	void quickAttackAnimation();
	void strongAttackAnimation();
	void blockAnimation();
	void blockedAttackAnimation();
	void blockedEnemyGrabAnimation();
	void enemyBlockedMyGrabAnimation();
	void stunnedAnimation();
	void dashAnimation();
	void resurrectAnimation();
	void tauntAnimation();
	void throwEnemyAnimation();
	void thrownAwayAnimation();
	void grabbedByEnemyAnimation();
private:
	// Play a not looping animation
	void notLoopAnimation(std::string name);
	// 0 for QUICK | 1 for STRONG
	void attackAnimation(int type);

	// Each of these functions checks if the state should transition to another one and makes the transition
	void updateIdle();		// IDLE
	void updateRun();		// RUN
	void updateJump();		// JUMP
	void updateFall();		// FALL
	void updateGrabbing();  // GRABBING
	void updateGrabbed();  // GRABBED
	void updateBlocking();  // BLOCKING
	void updateStunned();	// STUNNED

	void updateNotLoopingState(); // NOT LOOPING STATE (block, attacks...)
};

