#pragma once
#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <UserComponent.h>

class Obstacle : public UserComponent
{
private:
	// Damage dealt
	int damage;

	// Strength of the impulse
	float pushStrength;

	// Position for respawn used if the player dies
	Vector3 initialPosition;

public:
	Obstacle(GameObject* gameObject);
	virtual ~Obstacle();

	virtual void handleData(ComponentData* data);
	virtual void onCollisionEnter(GameObject* other);
};

#endif