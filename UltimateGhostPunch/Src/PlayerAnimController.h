#pragma once
#include <UserComponent.h>

class InputSystem;
class Animator;
class RigidBody;
class Jump;

class PlayerAnimController :
	public UserComponent
{
private:
	// Input
	InputSystem* inputSystem;
	// Animator
	Animator* anim;
	// RigidBody
	RigidBody* body;
	// Jump
	Jump* jump;

	enum PlayerAnimState
	{
		IDLE, RUN, JUMP, FALL, BLOCKING, GRABBING, GRABBED, NOT_LOOPING_STATE
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

	
public:
	PlayerAnimController(GameObject* gameObject);


	virtual void start();
	virtual void update(float deltaTime);

	void jumpAnimation();
private:
	// Each of these functions checks if the state should transition to another one and makes the transition
	void updateIdle(); // IDLE
	void updateRun(); // RUN
	void updateJump(); // JUMP
	void updateFall(); // FALL

	void updateNotLoopingState(); // NOT LOOPING STATE (block, attacks...)
};

