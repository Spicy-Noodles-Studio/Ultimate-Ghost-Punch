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

class GhostManager : public UserComponent
{
private:
	bool used;
	bool success;
	bool positionChanged;
	
	enum GhostMode
	{
		RESURRECT, ALIVE, DYING, APPEAR, GHOST, DISAPPEAR, DEAD
	};

	float resurrectTime;
	float dyingTime;
	float appearTime;
	float disappearTime;

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

	Vector3 aliveScale;
	Vector3 ghostScale;

	Vector3 playerColour;
	Vector3 deathPosition;
	Vector3 spawnOffset;

	GhostMode mode;

public:
	GhostManager(GameObject* gameObject);
	virtual ~GhostManager();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);
	virtual void onObjectEnter(GameObject* other);

	bool isGhost() const;

	bool ghostUsed() const;
	bool ghostSuccess() const;
	bool ghostDeath() const;

	float getGhostTime() const;

	void setPlayerColour(const Vector3& colour);
	void setDeathPosition(const Vector3& position);

	void activateGhost();
	void deactivateGhost();
	void deactivatePlayer();

	bool isResurrecting() const;
	bool isDying() const;
	bool isAppearing() const;
	bool isDisappearing() const;

private:
	void handleStates(float deltaTime);
};

#endif