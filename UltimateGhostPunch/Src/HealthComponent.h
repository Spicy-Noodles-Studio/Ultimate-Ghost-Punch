#pragma once

#include "UserComponent.h"

class HealthComponent : public UserComponent
{
public:
	HealthComponent(GameObject* gameObject);
	virtual ~HealthComponent();

	int getHealth();
	void setHealth(int health);

	bool isAlive();
	bool isGhost();
	bool isInvencible();

private:
	void recieveDamage(int damage);
	void die();
	void resurrect();

	void activateGhost();

private:
	int health; // 1 life = 10 health points
	int resurrectionHealth;

	int invencibleTime;

	bool alive; // player alive

	bool ghost; // !knight alive
	bool hasGhost;
	bool invencible;
};