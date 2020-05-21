#include "Obstacle.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <RigidBody.h>
#include <sstream>

#include "PlayerIndex.h"
#include "Health.h"
#include "Score.h"
#include "Game.h"
#include "GameManager.h"
#include "GhostManager.h"

REGISTER_FACTORY(Obstacle);

Obstacle::Obstacle(GameObject* gameObject) : UserComponent(gameObject), damage(1), pushStrength(40.0f), initialPosition(Vector3::ZERO)
{

}

Obstacle::~Obstacle()
{

}

void Obstacle::handleData(ComponentData* data)
{
	if(data == nullptr)
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "damage")
		{
			setInt(damage);
		}
		else if (prop.first == "pushStrength")
		{
			setFloat(pushStrength);
		}
		else
			LOG("OBSTACLE: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void Obstacle::onCollisionEnter(GameObject* other)
{
	if (other != nullptr && other->getTag() == "Player")
	{
		int xDir = 0, yDir = 0;
		if (other->transform != nullptr && gameObject->transform != nullptr) {
			xDir = other->transform->getPosition().x < gameObject->transform->getPosition().x ? -1 : 1; // PLAYER --> OBSTACLE
			yDir = other->transform->getPosition().y < gameObject->transform->getPosition().y ? -1 : 1; // OBSTACLE is over PLAYER
		}
		// The player receives damage
		Health* health = other->getComponent<Health>();
		if (health == nullptr) return;

		health->receiveDamage(damage);

		Score* score = GameManager::GetInstance()->getScore();
		PlayerIndex* playerIndex = other->getComponent<PlayerIndex>();

		if (score != nullptr && playerIndex != nullptr)
			score->damagedBySpike(playerIndex->getIndex());

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
					score->deathByEnviroment(playerIndex->getIndex());
			}
		}
		else
		{
			// The player gets pushed away of the obstacle
			Vector3 pushDir = { double(xDir), double(yDir), 0.0 };
			pushDir.normalize();

			RigidBody* rb = other->getComponent<RigidBody>();
			if (rb != nullptr)
				rb->addImpulse(pushDir * pushStrength);
		}
	}
}