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
#include "GhostMovement.h"
#include "UltimateGhostPunch.h"
#include "GhostManager.h"
#include "Respawn.h"

REGISTER_FACTORY(PlayerState);

PlayerState::PlayerState(GameObject* gameObject) : UserComponent(gameObject), attack(nullptr), block(nullptr), dodge(nullptr), grab(nullptr), movement(nullptr), jump(nullptr),
health(nullptr), ghostMovement(nullptr), ghostManager(nullptr), ghostPunch(nullptr), respawn(nullptr), thrown(0), taunt(0), grabbed(false), ignoringInput(false)
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
	checkNull(attack);

	aux = gameObject->findChildrenWithTag("groundSensor");
	if (aux.size() > 0)
	{
		block = aux[0]->getComponent<Block>();
		jump = aux[0]->getComponent<Jump>();
	}
	checkNull(block);
	checkNull(jump);

	aux = gameObject->findChildrenWithTag("grabSensor");
	if (aux.size() > 0)
		grab = aux[0]->getComponent<Grab>();
	checkNull(grab);

	dodge = gameObject->getComponent<Dodge>();
	movement = gameObject->getComponent<Movement>();
	health = gameObject->getComponent<Health>();
	ghostMovement = gameObject->getComponent<GhostMovement>();
	ghostManager = gameObject->getComponent<GhostManager>();
	ghostPunch = gameObject->getComponent<UltimateGhostPunch>();
	respawn = gameObject->getComponent<Respawn>();

	checkNull(dodge);
	checkNull(movement);
	checkNull(health);
	checkNull(ghostMovement);
	checkNull(ghostManager);
	checkNull(ghostPunch);
	checkNull(respawn);
}

void PlayerState::postUpdate(float deltaTime)
{
	if (taunt > 0)taunt--;
	if (thrown > 0)thrown--;
}

bool PlayerState::canAttack() const
{
	return (!notNull(ghostManager) || !ghostManager->isGhost()) && (!notNull(block) || !block->isBlocking()) && (!notNull(dodge) || !dodge->isDodging()) && (!notNull(grab) || !grab->isGrabbing());
}

bool PlayerState::canBlock() const
{
	return (!notNull(ghostManager) || !ghostManager->isGhost()) && (!notNull(attack) || !attack->isAttacking()) && (!notNull(dodge) || !dodge->isDodging()) && (!notNull(grab) || !grab->isGrabbing());
}

bool PlayerState::canDodge() const
{
	return (!notNull(ghostManager) || !ghostManager->isGhost()) && (!notNull(block)|| !block->isBlocking()) && (!notNull(attack) || !attack->isAttacking());
}

bool PlayerState::canGrab() const
{
	return (!notNull(ghostManager) || !ghostManager->isGhost()) && (!notNull(block) || !block->isBlocking()) && (!notNull(dodge) || !dodge->isDodging()) && (!notNull(attack) || !attack->isAttacking());
}

bool PlayerState::canMove() const
{
	return  (!notNull(ghostManager) || !ghostManager->isGhost()) && (!notNull(block) || !block->isBlocking()) && (!notNull(dodge) || !dodge->isDodging()) && (!notNull(attack) || !attack->isAttacking());
}

bool PlayerState::canJump() const
{
	return (!notNull(ghostManager) || !ghostManager->isGhost()) && (!notNull(block) || !block->isBlocking());
}

bool PlayerState::canGhostMove() const
{
	return  (notNull(ghostManager) && ghostManager->isGhost()) && (!notNull(ghostPunch) || !ghostPunch->isPunching());
}

bool PlayerState::canTaunt() const
{
	return  (!notNull(ghostManager) || !ghostManager->isGhost()) && (!notNull(block) || !block->isBlocking()) && (!notNull(dodge) || !dodge->isDodging()) &&
		(!notNull(attack) || !attack->isAttacking()) && (!notNull(grab) || !grab->isGrabbing()) && (!notNull(jump) || jump->isGrounded());
}

bool PlayerState::isMoving() const
{
	return notNull(movement) && movement->isMoving();
}

bool PlayerState::isJumping() const
{
	return notNull(jump) && jump->isJumping();
}

bool PlayerState::isGrounded() const
{
	return notNull(jump) && jump->isGrounded();
}

