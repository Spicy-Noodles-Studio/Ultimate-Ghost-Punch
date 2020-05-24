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

FightingState::FightingState(StateMachine* stateMachine) : StateAction(stateMachine), quickAttackProb_QAR(50), strongAttackProb_QAR(45), blockProb_QAR(5), strongAttackProb_SAR(60), seekProb_SAR(38),
blockProb_SAR(2), blockSpamTimeMAX(10), grabProb(2), maxQuickAttacks(3), quickAttackCounter(0), lastAction(ActionInput::STOP), dodgeProb(5), maxDistForJump(2), blockSpamTime(0), unblockTime(0), target(nullptr),
attack(nullptr), blockComp(nullptr), character(nullptr), dodgeComp(nullptr), fighting(false), grabComp(nullptr), jump(nullptr), movement(nullptr), playerState(nullptr)
{
}

FightingState::~FightingState()
{
	target = nullptr;
	attack = nullptr;
	blockComp = nullptr;
	character = nullptr;
	playerState = nullptr;
	dodgeComp = nullptr;
	movement = nullptr;
	grabComp = nullptr;
	jump = nullptr;
}

void FightingState::update(float deltaTime)
{
	if (blockSpamTime > 0) blockSpamTime -= deltaTime;
	if (unblockTime > 0) unblockTime -= deltaTime;
	selectAction();
}


bool FightingState::enemyInQuickAttackRange()
{
	return notNull(attack) && attack->isQuickAttackOnRange(target);
}

bool FightingState::enemyInStrongAttackRange()
{
	return notNull(attack) && attack->isStrongAttackOnRange(target);
}

