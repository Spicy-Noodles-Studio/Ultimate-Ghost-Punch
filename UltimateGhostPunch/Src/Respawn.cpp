#include "Respawn.h"

#include <ComponentRegister.h>
#include <GameObject.h>
#include <sstream>

#include "Movement.h"
#include "PlayerState.h"
#include "Health.h"

REGISTER_FACTORY(Respawn);

Respawn::Respawn(GameObject* gameObject) : UserComponent(gameObject), playerState(nullptr), initialPos(Vector3::ZERO), respawning(false), respawnTime(1.0f), time(0.0f)
{

}

Respawn::~Respawn()
{
	playerState = nullptr;
}

void Respawn::start()
{
	time = 0.0f;

	checkNullAndBreak(gameObject);
	playerState = gameObject->getComponent<PlayerState>();
	checkNull(playerState);

	checkNullAndBreak(gameObject->transform);
	initialPos = gameObject->transform->getPosition();
}

void Respawn::update(float deltaTime)
{
	if (time > 0)
		time -= deltaTime;
	else if (respawning)
	{
		if (notNull(playerState)) playerState->setIgnoringInput(false);
		respawning = false;
	}

	checkNullAndBreak(gameObject);
	if (notNull(playerState) && !playerState->isGhost() && notNull(gameObject->transform) && gameObject->transform->getPosition().y < -20)
		respawn();
}

void Respawn::handleData(ComponentData* data)
{
	checkNullAndBreak(data);
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
	checkNullAndBreak(gameObject);

	Movement* movement = gameObject->getComponent<Movement>();
	if (notNull(movement))
		movement->stop();

	if (notNull(playerState))
		playerState->setIgnoringInput(true);

	Health* health = gameObject->getComponent<Health>();
	if (notNull(health))
	{
		health->setInvencible(true);
		health->setTime(respawnTime);
	}

	if (notNull(gameObject->transform))
		gameObject->transform->setPosition(spawnPos);

	time = respawnTime;
	respawning = true;
}

bool Respawn::isRespawning() const
{
	return respawning;
}