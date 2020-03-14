#pragma once

#include "UserComponent.h"

class HealthComponent : public UserComponent
{
public:
	HealthComponent(GameObject* gameObject);
	virtual ~HealthComponent();

private:
	float health;

	bool alive;
	bool hasGhost;

	void changeHealth(float value);

};