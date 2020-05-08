#include "PlayerState.h"
#include <ComponentRegister.h>
#include <GameObject.h>

#include "Attack.h"
#include "Block.h"
#include "Dodge.h"
#include "Grab.h"
#include "Movement.h"
#include "Jump.h"
#include "Health.h"
#include "UltimateGhostPunch.h"
#include "GhostManager.h"
#include "Respawn.h"

REGISTER_FACTORY(PlayerState);

PlayerState::PlayerState(GameObject* gameObject) : UserComponent(gameObject), attack(nullptr), block(nullptr), dodge(nullptr), grab(nullptr), movement(nullptr), jump(nullptr), health(nullptr), ghostManager(nullptr), ghostPunch(nullptr)
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
	if (aux.size() > 0) {
		block = aux[0]->getComponent<Block>();
		jump = aux[0]->getComponent<Jump>();
	}

	aux = gameObject->findChildrenWithTag("grabSensor");
	if (aux.size() > 0)
		grab = aux[0]->getComponent<Grab>();

	dodge = gameObject->getComponent<Dodge>();
	movement = gameObject->getComponent<Movement>();
	health = gameObject->getComponent<Health>();
	ghostManager = gameObject->getComponent<GhostManager>();
	ghostPunch = gameObject->getComponent<UltimateGhostPunch>();
	respawn = gameObject->getComponent<Respawn>();
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
	return (ghostManager == nullptr || !ghostManager->isGhost()) && (block == nullptr || !block->isBlocking()) && (attack == nullptr || !attack->isAttacking());
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

bool PlayerState::isMoving() const
{
	return movement != nullptr && movement->isMoving();
}

bool PlayerState::isJumping() const
{
	return jump != nullptr && jump->isJumping();
}

bool PlayerState::isGrounded() const
{
	return jump != nullptr && jump->isGrounded();
}

bool PlayerState::isGrabbing() const
{
	return grab != nullptr && grab->isGrabbing();
}

bool PlayerState::isHurt() const
{
	return health != nullptr && health->isHurt();
}

bool PlayerState::isBlocking() const
{
	return block != nullptr && block->isBlocking();
}

bool PlayerState::isStunned() const
{
	return grab != nullptr && grab->isStunned();
}

bool PlayerState::isDodging() const
{
	return dodge != nullptr && dodge->isDodging();
}

bool PlayerState::isHeavyAttacking() const
{
	return attack!= nullptr && attack->isHeavyAttacking();
}

bool PlayerState::isQuickAttacking() const
{
	return attack != nullptr && attack->isQuickAttacking();
}

bool PlayerState::isPunching() const
{
	return (ghostManager != nullptr && ghostManager->isGhost()) && (ghostPunch != nullptr && ghostPunch->isPunching());
}

bool PlayerState::isAiming() const
{
	return (ghostManager != nullptr && ghostManager->isGhost()) && (ghostPunch != nullptr && ghostPunch->isAiming());
}

bool PlayerState::punchSucceeded() const
{
	return (ghostManager != nullptr && ghostManager->isGhost()) && (ghostPunch != nullptr && ghostPunch->punchSuccess());
}

bool PlayerState::isGhost() const
{
	return (ghostManager != nullptr && ghostManager->isGhost());
}

bool PlayerState::isRespawning() const
{
	return respawn != nullptr && respawn->isRespawning();
}

bool PlayerState::hasBlocked() const
{
	return block->hasBlocked();
}

bool PlayerState::hasLanded() const
{
	return jump != nullptr && jump->hasLanded();
}

bool PlayerState::hasHit() const
{
	return attack != nullptr && attack->hasHit();
}

bool PlayerState::hasBlockedGrab() const
{
	return isStunned();
}

bool PlayerState::hasDroppedGrab() const
{
	return grab != nullptr && grab->hasDropped();
}

bool PlayerState::hasMissedGrab() const
{
	return grab != nullptr && grab->hasMissed();
}

bool PlayerState::hasGhostSucceeded() const
{
	return ghostManager != nullptr && ghostManager->ghostSuccess();
}

bool PlayerState::hasGhostDied() const
{
	return ghostManager != nullptr && ghostManager->ghostDeath();
}
