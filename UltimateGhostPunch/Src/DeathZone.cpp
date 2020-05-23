#include "DeathZone.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <sstream>

#include "PlayerIndex.h"
#include "Respawn.h"
#include "Health.h"
#include "Score.h"
#include "Game.h"
#include "GameManager.h"
#include "GhostManager.h"

REGISTER_FACTORY(DeathZone);

DeathZone::DeathZone(GameObject* gameObject) : UserComponent(gameObject), fallDamage(2), initialPosition(Vector3::ZERO)
{

}

DeathZone::~DeathZone()
{

}

void DeathZone::handleData(ComponentData* data)
{
	checkNullAndBreak(data);
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "fallDamage")
		{
			setInt(fallDamage);
		}
		else
			LOG("DEATH ZONE: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void DeathZone::onObjectEnter(GameObject* other)
{
	//If a player gets inside it recives damage and respawns
	if (notNull(other) && other->getTag() == "Player")
	{
		Health* health = other->getComponent<Health>();
		if (notNull(health))
		{
			health->receiveDamage(fallDamage);

			Score* score = nullptr;
			if(notNull(GameManager::GetInstance())) score = GameManager::GetInstance()->getScore();
			PlayerIndex* playerIndex = other->getComponent<PlayerIndex>();

			if (notNull(score) && notNull(playerIndex))
				score->fall(playerIndex->getIndex());

			if (!health->isAlive())
			{
				GameObject* aux = findGameObjectWithName("Game");
				if (notNull(aux))
				{
					Game* game = aux->getComponent<Game>();
					if (notNull(game) && notNull(playerIndex))
						initialPosition = game->getPlayerInitialPosition(playerIndex->getIndex());
				}

				GhostManager* ghostManager = other->getComponent<GhostManager>();
				if (notNull(ghostManager))
				{
					ghostManager->setDeathPosition(initialPosition);
					if (notNull(score) && notNull(playerIndex))
						score->deathByEnviroment(playerIndex->getIndex());
				}
			}
			else
			{
				Respawn* respawn = other->getComponent<Respawn>();
				if (notNull(respawn))
					respawn->respawn();
			}
		}
	}
}