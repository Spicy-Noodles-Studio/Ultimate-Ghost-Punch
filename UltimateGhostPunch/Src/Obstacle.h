#pragma once
#include <UserComponent.h>

class GameObject;

class Obstacle :
	public UserComponent
{
private:
	// Damage dealt to the player that collides with this obstacle
	int damage = 0;
	// Strength of the impulse to the player that collides
	float pushStrength = 10.0f;
	// Offset for respawn used if the player dies from the obstacle's damage
	Vector3 respawnOffset = { 50.0f, 0.0f, 0.0f };

public:
	Obstacle(GameObject* gameObject);

	virtual void onCollisionEnter(GameObject* other);
	virtual void handleData(ComponentData* data);
	virtual void start();
};

