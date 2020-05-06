#pragma once
#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <UserComponent.h>

class Obstacle : public UserComponent
{
private:
	// Damage dealt to the player that collides with this obstacle
	int damage;

	// Strength of the impulse for player that collides
	float pushStrength;

	// Offset for respawn used if the player dies from the obstacle's damage
	Vector3 respawnOffset;

public:
	Obstacle(GameObject* gameObject);
	virtual ~Obstacle();

	virtual void handleData(ComponentData* data);
	virtual void onCollisionEnter(GameObject* other);
};

#endif