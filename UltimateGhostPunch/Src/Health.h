#pragma once
#ifndef HEALTH_H
#define HEALTH_H

#include <UserComponent.h>

class Health : public UserComponent
{
public:
	Health(GameObject* gameObject);
	virtual ~Health();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void postUpdate(float deltaTime);
	virtual void handleData(ComponentData* data);

	int getMaxHealth();
	int getHealth();

	void setHealth(int health);
	void receiveDamage(int damage);

	float getTime();
	float getInvDamTime();

	void setTime(float time);

	bool isAlive();
	void setAlive(bool alive);

	bool isInvencible();
	void setInvencible(bool invencible);

	bool isHurt() const;

private:
	int maxHealth;
	int health; // 1 life = 2 health points

	float time;
	float invencibleDamageTime;

	bool alive;
	bool invencible;

	bool hurt; // So we can know if it is being damaged
};

#endif