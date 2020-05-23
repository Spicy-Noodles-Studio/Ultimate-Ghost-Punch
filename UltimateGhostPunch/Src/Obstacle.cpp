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
	checkNullAndBreak(data);
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
	if (notNull(other) && other->getTag() == "Player")
	{
		int xDir = 0, yDir = 0;
		if (notNull(other->transform) && notNull(gameObject->transform)) {
			xDir = other->transform->getPosition().x < gameObject->transform->getPosition().x ? -1 : 1; // PLAYER --> OBSTACLE
			yDir = other->transform->getPosition().y < gameObject->transform->getPosition().y ? -1 : 1; // OBSTACLE is over PLAYER
		}
		// The player receives damage
		Health* health = other->getComponent<Health>();
		checkNullAndBreak(health);

		health->receiveDamage(damage);

		Score* score = GameManager::GetInstance()->getScore();
		PlayerIndex* playerIndex = other->getComponent<PlayerIndex>();

		if (notNull(score) && notNull(playerIndex))
			score->damagedBySpike(playerIndex->getIndex());

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
			// The player gets pushed away of the obstacle
			Vector3 pushDir = { double(xDir), double(yDir), 0.0 };
			pushDir.normalize();

			RigidBody* rb = other->getComponent<RigidBody>();
			if (notNull(rb))
				rb->addImpulse(pushDir * pushStrength);
		}
	}
}