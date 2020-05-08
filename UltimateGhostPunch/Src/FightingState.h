#pragma once
#include "StateAction.h"

class GameObject;
class Attack;
class Shield;

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
	int blockProb_QAR;

	// * IN STRONG ATTACK RANGE(SAR):
	int strongAttackProb_SAR;
	int seekProb_SAR;
	int blockProb_SAR;

	bool fighting;

	bool enemyInQuickAttackRange();
	bool enemyInStrongAttackRange();

	void selectAction();
	void quickAttack();
	void strongAttack();
	void block();
	void transitionToPlatformNav();

public:
	FightingState(StateMachine* stateMachine);
	virtual ~FightingState();

	void setTarget(GameObject* target);
	void setCharacter(GameObject* character);

	void setFighting(bool fighting);
	bool isFighting() const;
protected:
	virtual void update(float deltaTime);
};

