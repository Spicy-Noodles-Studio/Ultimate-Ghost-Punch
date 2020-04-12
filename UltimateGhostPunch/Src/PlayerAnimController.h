#pragma once
#include <UserComponent.h>

class InputSystem;
class Animator;

class PlayerAnimController :
	public UserComponent
{
private:
	// Input
	InputSystem* inputSystem;
	// Animator
	Animator* anim;

	enum PlayerAnimState
	{
		IDLE, RUN, JUMP, AIR, BLOCKING, GRABBING, GRABBED, NOT_LOOPING_STATE
	};

	// Current player state
	PlayerAnimState state;

	// Checks if there has been a change of state and updates the state if that is the case
	void updateState();
	// For the current state of the player, plays the appropriate animation
	void handleState();

	// Each of these functions checks if the state should transition to another one and makes the transition
	void updateIdle(); // IDLE
public:
	PlayerAnimController(GameObject* gameObject);


	virtual void start();
	virtual void update(float deltaTime);
};

