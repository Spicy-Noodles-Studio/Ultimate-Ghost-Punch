#pragma once
#include "StateAction.h"
class FightingState :
	public StateAction
{
private:
	bool enemyInQuickAttackRange();
	bool enemyInStrongAttackRange();

	void selectAction();
	void quickAttack();
	void strongAttack();

public:
	FightingState(StateMachine* stateMachine);
	virtual ~FightingState();


protected:
	virtual void update(float deltaTime);
};

