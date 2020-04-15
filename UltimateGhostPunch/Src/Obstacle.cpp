#include "Obstacle.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <sstream>

#include "Health.h"
#include "RigidBody.h"
#include "GameManager.h"
#include "Score.h"
#include "PlayerIndex.h"
#include "GhostManager.h"

REGISTER_FACTORY(Obstacle);

Obstacle::Obstacle(GameObject* gameObject) : UserComponent(gameObject), damage(1), pushStrength(40.0f), respawnOffset(5.0, 0.0, 0.0)
{

}

Obstacle::~Obstacle()
{

}

void Obstacle::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "damage") {
			if (!(ss >> damage))
				LOG("OBSTACLE: Invalid value \"%s\"", prop.second.c_str());
		}
		else if (prop.first == "pushStrength") {
			if (!(ss >> pushStrength))
				LOG("OBSTACLE: Invalid value \"%s\"", prop.second.c_str());
		}
		else if (prop.first == "respawnOffset") {
			double x, y, z;
			if (!(ss >> x >> y >> z))
				LOG("OBSTACLE: Invalid value \"%s\"", prop.second.c_str());
			else
				respawnOffset = { x,y,z };
		}
		else
			LOG("OBSTACLE: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void Obstacle::onCollisionEnter(GameObject* other)
{
	if (other->getTag() == "Player")
	{
		int xDir = other->transform->getPosition().x < gameObject->transform->getPosition().x ? -1 : 1; // PLAYER --> OBSTACLE
		int yDir = other->transform->getPosition().y < gameObject->transform->getPosition().y ? -1 : 1; // OBSTACLE is over PLAYER

		// The player receives damage
		Score* score = GameManager::GetInstance()->getScore();
		Health* health = other->getComponent<Health>();
		if (health == nullptr) return;
		int h = health->getHealth();
		health->receiveDamage(damage);
		if (h != health->getHealth())
			score->damagedBySpike(other->getComponent<PlayerIndex>()->getIndex());
		// If the player has died, add an offset to the respawn position, in case it resurrects
		if (!health->isAlive()) {
			GhostManager* ghostManager = other->getComponent<GhostManager>();
			if (ghostManager != nullptr) {
				respawnOffset.x *= xDir;
				ghostManager->setDeathPosition(other->transform->getPosition() + respawnOffset);
				score->deathByEnviromentHazard(other->getComponent<PlayerIndex>()->getIndex());
			}
		}
		else {
			// The player gets pushed away of the obstacle
			Vector3 pushDir = { double(xDir), double(yDir), 0.0 };
			pushDir.normalize();

			RigidBody* rb = other->getComponent<RigidBody>();
			if (rb != nullptr)
				rb->addImpulse(pushDir * pushStrength);
		}
	}
}