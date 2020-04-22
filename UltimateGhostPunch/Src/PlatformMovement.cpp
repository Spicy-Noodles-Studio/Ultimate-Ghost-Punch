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

	float limitOffset = 1.0f, targetOffset = 2.0f; // Tolerancia alta para cuando el target sea la posicion de un enemigo
	/* Off limits */
	if (targetPosition.x < leftLimit + limitOffset || targetPosition.x > rightLimit - limitOffset || differentPlatforms()){
		((AIStateMachine*)stateMachine)->startPlatformNavigation();//Change to platform navigation
		return;
	}

	/* If arrived */
	float diff = abs(targetPosition.x - currentPosition.x);
	if (diff < targetOffset) {
		stateMachine->addActionInput(ActionInput::STOP);
		return;
	}

	/* Move */
	ActionInput input = targetPosition.x < currentPosition.x ? ActionInput::MOVE_LEFT : ActionInput::MOVE_RIGHT;
	stateMachine->addActionInput(input);

	// DE MOMENTO, de manera random mete un jump y/o un dash
	//if (random(0.0, 100.0) < 15.0) stateMachine->addActionInput(ActionInput::JUMP);
	//if (random(0.0, 100.0) < 15.0) stateMachine->addActionInput(ActionInput::DODGE);
}

void PlatformMovement::setPlatformGraph(PlatformGraph* platformGraph)
{
	this->platformGraph = platformGraph;
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

bool PlatformMovement::differentPlatforms()
{
	return platformGraph != nullptr && platformGraph->getIndex(targetPosition) != platformGraph->getIndex(character->transform->getPosition());
}
