#include "Obstacle.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <sstream>

#include "Health.h"
#include "RigidBody.h"
#include "GhostManager.h"

REGISTER_FACTORY(Obstacle);

Obstacle::Obstacle(GameObject* gameObject) : UserComponent(gameObject), damage(0), pushStrength(10.f), respawnOffset(50.0, 0.0, 0.0)
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
		Health* h = other->getComponent<Health>();
		if (h == nullptr) return;
		
		h->receiveDamage(damage);
		// If the player has died, add an offset to the respawn position, in case it resurrects
		if (h->getHealth() <= 0) {
			GhostManager* ghost = other->getComponent<GhostManager>();
			if (ghost != nullptr) {
				respawnOffset.x *= xDir;
				ghost->setDeathPosition(other->transform->getPosition() + respawnOffset);
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