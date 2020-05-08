#pragma once
#include "StateAction.h"

class Attack;
class GameObject;

class FightingState :
	public StateAction
{
private:
	GameObject* target;
	GameObject* character;	// Object being controlled
	Attack* attack;

	// Probability of actions ( % )
	// * IN QUICK ATTACK RANGE(QAR):
	int quickAttackProb_QAR; 
	int strongAttackProb_QAR;
	int shieldProb_QAR;

	// * IN STRONG ATTACK RANGE(SAR):
	int strongAttackProb_SAR;
	int seekProb_SAR;
	int shieldProb_SAR;

	bool enemyInQuickAttackRange();
	bool enemyInStrongAttackRange();

	void selectAction();
	void quickAttack();
	void strongAttack();

public:
	FightingState(StateMachine* stateMachine);
	virtual ~FightingState();

	void setTarget(GameObject* target);
	void setCharacter(GameObject* character);
protected:
	virtual void update(float deltaTime);
};

