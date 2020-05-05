#include "PlayerState.h"
#include <ComponentRegister.h>
#include <GameObject.h>

#include "Attack.h"
#include "Block.h"
#include "Dodge.h"
#include "Grab.h"
#include "Movement.h"
#include "UltimateGhostPunch.h"
#include "GhostManager.h"

REGISTER_FACTORY(PlayerState);

PlayerState::PlayerState(GameObject* gameObject) : UserComponent(gameObject), attack(nullptr), block(nullptr), dodge(nullptr), grab(nullptr), movement(nullptr), ghostManager(nullptr), ghostPunch(nullptr)
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
	movement = gameObject->getComponent<Movement>();
	ghostManager = gameObject->getComponent<GhostManager>();
	ghostPunch = gameObject->getComponent<UltimateGhostPunch>();
}

bool PlayerState::canAttack() const
{
	return (ghostManager == nullptr || !ghostManager->isGhost()) && (block == nullptr || !block->isBlocking()) && (dodge == nullptr || !dodge->isDodging()) && (grab == nullptr || !grab->isGrabbing());
}

bool PlayerState::canBlock() const
{
	return (ghostManager == nullptr || !ghostManager->isGhost()) && (attack == nullptr || !attack->isAttacking()) && (dodge == nullptr || !dodge->isDodging()) && (grab == nullptr || !grab->isGrabbing());
}

bool PlayerState::canDodge() const
{
	return (ghostManager == nullptr || !ghostManager->isGhost()) && (block == nullptr || !block->isBlocking()) && (attack == nullptr || !attack->isAttacking()) && (grab == nullptr || !grab->isGrabbing());
}

bool PlayerState::canGrab() const
{
	return (ghostManager == nullptr || !ghostManager->isGhost()) && (block == nullptr || !block->isBlocking()) && (dodge == nullptr || !dodge->isDodging()) && (attack == nullptr || !attack->isAttacking());
}

bool PlayerState::canMove() const
{
	return  (ghostManager == nullptr || !ghostManager->isGhost()) && (block == nullptr || !block->isBlocking()) && (dodge == nullptr || !dodge->isDodging()) && (attack == nullptr || !attack->isAttacking());
}

bool PlayerState::canJump() const
{
	return (ghostManager == nullptr || !ghostManager->isGhost()) && (block == nullptr || !block->isBlocking());
}

bool PlayerState::canGhostMove() const
{
	return  (ghostManager != nullptr && ghostManager->isGhost()) && (ghostPunch == nullptr || !ghostPunch->isPunching());
}
