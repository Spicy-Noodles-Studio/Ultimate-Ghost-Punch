#pragma once
#ifndef PLAYERFX_H
#define PLAYERFX_H

#include <UserComponent.h>

class MeshRenderer;

class PlayerFX : public UserComponent
{
private:
	enum Effect
	{
		NONE, HURT, INVENCIBLE, SHIELD
	};
	Effect effect;

	MeshRenderer* mesh;

	std::vector<std::string> textureNames;
	std::vector<Vector3> diffuses;

	float time;
	float hurtTime;
	float frecuency;
	float invencibleFrec;

private:
	

public:
	PlayerFX(GameObject* gameObject);
	virtual ~PlayerFX();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

	void activateHurt();
	void deactivateHurt();

	void activateInvencible();
	void deactivateInvencible();

	void activateShield();
	void deactivateShield();
};

#endif