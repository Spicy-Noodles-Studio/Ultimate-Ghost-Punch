#include "Respawn.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <SoundEmitter.h>
#include <sstream>

#include "Movement.h"
#include "PlayerController.h"
#include "PlayerFX.h"
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

void Respawn::respawn()
{
	spawn(initialPos);
}

void Respawn::spawn(const Vector3& spawnPos)
{	
	Movement* movement = gameObject->getComponent<Movement>();
	if (movement != nullptr)
		movement->stop();

	if (playerController != nullptr)
		playerController->setActive(false);

	Health* health = gameObject->getComponent<Health>();
	if (health != nullptr)
	{
		health->setInvencible(true);
		health->setTime(respawnTime);
	}

	gameObject->transform->setPosition(spawnPos);
	time = respawnTime;
	respawning = true;
}

bool Respawn::isRespawning() const
{
	return respawning;
}