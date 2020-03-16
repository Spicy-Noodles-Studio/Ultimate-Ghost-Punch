#pragma once

#include "UserComponent.h"

class Health : public UserComponent
{
public:
	Health(GameObject* gameObject);
	virtual ~Health();

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

	float ghostTime;
	float invencibleTime;

	bool alive; // player alive

	bool ghost; // !knight alive
	bool hasGhost;
	bool invencible;
};