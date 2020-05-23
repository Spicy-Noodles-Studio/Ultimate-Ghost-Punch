#include "GhostNavigation.h"

#include <GameObject.h>

#include "AIStateMachine.h"
#include "UltimateGhostPunch.h"


GhostNavigation::GhostNavigation(StateMachine* stateMachine) : StateAction(stateMachine), direction(Vector3::ZERO), target(nullptr), character(nullptr), punchChargeDist(5), charging(false), chargeTime(10000), punchFailFactor(7)
{
}

GhostNavigation::~GhostNavigation()
{
}

void GhostNavigation::setTarget(GameObject* target)
{
	checkNullAndBreak(target);

	this->target = target;
}

void GhostNavigation::setCharacter(GameObject* character)
{
	checkNullAndBreak(character);

	this->character = character;
	punch = character->getComponent<UltimateGhostPunch>();
}

Vector3 GhostNavigation::getDirection() const
{
	return direction;
}

void GhostNavigation::update(float deltaTime)
{
	if (charging) chargeTime -= deltaTime;

	if (notNull(target) && notNull(target->transform) && notNull(character) && notNull(character->transform)) {
		direction = target->transform->getPosition() - character->transform->getPosition();
		if (notNull(punch) && !punch->isUsed())
		{
			float dist = direction.magnitude();
			if (!charging && dist < punchChargeDist)
			{
				chargeTime = (rand() % 5) + 1;
				punch->charge();
				charging = true;
			}
			else if (charging && chargeTime <= 0)
			{
				int range = (punchFailFactor - (-punchFailFactor)) + 1;
				float failY = -punchFailFactor + int(range * rand() / (RAND_MAX + 1.0));
				punch->aim(direction.x, direction.y + failY);
				punch->ghostPunch();
				return;
			}
		}
		
		direction.normalize();	
		if(notNull(stateMachine)) stateMachine->addActionInput(ActionInput::GHOST_MOVE);
	}
}
