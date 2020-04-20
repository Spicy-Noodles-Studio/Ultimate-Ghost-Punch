#pragma once
#include <UserComponent.h>
#include <queue>

class InputSystem;
class Animator;
class RigidBody;
class Jump;
class Grab;
class Block;
class MeshRenderer;
class GhostManager;

class PlayerFX;

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
	// Ghost Manager
	GhostManager* ghostManag;

	PlayerFX* playerFX;

	enum PlayerAnimState
	{
		IDLE, RUN, JUMP, FALL, BLOCKING, GRABBING, GRABBED, STUNNED, NOT_LOOPING_STATE, CHARGING_UGP, UGP
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


	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

	void jumpAnimation();
	void hurtAnimation();
	void grabAnimation();
	void grabFailedAnimation();
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

	// Each of these functions checks if the state should transition to another one and makes the transition
	void updateIdle();		// IDLE
	void updateRun();		// RUN
	void updateJump();		// JUMP
	void updateFall();		// FALL
	void updateGrabbing();  // GRABBING
	void updateGrabbed();   // GRABBED
	void updateBlocking();  // BLOCKING
	void updateStunned();	// STUNNED
	void updateUGP();		// UGP

	void updateNotLoopingState(); // NOT LOOPING STATE (block, attacks...)

	// Play delayed animations
	void updateDelayedAnimations(float deltaTime);


	/****************/
	/*	  GHOST		*/
	/****************/

	PlayerMode currentMode;
};

