#include "Respawn.h"

#include <ComponentRegister.h>
#include <GameObject.h>
#include <sstream>

#include "PlayerController.h"
#include "PlayerAnimController.h"
#include "Movement.h"
#include "Health.h"

#include "PlayerFX.h"

REGISTER_FACTORY(Respawn);

Respawn::Respawn(GameObject* gameObject) : UserComponent(gameObject), initialPos(Vector3()), respawnTime(1.0f), respawning(false), time(0.0f), playerController(nullptr)
{
}

Respawn::~Respawn()
{
}

void Respawn::start()
{
	playerController = gameObject->getComponent<PlayerController>();

	initialPos = gameObject->transform->getPosition();
	time = 0.0f;
}

void Respawn::update(float deltaTime)
{
	if (time > 0)
		time -= deltaTime;
	else if (playerController != nullptr && respawning) {
		playerController->setActive(true);
		respawning = false;
	}
}

void Respawn::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "respawnTime")
		{
			if (!(ss >> respawnTime))
				LOG("RESPAWN: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("RESPAWN: Invalid property name \"%s\"", prop.first.c_str());
	}
}

float Respawn::getRespawnTime()
{
	return respawnTime;
}

void Respawn::respawn()
{
	spawn(initialPos);
}

void Respawn::spawn(const Vector3& spawnPos)
{
	Movement* movement = gameObject->getComponent<Movement>();
	Health* health = gameObject->getComponent<Health>();
	PlayerAnimController* anim = gameObject->getComponent<PlayerAnimController>();

	if (movement != nullptr) movement->stop();
	if (health != nullptr) {
		health->setInvencible(true);
		health->setTime(respawnTime);
	}
	if (playerController != nullptr) playerController->setActive(false);

	gameObject->getComponent<PlayerFX>()->activateInvencible();

	gameObject->transform->setPosition(spawnPos);
	time = respawnTime;
	respawning = true;

	if (anim != nullptr) anim->resurrectAnimation();
}

bool Respawn::isRespawning()
{
	return respawning;
}
