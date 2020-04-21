#include "PlatformMovement.h"
#include "AIStateMachine.h"

#include <GameObject.h>
#include <MathUtils.h>

PlatformMovement::PlatformMovement(StateMachine* stateMachine) : StateAction(stateMachine)
{

}

PlatformMovement::~PlatformMovement()
{

}

void PlatformMovement::update(float deltaTime)
{
	if (character == nullptr) return;
	Vector3 currentPosition = character->transform->getPosition();

	float tolerance = 2.0f; // Tolerancia alta para cuando el target sea la posicion de un enemigo
	/* Off limits */
	if (targetPosition.x < leftLimit + tolerance || targetPosition.x > rightLimit - tolerance){
		stateMachine->addActionInput(ActionInput::STOP);
		//MAYBE CHANGE TARGET OR STATE
		//OR LIMITS
		return;
	}

	/* If arrived */
	float diff = abs(targetPosition.x - currentPosition.x);
	if (diff < tolerance) {
		stateMachine->addActionInput(ActionInput::STOP);
		return;
	}

	/* Move */
	ActionInput input = targetPosition.x < currentPosition.x ? ActionInput::MOVE_LEFT : ActionInput::MOVE_RIGHT;
	stateMachine->addActionInput(input);

	// DE MOMENTO, de manera random mete un jump y/o un dash
	if (random(0.0, 100.0) < 15.0) stateMachine->addActionInput(ActionInput::JUMP);
	if (random(0.0, 100.0) < 15.0) stateMachine->addActionInput(ActionInput::DODGE);
}

void PlatformMovement::setCharacter(GameObject* character)
{
	this->character = character;
}

void PlatformMovement::setTargetPosition(const Vector3& position)
{
	targetPosition = position;
}

void PlatformMovement::setLimits(float left, float right)
{
	leftLimit = left;
	rightLimit = right;
}