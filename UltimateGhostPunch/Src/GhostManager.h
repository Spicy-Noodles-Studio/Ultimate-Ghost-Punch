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
class PlayerAnimController;
class PlayerController;

class GhostManager : public UserComponent
{
private:
	bool ghost;
	bool used;
	bool deathPosChanged;
	bool ended;
	
	enum GhostMode
	{
		ALIVE, GHOST, DYING
	};

	float ghostTime;
	float playerGravity;

	int ghostDamage;
	int resurrectionHealth;

	Movement* movement;
	GhostMovement* ghostMovement;
	Health* health;
	Transform* transform;
	MeshRenderer* meshRenderer;
	RigidBody* rigidBody;
	PlayerUI* playerUI;
	FightManager* fightManager;
	PlayerAnimController* anim;
	PlayerController* control;

	Vector3 aliveScale;
	Vector3 ghostScale;
	Vector3 ghostSpawnOffset;
	Vector3 deathPosition;

	Vector3 playerColour;
	GhostMode mode;

public:
	GhostManager(GameObject* gameObject);
	virtual ~GhostManager();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);
	virtual void onObjectEnter(GameObject* other);

	bool isGhost();
	bool ghostEnded();
	float getGhostTime();

	void activateGhost();
	void deactivateGhost();

	void setDeathPosition(const Vector3& dPos);
	void setPlayerColour(const Vector3& colour);

	void deactivatePlayer();
	void handlePlayerDeath();
};

#endif