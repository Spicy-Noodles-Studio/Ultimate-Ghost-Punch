#include "PlayerAnimController.h"

#include <sstream>
#include <ComponentRegister.h>
#include <GameObject.h>

#include <InputSystem.h>

#include "Attack.h"
#include "Jump.h"
#include "Health.h"
#include "GhostManager.h"
#include "Dodge.h"
#include "UltimateGhostPunch.h"
#include "Animator.h"
#include "Grab.h"
#include "Block.h"
#include "GameManager.h"


REGISTER_FACTORY(PlayerAnimController);

void PlayerAnimController::updateIdle()
{

}

PlayerAnimController::PlayerAnimController(GameObject* gameObject) : UserComponent(gameObject), inputSystem(nullptr), state(IDLE)
{
}

void PlayerAnimController::start()
{
	inputSystem = InputSystem::GetInstance();
	anim = gameObject->getComponent<Animator>();

	anim->printAllAnimationsNames();
}

void PlayerAnimController::update(float deltaTime)
{
	// Update the current state
	updateState();

	// Handle the current state
	handleState();

	anim->updateAnimationSequence();
}

void PlayerAnimController::updateState()
{
	switch (state)
	{
	case PlayerAnimController::IDLE:				// IDLE //
		updateIdle();
		break;
	case PlayerAnimController::RUN:					// RUN //
		break;
	case PlayerAnimController::AIR:					// AIR //
		break;
	case PlayerAnimController::BLOCKING:			// BLOCKING // 
		break;
	case PlayerAnimController::GRABBING:			//  GRABBING //
		break;
	case PlayerAnimController::GRABBED:				// GRABBED //
		break;
	case PlayerAnimController::NOT_LOOPING_STATE:	// NOT_LOOPING_STATE //
		break;
	default:
		break;
	}
}

void PlayerAnimController::handleState()
{
	switch (state)
	{
	case PlayerAnimController::IDLE:				// IDLE //
		if (anim->getCurrentAnimation() != "Idle")
		{
			anim->playAnimation("Idle");
			anim->setLoop(true);
		}
		break;
	case PlayerAnimController::RUN:					// RUN //
		if (anim->getCurrentAnimation() != "Run")
		{
			anim->playAnimation("Run");
			anim->setLoop(true);
		}
		break;
	case PlayerAnimController::AIR:					// AIR //
		if (anim->getCurrentAnimation() != "Fall")
		{
			anim->playAnimation("Fall");
			anim->setLoop(true);
		}
		break;
	case PlayerAnimController::BLOCKING:			// BLOCKING // 
		if (anim->getCurrentAnimation() != "BlockHold")
		{
			anim->playAnimation("BlockHold");
			anim->setLoop(true);
		}
		break;
	case PlayerAnimController::GRABBING:			//  GRABBING //
		/*if (anim->getCurrentAnimation() != "Grabbing")
		{
			anim->playAnimation("Grabbing");
			anim->setLoop(true);
		}*/
		break;
	case PlayerAnimController::GRABBED:				// GRABBED //
		/*if (anim->getCurrentAnimation() != "Grabbed")
		{
			anim->playAnimation("Grabbed");
			anim->setLoop(true);
		}*/
		break;
	default:
		break;
	}
}
