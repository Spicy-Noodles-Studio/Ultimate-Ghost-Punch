#include "PlatformMovement.h"

#include <GameObject.h>
#include <MathUtils.h>

#include "AIStateMachine.h"

PlatformMovement::PlatformMovement(StateMachine* stateMachine) : StateAction(stateMachine), platformGraph(nullptr), character(nullptr),	
																 targetPosition(Vector3::ZERO), leftLimit(0.0f), rightLimit(0.0f)
{

}

PlatformMovement::~PlatformMovement()
{

}

void PlatformMovement::update(float deltaTime)
{
	if (!notNull(stateMachine) || !notNull(character) || !notNull(character->transform)) return;
	Vector3 currentPosition = character->transform->getPosition();

	double limitOffset = 1.0, targetOffset = 2.0; // High tolerance for when the target is the enemy
	/* Off limits */
	if (targetPosition.x < (double)leftLimit + limitOffset || targetPosition.x >(double)rightLimit - limitOffset || differentPlatforms()){
		((AIStateMachine*)stateMachine)->startPlatformNavigation();	// Change to platform navigation
		return;
	}

	/* If arrived */
	float diff = abs(targetPosition.x - currentPosition.x);
	if (diff < targetOffset) {
		stateMachine->addActionInput(ActionInput::STOP);
		((AIStateMachine*)stateMachine)->startFightingState();	// Change to fighting state
		return;
	}

	/* Move */
	ActionInput input = targetPosition.x < currentPosition.x ? ActionInput::MOVE_LEFT : ActionInput::MOVE_RIGHT;
	stateMachine->addActionInput(input);

	//if (random(0.0, 100.0) < 15.0) stateMachine->addActionInput(ActionInput::JUMP);
	//if (random(0.0, 100.0) < 15.0) stateMachine->addActionInput(ActionInput::DODGE);
}

void PlatformMovement::setPlatformGraph(PlatformGraph* platformGraph)
{
	checkNullAndBreak(platformGraph);

	this->platformGraph = platformGraph;
}

void PlatformMovement::setCharacter(GameObject* character)
{
	checkNullAndBreak(character);

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
	return notNull(platformGraph) && notNull(character->transform) && platformGraph->getIndex(targetPosition) != platformGraph->getIndex(character->transform->getPosition());
}