void FightingState::selectAction()
{
	checkNullAndBreak(character);

	if (lastAction == ActionInput::DODGE && notNull(dodgeComp) && !dodgeComp->isDodging()) turnTowardsTarget(); // Turn towards target after dodge

	if (notNull(blockComp) && blockComp->isBlocking())
	{
		if (unblockTime <= 0) unblock();

		return;
	}

	if (notNull(attack) && attack->isAttacking()) // Wait until attack ends
		return;

	// If AI is over a player -> jump off
	if (notNull(jump) && notNull(stateMachine) && jump->isAbovePlayer())
	{
		jump->jump();
		turnBackOnTarget();
		stateMachine->addActionInput(ActionInput::DODGE);
	}

	checkNullAndBreak(target);

	Health* targetHealth = target->getComponent<Health>();
	GhostManager* targetGhostManager = target->getComponent<GhostManager>();

	if (notNull(targetHealth) && notNull(targetGhostManager) && (!targetHealth->isAlive() && !targetGhostManager->ghostUsed()) || targetGhostManager->isGhost()) // Flee if target is entering ghost mode
	{
		transitionToFlee();
		return;
	}

	if (notNull(targetHealth) && !targetHealth->isAlive()) // Change target if target is dead
	{
		transitionToPlatformNav();
		return;
	}

	// AIR COMBAT
	PlayerState* targetState = target->getComponent<PlayerState>();

	if (notNull(targetState) && !targetState->isGrounded() && notNull(target->transform) && notNull(character->transform)) // Check if target is jumping
	{
		float distX = abs(target->transform->getPosition().x - character->transform->getPosition().x);
		if (distX <= maxDistForJump)
		{
			if (notNull(jump)) jump->jump();
			turnTowardsTarget();
			ActionInput action = ActionInput::QUICK_ATTACK;
			if (notNull(stateMachine)) stateMachine->addActionInput(action);
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
		else if (blockSpamTime <= 0)								// Action: Try to shield 
			block();
		return;
	}

	// STRONG ATTACK RANGE
	if (enemyInStrongAttackRange())
	{
		if (rnd < strongAttackProb_SAR)							// Action: Strong Attack
			strongAttack();
		else if (rnd < strongAttackProb_SAR + seekProb_SAR)		// Action: Transition to seek
			transitionToPlatformNav();
		else if (blockSpamTime <= 0)							// Action: Try to shield
			block();

		return;
	}

	// NOT IN RANGE OF ATTACK
	//LOG("GETTING CLOSER...\n");
	transitionToPlatformNav();
}

void FightingState::quickAttack()
{
	checkNullAndBreak(stateMachine);

	ActionInput action = ActionInput::QUICK_ATTACK;
	if (lastAction != action) quickAttackCounter = 0;

	if (notNull(attack) && !attack->attackOnCD())
		quickAttackCounter++;

	// Max. consecutive quick attacks reached?
	if (quickAttackCounter < 3)
	{
		stateMachine->addActionInput(action);
		lastAction = action;
	}
	else if (notNull(dodgeComp) && !dodgeComp->isOnCooldown() && notNull(playerState) && playerState->canDodge())
	{
		turnBackOnTarget();
		dodge();
	}

}

void FightingState::strongAttack()
{
	checkNullAndBreak(stateMachine);

	if (notNull(attack) && !attack->attackOnCD())
	{
		ActionInput action = ActionInput::STRONG_ATTACK;
		stateMachine->addActionInput(action);
		lastAction = action;
	}
	else
	{
		// AI decides to grab?
		int rnd = rand() % 100;
		if (rnd < grabProb)
			grab();
		else if (rnd < dodgeProb + grabProb && notNull(dodgeComp) && !dodgeComp->isOnCooldown() && notNull(playerState) && playerState->canDodge() && lastAction != ActionInput::QUICK_ATTACK) // Don't dodge after a quick attack and check cooldown
		{
			turnBackOnTarget();
			dodge();
		}
	}
}

void FightingState::block()
{
	checkNullAndBreak(stateMachine);

	if (notNull(blockComp) && notNull(playerState) && playerState->canBlock())
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
	checkNullAndBreak(stateMachine);

	ActionInput action = ActionInput::UNBLOCK;
	stateMachine->addActionInput(action);
	lastAction = action;
}

void FightingState::transitionToPlatformNav()
{
	fighting = false;
	if (notNull(blockComp)) blockComp->unblock();

	checkNullAndBreak(stateMachine);

	((AIStateMachine*)stateMachine)->changeTarget();
	((AIStateMachine*)stateMachine)->startPlatformNavigation();
}

void FightingState::transitionToFlee()
{
	fighting = false;
	if (notNull(blockComp)) blockComp->unblock();

	checkNullAndBreak(stateMachine);

	((AIStateMachine*)stateMachine)->startFleeingState(target);
}

void FightingState::grab()
{
	checkNullAndBreak(stateMachine);

	ActionInput action = ActionInput::GRAB;
	stateMachine->addActionInput(action);
	lastAction = action;
}

void FightingState::drop()
{
	checkNullAndBreak(stateMachine);

	ActionInput action = ActionInput::DROP;
	stateMachine->addActionInput(action);
	lastAction = action;
}

void FightingState::dodge()
{
	checkNullAndBreak(stateMachine);

	ActionInput action = ActionInput::DODGE;
	stateMachine->addActionInput(action);
	lastAction = action;
}

void FightingState::turnTowardsTarget()
{
	if (!notNull(character->transform) || !notNull(target->transform)) return;

	int x = character->transform->getPosition().x, targetX = target->transform->getPosition().x;
	Vector3 dir = x <= targetX ? dir = Vector3::RIGHT : Vector3::NEGATIVE_RIGHT;

	if (notNull(movement)) movement->move(dir);
}

void FightingState::turnBackOnTarget()
{
	if (!notNull(character->transform) || !notNull(target->transform)) return;

	int x = character->transform->getPosition().x, targetX = target->transform->getPosition().x;
	Vector3 dir = x <= targetX ? Vector3::NEGATIVE_RIGHT : Vector3::RIGHT;

	if (notNull(movement)) movement->move(dir);
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
