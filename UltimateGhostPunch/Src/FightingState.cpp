#include "FightingState.h"
#include <time.h>
#include <MathUtils.h>

#include "GameObject.h"

#include "AIStateMachine.h"
#include "Attack.h"

FightingState::FightingState(StateMachine* stateMachine) : StateAction(stateMachine), quickAttackProb_QAR(50), strongAttackProb_QAR(30), shieldProb_QAR(20), strongAttackProb_SAR(60), seekProb_SAR(20), shieldProb_SAR(20)
{
}

FightingState::~FightingState()
{
}

void FightingState::update(float deltaTime)
{
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
	if (attack->isAttacking()) // Wait until attack ends
	{
		LOG("ATACANDO...\n");
		return;
	}

	int rnd = rand() % 100;

	// QUICK ATTACK RANGE
	if (enemyInQuickAttackRange())
	{
		if (rnd < quickAttackProb_QAR)								// Action: Quick Attack
		{
			LOG("QUICK ATTACK...\n");
			quickAttack();
		}
		else if (rnd < quickAttackProb_QAR + strongAttackProb_QAR)	// Action: Strong Attack
		{
			LOG("STRONG ATTACK...\n");
			strongAttack();
		}
		else														// Action: Try to shield
		{
			LOG("USING SHIELD...\n");
			//shield();
		}
		return;
	}

	// STRONG ATTACK RANGE
	if (enemyInStrongAttackRange())
	{
		if (rnd < strongAttackProb_SAR)					// Action: Strong Attack
		{
			LOG("STRONG ATTACK (SAR)...\n");
			strongAttack();
		}
		else if (rnd < seekProb_SAR)					// Action: Transition to seek
		{
			LOG("GETTING CLOSER...\n");
			((AIStateMachine*)stateMachine)->startPlatformNavigation();
		}
		else											// Action: Try to shield
		{
			LOG("USING SHIELD (SAR)...\n");
			//shield();
		}
		return;
	}

	// NOT IN RANGE OF ATTACK
	LOG("GETTING CLOSER...\n");
	((AIStateMachine*)stateMachine)->startPlatformNavigation();

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
	
}