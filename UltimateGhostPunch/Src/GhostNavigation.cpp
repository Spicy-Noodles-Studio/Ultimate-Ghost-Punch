#include "GhostNavigation.h"

#include <GameObject.h>

#include "AIStateMachine.h"


GhostNavigation::GhostNavigation(StateMachine* stateMachine) : StateAction(stateMachine), direction(Vector3::ZERO), target(nullptr), character(nullptr)
{
}

GhostNavigation::~GhostNavigation()
{
}

void GhostNavigation::setTarget(GameObject* target)
{
	this->target = target;
}

void GhostNavigation::setCharacter(GameObject* character)
{
	this->character = character;
}

Vector3 GhostNavigation::getDirection() const
{
	return direction;
}

void GhostNavigation::update(float deltaTime)
{
	if (target != nullptr) {
		direction = target->transform->getPosition() - character->transform->getPosition();
		direction.normalize();	
		stateMachine->addActionInput(ActionInput::GHOST_MOVE);
	}
}
