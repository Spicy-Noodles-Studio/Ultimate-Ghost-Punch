#include "Respawn.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <SoundEmitter.h>
#include <sstream>

#include "PlayerController.h"
#include "PlayerAnimController.h"
#include "PlayerFX.h"
#include "Movement.h"
#include "Health.h"

REGISTER_FACTORY(Respawn);

Respawn::Respawn(GameObject* gameObject) : UserComponent(gameObject), playerController(nullptr), initialPos(Vector3::ZERO), respawning(false), respawnTime(1.0f), time(0.0f)
{

}

Respawn::~Respawn()
{

}

void Respawn::start()
{
	playerController = gameObject->getComponent<PlayerController>();
	soundEmitter = gameObject->getComponent<SoundEmitter>();

	initialPos = gameObject->transform->getPosition();
	time = 0.0f;
}

void Respawn::update(float deltaTime)
{
	if (time > 0)
		time -= deltaTime;
	else if (playerController != nullptr && respawning)
	{
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
			setFloat(respawnTime);
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

	if (movement != nullptr)
		movement->stop();

	if (health != nullptr)
	{
		health->setInvencible(true);
		health->setTime(respawnTime);
	}

	if (playerController != nullptr)
		playerController->setActive(false);

	gameObject->getComponent<PlayerFX>()->activateInvencible();

	gameObject->transform->setPosition(spawnPos);
	time = respawnTime;
	respawning = true;

	if (anim != nullptr)
		anim->resurrectAnimation();

	if (soundEmitter != nullptr) soundEmitter->playSound("respawn");
}

bool Respawn::isRespawning()
{
	return respawning;
}