#pragma once
#ifndef PLAYERFX_H
#define PLAYERFX_H

#include <UserComponent.h>

class MeshRenderer;
class Health;
class GhostManager;

class PlayerFX : public UserComponent
{
private:
	enum Effect
	{
		NONE, HURT, INVENCIBLE, SHIELD, GHOST
	};

	Effect effect;

	MeshRenderer* shieldMesh;
	MeshRenderer* mesh;
	Health* health;
	GhostManager* ghost;

	std::vector<Vector3> diffuses;

	float time;
	float hurtTime;

	float frecuency;
	float invencibleFrec;

	float ghostFXTime;
	float ghostFXFrec;

private:
	void updateHurtFX(float deltaTime);
	void updateInvencibleFX(float deltaTime);
	void updateGhostFX(float deltaTime);

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

	void activateGhostFX();
	void deactivateGhostFX();

	void activateShield();
	void deactivateShield();
};

#endif