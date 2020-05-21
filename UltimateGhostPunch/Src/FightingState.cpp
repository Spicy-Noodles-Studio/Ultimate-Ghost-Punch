#include "FightingState.h"
#include <time.h>
#include <MathUtils.h>

#include "GameObject.h"

#include "AIStateMachine.h"
#include "Attack.h"
#include "Health.h"
#include "GhostManager.h"
#include "Block.h"
#include "Jump.h"
#include "Grab.h"
#include "Dodge.h"
#include "Movement.h"
#include "PlayerState.h"

FightingState::FightingState(StateMachine* stateMachine) : StateAction(stateMachine), quickAttackProb_QAR(50), strongAttackProb_QAR(45), blockProb_QAR(5), strongAttackProb_SAR(60), seekProb_SAR(38), blockProb_SAR(2), blockSpamTimeMAX(10), grabProb(2), maxQuickAttacks(3), quickAttackCounter(0), lastAction(ActionInput::STOP), dodgeProb(5), maxDistForJump(2)
{
}

FightingState::~FightingState()
{
}

void FightingState::update(float deltaTime)
{
	if (blockSpamTime > 0) blockSpamTime -= deltaTime;
	if (unblockTime > 0) unblockTime -= deltaTime;
	selectAction();
}


bool FightingState::enemyInQuickAttackRange()
{
	return attack != nullptr && attack->isQuickAttackOnRange(target);
}

bool FightingState::enemyInStrongAttackRange()
{
	return attack != nullptr && attack->isStrongAttackOnRange(target);
}

void FightingState::selectAction()
{
	if (character == nullptr) return;

	Health* health = character->getComponent<Health>();
	if (health == nullptr || !health->isAlive() || health->isInvencible()) return;

	if (lastAction == ActionInput::DODGE && dodgeComp != nullptr && !dodgeComp->isDodging()) turnTowardsTarget(); // Turn towards target after dodge

	if (blockComp != nullptr && blockComp->isBlocking())
	{
		if (unblockTime <= 0) unblock();
		
		return;
	}

	if (attack != nullptr && attack->isAttacking()) // Wait until attack ends
		return;

	// If AI is over a player -> jump off
	if (jump != nullptr && jump->isAbovePlayer())
	{
		jump->jump();
		turnBackOnTarget();
		stateMachine->addActionInput(ActionInput::DODGE);
	}

	// Wait til the AI lands
	if (playerState != nullptr && !playerState->isGrounded()/* || pState->isJumping()*/)
		return;

	if (target == nullptr)return;

	Health* targetHealth = target->getComponent<Health>();
	GhostManager* targetGhostManager = target->getComponent<GhostManager>();

	if (targetHealth != nullptr && targetGhostManager != nullptr && (!targetHealth->isAlive() && !targetGhostManager->ghostUsed()) || targetGhostManager->isGhost()) // Flee if target is entering ghost mode
	{
		transitionToFlee();
		return;
	}
	
	if (targetHealth != nullptr && !targetHealth->isAlive()) // Change target if target is dead
	{
		transitionToPlatformNav();
		return;
	}


	// AIR COMBAT
	PlayerState* targetState = target->getComponent<PlayerState>();

	if (targetState != nullptr && !targetState->isGrounded() && target->transform != nullptr && character->transform != nullptr) // Check if target is jumping
	{
		float distX = abs(target->transform->getPosition().x - character->transform->getPosition().x);
		if (distX <= maxDistForJump)
		{
			if(jump != nullptr) jump->jump();
			turnTowardsTarget();
			ActionInput action = ActionInput::QUICK_ATTACK;
			if(stateMachine != nullptr) stateMachine->addActionInput(action);
			lastAction = action;
			return;
		}
	}

	int rnd = rand() % 100;
	// QUICK ATTACK RANGE
	if (enemyInQuickAttackRange())
	{
		if (rnd < quickAttackProb_QAR)								// Action: Quick Attack
			quickAttack();
		else if (rnd < quickAttackProb_QAR + strongAttackProb_QAR)	// Action: Strong Attack
			strongAttack();
		else														// Action: Try to shield
			if(blockSpamTime <= 0)
				block(); 
		return;
	}

	// STRONG ATTACK RANGE
	if (enemyInStrongAttackRange())
	{
		if (rnd < strongAttackProb_SAR)					// Action: Strong Attack
			strongAttack();
		else if (rnd < strongAttackProb_SAR + seekProb_SAR)					// Action: Transition to seek
			transitionToPlatformNav();
		else											// Action: Try to shield
			if (blockSpamTime <= 0)
				block();

		return;
	}

	// NOT IN RANGE OF ATTACK
	//LOG("GETTING CLOSER...\n");
	transitionToPlatformNav();


}

void FightingState::quickAttack()
{
	if (stateMachine == nullptr) return;

	ActionInput action = ActionInput::QUICK_ATTACK;
	if (lastAction != action) quickAttackCounter = 0;

	if (!attack->attackOnCD())
		quickAttackCounter++;

	// Max. consecutive quick attacks reached?
	if (quickAttackCounter < 3)
	{
		stateMachine->addActionInput(action);
		lastAction = action;
	}
	else if(dodgeComp != nullptr && !dodgeComp->isOnCooldown() && playerState != nullptr && playerState->canDodge())
	{
		turnBackOnTarget();
		dodge();
	}

}

