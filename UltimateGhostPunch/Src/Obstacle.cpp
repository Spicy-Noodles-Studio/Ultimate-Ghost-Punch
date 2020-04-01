#include "Obstacle.h"
#include "GameObject.h"
#include <sstream>

#include "Health.h"
#include "RigidBody.h"
#include "GhostManager.h"
#include "ComponentRegister.h"

REGISTER_FACTORY(Obstacle);


Obstacle::Obstacle(GameObject* gameObject) : UserComponent(gameObject)
{
}

void Obstacle::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		if (prop.first == "damage") {
			if (!(ss >> damage))
				LOG("OBSTACLE: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "pushStrength") {
			if (!(ss >> pushStrength))
				LOG("OBSTACLE: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "respawnOffset") {
			double x, y, z;
			if (!(ss >> x >> y >> z))
				LOG("OBSTACLE: Invalid property with name \"%s\"", prop.first.c_str());
			else
				respawnOffset = { x,y,z };
		}
		else
			LOG("OBSTACLE: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void Obstacle::start()
{
	printf("ME HE CREADO\n");
}


void Obstacle::onCollisionEnter(GameObject* other)
{
	if (other->getTag() == "Player")
	{
		int xDir;
		if (other->transform->getPosition().x < gameObject->transform->getPosition().x) // PLAYER --> OBSTACLE
			xDir = -1;
		else // OBSTACLE <-- PLAYER
			xDir = 1;

		int yDir;
		if (other->transform->getPosition().y < gameObject->transform->getPosition().y) // OBSTACLE is over PLAYER
			yDir = -1;
		else // PLAYER is over OBSTACLE
			yDir = 1;

		// The player receives damage
		Health* h = other->getComponent<Health>();
		if (h == nullptr) return;
		
		h->receiveDamage(damage);
		// If the player has died, add an offset to the respawn position, in case it resurrects
		if (h->getHealth() <= 0)
		{
			GhostManager* ghost = other->getComponent<GhostManager>();
			if (ghost != nullptr)
			{
				respawnOffset.x *= xDir;
				ghost->setDeathPosition(other->transform->getPosition() + respawnOffset);
			}
		}
		else
		{
			// The player gets pushed away of the obstacle
			Vector3 pushDir;
			
			pushDir = { double(xDir), double(yDir), 0.0f };
			pushDir.normalize();

			RigidBody* rb = other->getComponent<RigidBody>();
			if (rb != nullptr)
				rb->addImpulse(pushDir * pushStrength);
		}

		

		
	}
}


