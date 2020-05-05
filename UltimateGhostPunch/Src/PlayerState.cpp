#include "PlayerState.h"
#include <ComponentRegister.h>
#include <GameObject.h>

#include "Attack.h"
#include "Block.h"
#include "Dodge.h"
#include "Grab.h"

REGISTER_FACTORY(PlayerState);

PlayerState::PlayerState(GameObject* gameObject) : UserComponent(gameObject), attack(nullptr), block(nullptr), dodge(nullptr), grab(nullptr)
{

}

PlayerState::~PlayerState()
{

}

void PlayerState::start()
{
	std::vector<GameObject*> aux = gameObject->findChildrenWithTag("attackSensor");
	if (aux.size() > 0)
		attack = aux[0]->getComponent<Attack>();

	aux = gameObject->findChildrenWithTag("groundSensor");
	if (aux.size() > 0)
		block = aux[0]->getComponent<Block>();

	aux = gameObject->findChildrenWithTag("grabSensor");
	if (aux.size() > 0)
		grab = aux[0]->getComponent<Grab>();

	dodge = gameObject->getComponent<Dodge>();
}

bool PlayerState::canAttack() const
{
	return !block->isBlocking() && !dodge->isDodging() && !grab->isGrabbing();
}

bool PlayerState::canBlock() const
{
	return !attack->isAttacking() && !dodge->isDodging() && !grab->isGrabbing();
}

bool PlayerState::canDodge() const
{
	return !attack->isAttacking() && !block->isBlocking() && !grab->isGrabbing();
}

bool PlayerState::canGrab() const
{
	return !attack->isAttacking() && !block->isBlocking() && !dodge->isDodging();
}