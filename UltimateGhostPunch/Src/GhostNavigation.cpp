#include "GhostNavigation.h"

#include <GameObject.h>

#include "AIStateMachine.h"
#include "UltimateGhostPunch.h"
#include "GameManager.h"
#include "PlayerState.h"

GhostNavigation::GhostNavigation(StateMachine* stateMachine) : StateAction(stateMachine), direction(Vector3::ZERO), target(nullptr), character(nullptr), punch(nullptr),
charging(false), punchChargeDist(5.0f), chargeTime(10.0f), punchFailFactor(7.0f), timer(0.0f), changeTime(5.0f)
{
}

GhostNavigation::~GhostNavigation()
{
	target = nullptr;
	character = nullptr;
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

	timer -= deltaTime;
	if (timer < 0) timer = 0;

	selectTarget();

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
		if (notNull(stateMachine)) stateMachine->addActionInput(ActionInput::GHOST_MOVE);
	}
}

void GhostNavigation::selectTarget()
{
	bool changeTarget = false;
	if (notNull(target)) {
		PlayerState* targetState = target->getComponent<PlayerState>();
		if (notNull(targetState))
			changeTarget = targetState->isGhost() || targetState->isDead();
	}

	if (changeTarget || timer <= 0) {
		timer = changeTime;
		float dist = 0, minDist = INFINITY;

		checkNullAndBreak(GameManager::GetInstance());
		std::vector<GameObject*> knights = GameManager::GetInstance()->getKnights();
		GameObject* closestKnight = nullptr;
		PlayerState* knightState = nullptr;

		for (GameObject* knight : knights) {
			if (notNull(knight)) {
				knightState = knight->getComponent<PlayerState>();
				if (notNull(knightState) && !knightState->isDead() && !knightState->isGhost()) {
					if (notNull(knight->transform) && notNull(character) && notNull(character->transform)) {
						dist = (knight->transform->getPosition() - character->transform->getPosition()).magnitude();
						if (dist < minDist) {
							minDist = dist;
							closestKnight = knight;
						}
					}
				}
			}
		}
		if (closestKnight != nullptr) target = closestKnight;
	}
}
