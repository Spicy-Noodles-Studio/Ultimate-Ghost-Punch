#pragma once
#include "StateAction.h"
#include "AIStateMachine.h"

class GameObject;
class Attack;
class Block;
class Jump;
class Grab;
class Dodge;
class Movement;
class PlayerState;

class FightingState :
	public StateAction
{
private:
	GameObject* target; // Object attacking
	GameObject* character;	// Object being controlled
	Attack* attack;
	Block* blockComp;
	Jump* jump;
	Grab* grabComp;
	Dodge* dodgeComp;
	Movement* movement;
	PlayerState* pState;

	ActionInput lastAction;

	// Probability of actions ( % )
	// * IN QUICK ATTACK RANGE(QAR):
	int quickAttackProb_QAR; 
	int strongAttackProb_QAR;
	int blockProb_QAR;

	// Max. number of consecutive quick attacks
	int maxQuickAttacks;
	int quickAttackCounter;

	// * IN STRONG ATTACK RANGE(SAR):
	int strongAttackProb_SAR;
	int seekProb_SAR;
	int blockProb_SAR;

	// Grab probability
	int grabProb;
	// Dodge probability
	int dodgeProb;

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
	void transitionToFlee();
	void grab();
	void drop();
	void dodge();
	void turnTowardsTarget();
	void turnBackOnTarget();

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

