#include "FightingState.h"
#include <time.h>
#include <MathUtils.h>
#include "AIStateMachine.h"

FightingState::FightingState(StateMachine* stateMachine) : StateAction(stateMachine)
{
}

FightingState::~FightingState()
{
}


void FightingState::update(float deltaTime)
{
	selectAction();
}


bool FightingState::enemyInQuickAttackRange()
{
	// FALTA COMPROBAR SI HAY ALGUIEN EN EL RANGO
	return true;
}

bool FightingState::enemyInStrongAttackRange()
{
	// FALTA COMPROBAR SI HAY ALGUIEN EN EL RANGO
	return true;
}

void FightingState::selectAction()
{
	srand(time(NULL));


	int i = rand() % 100;

	if (i < 30 && enemyInQuickAttackRange()) quickAttack();
	else if (i < 60 && enemyInStrongAttackRange()) strongAttack();
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



