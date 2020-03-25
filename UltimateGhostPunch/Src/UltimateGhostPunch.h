#pragma once
#ifndef GHOSTPUNCH_H
#define GHOSTPUNCH_H

#include <UserComponent.h>

class RigidBody;
class GameObject;

class UltimateGhostPunch : public UserComponent
{
private:

	bool available = true;
	RigidBody* body;
	float duration = 0;
	float force = 0;
	bool punching = false;


public:
	UltimateGhostPunch(GameObject* gameObject);

	virtual void update(float deltaTime);
	virtual void start();
	void ghostPunch(const Vector3 & dir);

	bool isAvailable();
	bool isPunching();
	virtual void handleData(ComponentData* data);


};

#endif