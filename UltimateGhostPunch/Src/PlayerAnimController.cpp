#include "PlayerAnimController.h"

#include <sstream>
#include <ComponentRegister.h>
#include <GameObject.h>

#include <InputSystem.h>
#include "Animator.h"

#include "Attack.h"
#include "Jump.h"
#include "Health.h"
#include "GhostManager.h"
#include "Dodge.h"
#include "UltimateGhostPunch.h"
#include "Grab.h"
#include "Block.h"
#include "GameManager.h"
#include "RigidBody.h"


REGISTER_FACTORY(PlayerAnimController);

PlayerAnimController::PlayerAnimController(GameObject* gameObject) : UserComponent(gameObject), inputSystem(nullptr), body(nullptr), anim(nullptr), jump(nullptr), state(IDLE), runThreshold(0.50f), fallThreshold(1.0f)
{
}

void PlayerAnimController::start()
{
	inputSystem = InputSystem::GetInstance();
	anim = gameObject->getComponent<Animator>();
	body = gameObject->getComponent<RigidBody>();
	std::vector<GameObject*> aux = gameObject->findChildrenWithTag("groundSensor");
	if (aux.size() > 0) 
	{
		jump = aux[0]->getComponent<Jump>();
	}

	if (anim == nullptr)
		LOG("ERROR: Animator component not found in player.\n");
	else
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

void PlayerAnimController::jumpAnimation()
{
	if (anim->getCurrentAnimation() == "JumpStart" || anim->getCurrentAnimation() == "JumpChange" || anim->getCurrentAnimation() == "Fall")
		return;

	anim->playAnimation("JumpStart");
	anim->setLoop(false);
	state = JUMP;
}

void PlayerAnimController::updateState()
{
	switch (state)
	{
	case PlayerAnimController::IDLE:				// IDLE //
		updateIdle();
		break;
	case PlayerAnimController::RUN:					// RUN //
		updateRun();
		break;
	case PlayerAnimController::JUMP:				// JUMP //
		updateJump();
		break;
	case PlayerAnimController::FALL:				// FALL //
		updateFall();
		break;
	case PlayerAnimController::BLOCKING:			// BLOCKING // 
		break;
	case PlayerAnimController::GRABBING:			//  GRABBING //
		break;
	case PlayerAnimController::GRABBED:				// GRABBED //
		break;
	case PlayerAnimController::NOT_LOOPING_STATE:	// NOT_LOOPING_STATE //
		updateNotLoopingState();
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
	case PlayerAnimController::FALL:					// FALL //
		if (anim->getCurrentAnimation() != "Fall")
		{
			anim->playAnimation("Fall");
			anim->setLoop(true);
		}
		break;
	case PlayerAnimController::JUMP:					// JUMP //
		
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


void PlayerAnimController::updateIdle()
{
	// TRANSITION TO FALL IF
	if (!jump->isGrounded())
	{
		state = FALL;
		return;
	}

	// TRANSITION TO RUN IF
	if (abs(body->getLinearVelocity().x) >= runThreshold)
	{
		state = RUN;
		return;
	}

}

void PlayerAnimController::updateRun()
{
	// TRANSITION TO FALL IF
	if (!jump->isGrounded())
	{
		state = FALL;
		return;
	}

	// TRANSITION TO IDLE IF
	if (abs(body->getLinearVelocity().x) < runThreshold)
	{
		state = IDLE;
		return;
	}
}

void PlayerAnimController::updateJump()
{
	if (anim->getCurrentAnimation() == "JumpStart" && anim->hasEnded() && abs(body->getLinearVelocity().y) <= fallThreshold)
	{
		anim->playAnimationSequence({ "JumpChange", "Fall" }, true);
		anim->setLoop(false);
		return;
	}

	if (anim->getCurrentAnimation() == "Fall") // Player is falling: transition to Fall
		state = FALL;
}

void PlayerAnimController::updateFall()
{
	if (!jump->isGrounded())
		return;

	// If isGrounded, the player has landed
	anim->playAnimation("Land");
	anim->setLoop(false);
	state = NOT_LOOPING_STATE;
}

void PlayerAnimController::updateNotLoopingState()
{
	// Only transition when the animation has finished
	if (!anim->getLoop() && !anim->hasEnded())
		return;

	// TRANSITION TO FALL IF
	if (!jump->isGrounded())
	{
		state = FALL;
		return;
	}

	// TRANSITION TO IDLE IF
	if (abs(body->getLinearVelocity().x) < runThreshold)
	{
		state = IDLE;
		return;
	}
	else
	{
		// TRANSITION TO RUN
		state = RUN;
		return;
	}
}


