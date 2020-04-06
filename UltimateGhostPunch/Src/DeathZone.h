#pragma once

#ifndef DEATH_ZONE_H
#define DEATH_ZONE_H

#include <UserComponent.h>

class DeathZone : public UserComponent
{
private:
	int fallDamage;

public:
	DeathZone(GameObject* gameObject);
	~DeathZone();

	virtual void handleData(ComponentData* data);

	virtual void onObjectEnter(GameObject* other);
};

#endif 
