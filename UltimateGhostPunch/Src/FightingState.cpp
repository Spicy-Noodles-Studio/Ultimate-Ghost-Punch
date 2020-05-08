#include "FightingState.h"
#include <time.h>
#include <MathUtils.h>

#include "GameObject.h"

#include "AIStateMachine.h"
#include "Attack.h"
#include "Health.h"
#include "Block.h"

FightingState::FightingState(StateMachine* stateMachine) : StateAction(stateMachine), quickAttackProb_QAR(50), strongAttackProb_QAR(45), blockProb_QAR(5), strongAttackProb_SAR(60), seekProb_SAR(38), blockProb_SAR(2), blockSpamTimeMAX(10)
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
	//attack->objInQuickAttackSensor(target);
}


bool FightingState::enemyInQuickAttackRange()
{
	return attack->objInQuickAttackSensor(target);
}

bool FightingState::enemyInStrongAttackRange()
{
	return attack->objInStrongAttackSensor(target);
}

void FightingState::selectAction()
{
	if (blockComp != nullptr && blockComp->isBlocking())
	{
		if (unblockTime <= 0)
			unblock();
		
		return;
	}

	if (attack != nullptr && attack->isAttacking()) // Wait until attack ends
	{
		//LOG("ATACANDO...\n");
		return;
	}

	Health* targetHealth = target->getComponent<Health>();
	if (targetHealth != nullptr && !targetHealth->isAlive()) // Change target if target is dead
	{
		transitionToPlatformNav();
		return;
	}

	int rnd = rand() % 100;

	// QUICK ATTACK RANGE
	if (enemyInQuickAttackRange())
	{
		if (rnd < quickAttackProb_QAR)								// Action: Quick Attack
		{
			//LOG("QUICK ATTACK...\n");
			quickAttack();
		}
		else if (rnd < quickAttackProb_QAR + strongAttackProb_QAR)	// Action: Strong Attack
		{
			//LOG("STRONG ATTACK...\n");
			strongAttack();
		}
		else														// Action: Try to shield
		{
			//LOG("USING SHIELD...\n");
			if(blockSpamTime <= 0)
				block();
		}
		return;
	}

	// STRONG ATTACK RANGE
	if (enemyInStrongAttackRange())
	{
		if (rnd < strongAttackProb_SAR)					// Action: Strong Attack
		{
			//LOG("STRONG ATTACK (SAR)...\n");
			strongAttack();
		}
		else if (rnd < strongAttackProb_SAR + seekProb_SAR)					// Action: Transition to seek
		{
			//LOG("GETTING CLOSER...\n");
			transitionToPlatformNav();
		}
		else											// Action: Try to shield
		{
			//LOG("USING SHIELD (SAR)...\n");
			block();
		}
		return;
	}

	// NOT IN RANGE OF ATTACK
	//LOG("GETTING CLOSER...\n");
	transitionToPlatformNav();


}

void FightingState::quickAttack()
{
	ActionInput action = ActionInput::QUICK_ATTACK;
	stateMachine->addActionInput(action);
}

void FightingState::strongAttack()
{
	ActionInput action = ActionInput::STRONG_ATTACK;
	stateMachine->addActionInput(action);
}

void FightingState::block()
{
	ActionInput action = ActionInput::BLOCK;
	stateMachine->addActionInput(action);
	
	float minUnblockTime = blockComp->getMaxBlockTime() / 4;
	unblockTime = minUnblockTime + (float) (rand()) / ((float) (RAND_MAX / (blockComp->getMaxBlockTime() - minUnblockTime)));
	blockSpamTime = blockSpamTimeMAX;
}

void FightingState::unblock()
{
	ActionInput action = ActionInput::UNBLOCK;
	stateMachine->addActionInput(action);
}

void FightingState::transitionToPlatformNav()
{
	fighting = false;
	((AIStateMachine*)stateMachine)->changeTarget();
	((AIStateMachine*)stateMachine)->startPlatformNavigation();
}


void FightingState::setTarget(GameObject* target)
{
	this->target = target;
}

void FightingState::setCharacter(GameObject* character)
{
	this->character = character;

	auto aux = character->findChildrenWithTag("attackSensor");
	if (aux.size() > 0)
		attack = aux[0]->getComponent<Attack>();
	
	std::vector<GameObject*> ground = character->findChildrenWithTag("groundSensor");
	if (ground.size() > 0)
	{
		//jump = aux[0]->getComponent<Jump>();
		blockComp = ground[0]->getComponent<Block>();
	}
}

void FightingState::setFighting(bool fighting)
{
	this->fighting = fighting;
}

bool FightingState::isFighting() const
{
	return fighting;
}
