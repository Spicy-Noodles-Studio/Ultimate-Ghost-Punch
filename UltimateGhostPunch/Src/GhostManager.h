#pragma once
#ifndef GHOST_MANAGER_H
#define GHOST_MANAGER_H

#include <UserComponent.h>

class Game;
class Transform;
class MeshRenderer;
class RigidBody;
class Movement;
class GhostMovement;
class Health;
class PlayerUI;
class PlayerController;
class PlayerAnimController;
class PlayerController;

class GhostManager : public UserComponent
{
private:
	bool ghost;
	bool used;
	bool ended;
	bool deathPositionChanged;
	bool success;
	bool ghostDead;
	bool punchSuccess;
	
	enum GhostMode
	{
		ALIVE, GHOST, DYING
	};

	float ghostTime;
	float playerGravity;

	int ghostDamage;
	int resurrectionHealth;

	Game* game;
	Transform* transform;
	MeshRenderer* meshRenderer;
	RigidBody* rigidBody;
	Movement* movement;
	GhostMovement* ghostMovement;
	Health* health;
	PlayerUI* playerUI;
	PlayerController* control;
	PlayerAnimController* anim;

	Vector3 playerColour;
	Vector3 aliveScale;
	Vector3 ghostScale;

	Vector3 ghostSpawnOffset;
	Vector3 deathPosition;

	GhostMode mode;

public:
	GhostManager(GameObject* gameObject);
	virtual ~GhostManager();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void postUpdate(float deltaTime);
	virtual void handleData(ComponentData* data);
	virtual void onObjectEnter(GameObject* other);

	bool isGhost() const;
	bool ghostUsed() const;
	bool ghostEnded() const;

	float getGhostTime();
	bool ghostUsed();

	void activateGhost();
	void deactivateGhost();

	void setPlayerColour(const Vector3& colour);
	void setDeathPosition(const Vector3& position);

	void deactivatePlayer();
	void handlePlayerDeath();

	bool ghostSuccess() const;
	bool ghostDeath() const;
	bool hasPunchSuccess() const;
};

#endif