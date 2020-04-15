#include "DeathZone.h"

#include <ComponentRegister.h>
#include<GameObject.h>
#include<sstream>

#include "Respawn.h"
#include "Health.h"
#include "GameManager.h"
#include "Score.h"
#include "PlayerIndex.h"

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
			if (!(ss >> fallDamage))
				LOG("DEATH ZONE: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("DEATH ZONE: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void DeathZone::onObjectEnter(GameObject* other)
{
	//If a player gets inside it recives damage and respawns
	if (other->getTag() == "Player") {
		Health* health = other->getComponent<Health>();
		if (health != nullptr) {
			Score* score = GameManager::GetInstance()->getScore();
			int h = health->getHealth();
			health->receiveDamage(fallDamage);
			if (h != health->getHealth())
				score->fall(other->getComponent<PlayerIndex>()->getIndex());

			if (!health->isAlive())
				score->deathByEnviromentHazard(other->getComponent<PlayerIndex>()->getIndex());

			Respawn* respawn = other->getComponent<Respawn>();
			if (respawn != nullptr); respawn->respawn();
		}
	}
}
