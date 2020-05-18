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
	if (other->getTag() == "Player")
	{
		Health* health = other->getComponent<Health>();
		if (health != nullptr)
		{
			health->receiveDamage(fallDamage);

			Score* score = GameManager::GetInstance()->getScore();
			PlayerIndex* playerIndex = other->getComponent<PlayerIndex>();

			if (score != nullptr && playerIndex != nullptr)
				score->fall(playerIndex->getIndex());

			if (!health->isAlive())
			{
				GameObject* aux = findGameObjectWithName("Game");
				if (aux != nullptr)
				{
					Game* game = aux->getComponent<Game>();
					if (game != nullptr && playerIndex != nullptr)
						initialPosition = game->getPlayerInitialPosition(playerIndex->getIndex());
				}

				GhostManager* ghostManager = other->getComponent<GhostManager>();
				if (ghostManager != nullptr)
				{
					ghostManager->setDeathPosition(initialPosition);
					if (score != nullptr && playerIndex != nullptr)
						score->deathByEnviromentHazard(playerIndex->getIndex());
				}
			}
			else
			{
				Respawn* respawn = other->getComponent<Respawn>();
				if (respawn != nullptr)
					respawn->respawn();
			}
		}
	}
}