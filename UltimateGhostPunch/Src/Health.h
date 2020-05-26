#pragma once
#ifndef HEALTH_H
#define HEALTH_H

#include <UserComponent.h>

class CameraEffects;

class Health : public UserComponent
{
protected:
	virtual void start();
	virtual void update(float deltaTime);
	virtual void postUpdate(float deltaTime);
	virtual void handleData(ComponentData* data);

public:
	Health(GameObject* gameObject);
	virtual ~Health();


	int getMaxHealth();
	int getHealth();

	void setHealth(int health);
	void receiveDamage(int damage);

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
	float invencibleTime;

	bool alive;
	bool invencible;

	bool hurt; // So we can know if it is being damaged

	CameraEffects* cameraEffects;
};

#endif