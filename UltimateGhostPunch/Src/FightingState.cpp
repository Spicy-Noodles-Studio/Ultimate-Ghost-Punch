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

FightingState::FightingState(StateMachine* stateMachine) : StateAction(stateMachine), quickAttackProb_QAR(50), strongAttackProb_QAR(45), blockProb_QAR(5), strongAttackProb_SAR(60), seekProb_SAR(38), blockProb_SAR(2), blockSpamTimeMAX(10), grabProb(2), maxQuickAttacks(3), quickAttackCounter(0), lastAction(ActionInput::STOP), dodgeProb(5)
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
	return attack->isQuickAttackOnRange(target);
}

bool FightingState::enemyInStrongAttackRange()
{
	return attack->isStrongAttackOnRange(target);
}

void FightingState::selectAction()
{
	if (!character->getComponent<Health>()->isAlive())
		return;

	if (character->getComponent<Health>()->isInvencible())
		return;



	if (lastAction == ActionInput::DODGE && !dodgeComp->isDodging()) turnTowardsTarget(); // Turn towards target after dodge

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
	GhostManager* targetGhostManager = target->getComponent<GhostManager>();

	if (targetHealth != nullptr && targetGhostManager != nullptr && (!targetHealth->isAlive() && !targetGhostManager->ghostUsed()) || targetGhostManager->isGhost()) // Flee if target is entering ghost mode
	{
		//LOG("FLEEING FROM TARGET...\n");
		transitionToFlee();
		return;
	}
	
	if (targetHealth != nullptr && !targetHealth->isAlive()) // Change target if target is dead
	{
		//LOG("QUICK ATTACK...\n");
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
	if (lastAction != action) quickAttackCounter = 0;

	if (!attack->attackOnCD())
	{
		quickAttackCounter++;
	}
	// Max. consecutive quick attacks reached?
	if (quickAttackCounter < 3)
	{
		stateMachine->addActionInput(action);
		lastAction = action;
	}
	else if(dodgeComp != nullptr && !dodgeComp->isOnCooldown() && pState != nullptr && pState->canDodge())
	{
		turnBackOnTarget();
		dodge();
	}

}

void FightingState::strongAttack()
{
	if (!attack->attackOnCD())
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
		else if(rnd  < dodgeProb + grabProb && dodgeComp != nullptr && !dodgeComp->isOnCooldown() && pState != nullptr && pState->canDodge() && lastAction != ActionInput::QUICK_ATTACK) // Don't dodge after a quick attack and check cooldown
		{
			turnBackOnTarget();
			dodge();
		}
	}
		
}

void FightingState::block()
{
	if (blockComp->canBlock())
	{
		ActionInput action = ActionInput::BLOCK;
		stateMachine->addActionInput(action);

		float minUnblockTime = blockComp->getMaxBlockTime() / 4;
		unblockTime = minUnblockTime + (float)(rand()) / ((float)(RAND_MAX / (blockComp->getMaxBlockTime() - minUnblockTime)));
		blockSpamTime = blockSpamTimeMAX;
		lastAction = action;
	}
	else
	{
		LOG("CANNOT BLOCK -> FLEEING INSTEAD\n");
		transitionToFlee();
	}
	
}

void FightingState::unblock()
{
	ActionInput action = ActionInput::UNBLOCK;
	stateMachine->addActionInput(action);
	lastAction = action;
}

void FightingState::transitionToPlatformNav()
{
	blockComp->unblock();
	fighting = false;
	((AIStateMachine*)stateMachine)->changeTarget();
	((AIStateMachine*)stateMachine)->startPlatformNavigation();
}

void FightingState::transitionToFlee()
{
	blockComp->unblock();
	fighting = false;
	((AIStateMachine*)stateMachine)->startFleeingState(target);
}

void FightingState::grab()
{
	LOG("TRIED TO GRAB!!!\n");
	ActionInput action = ActionInput::GRAB;
	stateMachine->addActionInput(action);
	lastAction = action;
}

void FightingState::drop()
{
	ActionInput action = ActionInput::DROP;
	stateMachine->addActionInput(action);
	lastAction = action;
}

void FightingState::dodge()
{
	ActionInput action = ActionInput::DODGE;
	stateMachine->addActionInput(action);
	lastAction = action;
	//LOG("DODGING!\n");
}

void FightingState::turnTowardsTarget()
{
	Vector3 dir;
	int x = character->transform->getPosition().x, targetX = target->transform->getPosition().x;
	if (x <= targetX) // (AI) --- (TARGET) ---
		dir = Vector3::RIGHT;
	else              //      --- (TARGET) --- (AI)
		dir = Vector3::NEGATIVE_RIGHT;

	if (movement != nullptr)
		movement->move(dir);
}

void FightingState::turnBackOnTarget()
{
	Vector3 dir;
	int x = character->transform->getPosition().x, targetX = target->transform->getPosition().x;
	if (x <= targetX) // (AI) --- (TARGET) ---
		dir = Vector3::NEGATIVE_RIGHT;
	else              //      --- (TARGET) --- (AI)
		dir = Vector3::RIGHT;

	if (movement != nullptr)
		movement->move(dir);
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
		jump = aux[0]->getComponent<Jump>();
		blockComp = ground[0]->getComponent<Block>();
	}

	std::vector<GameObject*> grabSensor = character->findChildrenWithTag("grabSensor");
	if (grabSensor.size() > 0)
	{
		grabComp = grabSensor[0]->getComponent<Grab>();
	}

	dodgeComp = character->getComponent<Dodge>();
	movement = character->getComponent<Movement>();
	pState = character->getComponent<PlayerState>();
	if (pState == nullptr) LOG("Error: AI has no PlayerState\n");

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
