#pragma once
#ifndef DEATH_ZONE_H
#define DEATH_ZONE_H

#include <UserComponent.h>

class DeathZone : public UserComponent
{
private:
	int fallDamage;

	Vector3 initialPosition;

protected:
	virtual void handleData(ComponentData* data);
	virtual void onObjectEnter(GameObject* other);

public:
	DeathZone(GameObject* gameObject);
	virtual ~DeathZone();
};

#endif