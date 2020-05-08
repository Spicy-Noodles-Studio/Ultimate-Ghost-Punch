#pragma once
#include "StateAction.h"

class GameObject;
class Attack;
class Block;

class FightingState :
	public StateAction
{
private:
	GameObject* target;
	GameObject* character;	// Object being controlled
	Attack* attack;
	Block* blockComp;

	// Probability of actions ( % )
	// * IN QUICK ATTACK RANGE(QAR):
	int quickAttackProb_QAR; 
	int strongAttackProb_QAR;
	int blockProb_QAR;

	// * IN STRONG ATTACK RANGE(SAR):
	int strongAttackProb_SAR;
	int seekProb_SAR;
	int blockProb_SAR;

	float blockSpamTimeMAX; // Time until the AI can block again
	float blockSpamTime;
	float unblockTime; // Time until unblock

	bool fighting;

	bool enemyInQuickAttackRange();
	bool enemyInStrongAttackRange();

	void selectAction();
	void quickAttack();
	void strongAttack();
	void block();
	void unblock();
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