bool PlayerState::isGrabbing() const
{
	return notNull(grab) && grab->isGrabbing();
}

bool PlayerState::isHurt() const
{
	return notNull(health) && health->isHurt();
}

bool PlayerState::isBlocking() const
{
	return notNull(block) && block->isBlocking();
}

bool PlayerState::isStunned() const
{
	return notNull(grab) && grab->isStunned();
}

bool PlayerState::isDodging() const
{
	return notNull(dodge) && dodge->isDodging();
}

bool PlayerState::isHeavyAttacking() const
{
	return notNull(attack) && attack->isHeavyAttacking();
}

bool PlayerState::isQuickAttacking() const
{
	return notNull(attack) && attack->isQuickAttacking();
}

bool PlayerState::isGhostMoving() const
{
	return notNull(ghostMovement) && ghostMovement->isGhostMoving();
}

bool PlayerState::isPunching() const
{
	return (notNull(ghostManager) && ghostManager->isGhost()) && (notNull(ghostPunch) && ghostPunch->isPunching());
}

bool PlayerState::isAiming() const
{
	return (notNull(ghostManager) && ghostManager->isGhost()) && (notNull(ghostPunch) && ghostPunch->isAiming());
}

bool PlayerState::isGhost() const
{
	return (notNull(ghostManager) && ghostManager->isGhost());
}

bool PlayerState::isRespawning() const
{
	return notNull(respawn) && respawn->isRespawning();
}

bool PlayerState::isIgnoringInput() const
{
	return ignoringInput;
}
bool PlayerState::isFalling() const
{
	return notNull(jump) && jump->isFalling();
}

bool PlayerState::isGrabbed() const
{
	return grabbed;
}

bool PlayerState::hasBlocked() const
{
	return notNull(block) && block->hasBlocked();
}

bool PlayerState::hasLanded() const
{
	return (notNull(ghostManager) || !ghostManager->isGhost()) && notNull(jump) && jump->hasLanded();
}

bool PlayerState::hasJumped() const
{
	return notNull(jump) && jump->hasJumped();
}

bool PlayerState::hasHit() const
{
	return notNull(attack) && attack->hasHit();
}

bool PlayerState::hasBlockedGrab() const
{
	return notNull(block) && block->hasBlockedGrab();
}

bool PlayerState::hasDroppedGrab() const
{
	return notNull(grab) && grab->hasDropped();
}

bool PlayerState::hasMissedGrab() const
{
	return notNull(grab) && grab->hasMissed();
}

bool PlayerState::hasGhostSucceeded() const
{
	return notNull(ghostManager) && ghostManager->ghostSuccess() && !hasPunchSucceeded();
}

bool PlayerState::hasPunchSucceeded() const
{
	return notNull(ghostPunch) && ghostPunch->punchSuccess();
}

bool PlayerState::hasPunchFailed() const
{
	return notNull(ghostPunch) && ghostPunch->punchFail();
}

bool PlayerState::hasGhostDied() const
{
	return notNull(ghostManager) && ghostManager->ghostDeath();
}

bool PlayerState::hasKnightDied() const
{
	return notNull(health) && !health->isAlive();
}

bool PlayerState::hasBeenThrown() const
{
	return thrown > 0;
}

void PlayerState::setIgnoringInput(bool ignoreInput)
{
	ignoringInput = ignoreInput;
}
bool PlayerState::hasTaunted() const
{
	return taunt > 0;
}

bool PlayerState::isResurrecting() const
{
	return notNull(ghostManager) && ghostManager->isResurrecting();
}

bool PlayerState::isDying() const
{
	return notNull(ghostManager) && ghostManager->isDying();
}

bool PlayerState::isAppearing() const
{
	return notNull(ghostManager) && ghostManager->isAppearing();
}

bool PlayerState::isDisappearing() const
{
	return notNull(ghostManager) && ghostManager->isDisappearing();
}

bool PlayerState::isDead() const
{
	return notNull(ghostManager) && ghostManager->isDead();
}

void PlayerState::setGrabbed()
{
	grabbed = true;
}

void PlayerState::setThrown()
{
	grabbed = false;
	thrown = 2;
}

void PlayerState::setTaunting()
{
	taunt = 2;
}
