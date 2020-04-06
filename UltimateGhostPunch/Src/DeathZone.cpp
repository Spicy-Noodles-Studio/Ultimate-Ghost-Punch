#include "DeathZone.h"

#include<GameObject.h>
#include<sstream>
#include <ComponentRegister.h>

#include "Health.h"
#include "Respawn.h"

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
			health->receiveDamage(fallDamage);

			Respawn* respawn = other->getComponent<Respawn>();
			if (respawn != nullptr); respawn->respawn();
		}
	}
}
