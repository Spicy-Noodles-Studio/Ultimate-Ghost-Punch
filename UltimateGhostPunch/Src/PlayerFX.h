#pragma once
#ifndef PLAYERFX_H
#define PLAYERFX_H

#include <UserComponent.h>

class PlayerFX : public UserComponent
{
private:
	float cooldown;

	Vector3 diffuse;

public:
	PlayerFX(GameObject* gameObject);
	virtual ~PlayerFX();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);
};

#endif