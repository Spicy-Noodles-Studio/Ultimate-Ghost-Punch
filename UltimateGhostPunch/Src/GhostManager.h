#pragma once
#ifndef GHOST_MANAGER_H
#define GHOST_MANAGER_H

#include <UserComponent.h>

class Movement;
class GhostMovement;
class Health;
class Transform;
class MeshRenderer;
class RigidBody;
class PlayerUI;
class FightManager;

class GhostManager : public UserComponent
{
private:
	bool ghost, used, deathPosChanged;

	float ghostTime, playerGravity;
	int ghostDamage, resurrectionHealth;

	Movement* movement;
	GhostMovement* ghostMovement;
	Health* health;
	Transform* transform;
	MeshRenderer* meshRenderer;
	RigidBody* rigidBody;
	PlayerUI* playerUI;
	FightManager* fightManager;

	Vector3 aliveScale;
	Vector3 ghostScale;
	Vector3 ghostSpawnOffset;

	Vector3 deathPosition;

public:
	GhostManager(GameObject* gameObject);
	virtual ~GhostManager();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);
	virtual void onObjectEnter(GameObject* other);

	bool isGhost();
	float getGhostTime();

	void activateGhost();
	void deactivateGhost();

	void setDeathPosition(const Vector3& dPos);
};

#endif