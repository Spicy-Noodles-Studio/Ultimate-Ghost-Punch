#include "FightManager.h"

#include "GameManager.h"
#include <GameObject.h>
#include "Health.h"
#include "PlayerController.h"

FightManager::FightManager(GameObject* gameObject) : UserComponent(gameObject)
{

}

FightManager::~FightManager()
{

}

void FightManager::start()
{
	createKnights();
}

void FightManager::update(float deltaTime)
{

}

void FightManager::createLevel()
{
}

void FightManager::createKnights()
{
	int nPlayers = GameManager::GetInstance()->getNPlayers();

	for (int i = 0; i < nPlayers; i++)
	{
		GameObject* knight = instantiate("Player", { 50,10,-200 });
		knight->getComponent<Health>()->setHealth(/*GameManager::GetInstance()->getHealth()*/8);
		knight->getComponent<PlayerController>()->setPlayerIndex(i + 1);

		GameManager::GetInstance()->getKnights().push_back(knight);
	}
}
