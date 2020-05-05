#include "DeathZone.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <sstream>

#include "PlayerIndex.h"
#include "Respawn.h"
#include "Health.h"
#include "Score.h"
#include "GameManager.h"

REGISTER_FACTORY(DeathZone);

DeathZone::DeathZone(GameObject* gameObject) : UserComponent(gameObject), fallDamage(2)
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
			Score* score = GameManager::GetInstance()->getScore();
			int id = other->getComponent<PlayerIndex>()->getIndex();
			int h = health->getHealth();

			health->receiveDamage(fallDamage);

			if (h != health->getHealth() && score != nullptr)
				score->fall(id);

			if (!health->isAlive() && score != nullptr)
				score->deathByEnviromentHazard(id);

			Respawn* respawn = other->getComponent<Respawn>();

			if (respawn != nullptr)
				respawn->respawn();
		}
	}
}