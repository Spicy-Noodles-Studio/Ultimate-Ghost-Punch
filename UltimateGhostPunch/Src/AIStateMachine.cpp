#include "AIStateMachine.h"
#include "PlatformNavigation.h"

#include "Movement.h"
#include "Jump.h"
#include "Dodge.h"

AIStateMachine::AIStateMachine(GameObject* gameObject) : StateMachine(gameObject), movement(nullptr), jump(nullptr), dodge(nullptr)
{

}

AIStateMachine::~AIStateMachine()
{

}

void AIStateMachine::start()
{
	// Create states here

	/* MOVING PLATFORM STATE ACTION */
	createMovingPlatformsAction();
}

void AIStateMachine::processActionInput()
{
	for (auto input : actionInputs) {
		switch (input)
		{
			/* MOVEMENT */
		case ActionInput::MOVE_RIGHT:
			movement->move(Vector3::RIGHT);
			break;
		case ActionInput::MOVE_LEFT:
			movement->move(Vector3::NEGATIVE_RIGHT);
			break;
		case ActionInput::JUMP:
			jump->jump();
			break;
		case ActionInput::CANCEL_JUMP:
			jump->cancelJump();
			break;
		case ActionInput::DODGE:
			dodge->dodge();
			break;

			/* ATTACK */
		default:
			LOG("ActionInput no procesado");
			break;
		}
	}
}

void AIStateMachine::createMovingPlatformsAction()
{
	PlatformNavigation* platformNavigation = new PlatformNavigation(this);
	addStateAction(platformNavigation);

	/* ADD MORE DATA IF NEEDED */
	/* GRAPH DATA */

	// TODO: quitar cuando se unifiquen las IAs
	currentState = platformNavigation;
}