void FightingState::strongAttack()
{
	if (stateMachine == nullptr) return;

	if (attack != nullptr && !attack->attackOnCD())
	{
		ActionInput action = ActionInput::STRONG_ATTACK;
		stateMachine->addActionInput(action);
		lastAction = action;
	}
	else
	{
		// AI decides to grab?
		int rnd = rand() % 100;
		if(rnd < grabProb)
			grab();
		else if(rnd  < dodgeProb + grabProb && dodgeComp != nullptr && !dodgeComp->isOnCooldown() && playerState != nullptr && playerState->canDodge() && lastAction != ActionInput::QUICK_ATTACK) // Don't dodge after a quick attack and check cooldown
		{
			turnBackOnTarget();
			dodge();
		}
	}
		
}

void FightingState::block()
{
	if (stateMachine == nullptr) return;

	if (blockComp != nullptr && playerState != nullptr && playerState->canBlock())
	{
		ActionInput action = ActionInput::BLOCK;
		stateMachine->addActionInput(action);

		float minUnblockTime = blockComp->getMaxBlockTime() / 4;
		unblockTime = minUnblockTime + (float)(rand()) / ((float)(RAND_MAX / (blockComp->getMaxBlockTime() - minUnblockTime)));
		blockSpamTime = blockSpamTimeMAX;
		lastAction = action;
	}
	else
		transitionToFlee();
	
}

void FightingState::unblock()
{
	if (stateMachine == nullptr) return;

	ActionInput action = ActionInput::UNBLOCK;
	stateMachine->addActionInput(action);
	lastAction = action;
}

void FightingState::transitionToPlatformNav()
{
	fighting = false;
	if(blockComp != nullptr) blockComp->unblock();

	if (stateMachine == nullptr) return;
	((AIStateMachine*)stateMachine)->changeTarget();
	((AIStateMachine*)stateMachine)->startPlatformNavigation();
}

void FightingState::transitionToFlee()
{
	fighting = false;
	if(blockComp != nullptr) blockComp->unblock();

	if (stateMachine == nullptr) return;
	((AIStateMachine*)stateMachine)->startFleeingState(target);
}

void FightingState::grab()
{
	if (stateMachine == nullptr) return;

	ActionInput action = ActionInput::GRAB;
	stateMachine->addActionInput(action);
	lastAction = action;
}

void FightingState::drop()
{
	if (stateMachine == nullptr) return;

	ActionInput action = ActionInput::DROP;
	stateMachine->addActionInput(action);
	lastAction = action;
}

void FightingState::dodge()
{
	if (stateMachine == nullptr) return;

	ActionInput action = ActionInput::DODGE;
	stateMachine->addActionInput(action);
	lastAction = action;
}

void FightingState::turnTowardsTarget()
{
	if (character->transform == nullptr || target->transform == nullptr) return;

	int x = character->transform->getPosition().x, targetX = target->transform->getPosition().x;
	Vector3 dir = x <= targetX ? dir = Vector3::RIGHT : Vector3::NEGATIVE_RIGHT;

	if (movement != nullptr) movement->move(dir);
}

void FightingState::turnBackOnTarget()
{
	if (character->transform == nullptr || target->transform == nullptr) return;

	int x = character->transform->getPosition().x, targetX = target->transform->getPosition().x;
	Vector3 dir = x <= targetX ? Vector3::NEGATIVE_RIGHT : Vector3::RIGHT;

	if (movement != nullptr) movement->move(dir);
}

void FightingState::setTarget(GameObject* target)
{
	checkNullAndBreak(target);
	this->target = target;
}

void FightingState::setCharacter(GameObject* character)
{
	checkNullAndBreak(character);
	this->character = character;

	auto aux = character->findChildrenWithTag("attackSensor");
	if (aux.size() > 0)
		attack = aux[0]->getComponent<Attack>();
	checkNull(attack);

	std::vector<GameObject*> ground = character->findChildrenWithTag("groundSensor");
	if (ground.size() > 0)
	{
		jump = ground[0]->getComponent<Jump>();
		blockComp = ground[0]->getComponent<Block>();
	}
	checkNull(jump);
	checkNull(blockComp);

	std::vector<GameObject*> grabSensor = character->findChildrenWithTag("grabSensor");
	if (grabSensor.size() > 0)
		grabComp = grabSensor[0]->getComponent<Grab>();
	checkNull(grabComp);

	dodgeComp = character->getComponent<Dodge>();
	movement = character->getComponent<Movement>();
	playerState = character->getComponent<PlayerState>();
	
	checkNull(dodgeComp);
	checkNull(movement);
	checkNull(playerState);

	lastAction = ActionInput::STOP;
}

void FightingState::setFighting(bool fighting)
{
	this->fighting = fighting;
}

bool FightingState::isFighting() const
{
	return fighting;
}
