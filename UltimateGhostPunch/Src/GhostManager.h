#pragma once
#ifndef GHOST_MANAGER_H
#define GHOST_MANAGER_H

#include <UserComponent.h>
#include <string>

class Movement;
class GhostMovement;
class Health;
class Transform;
class MeshRenderer;
class RigidBody;
class PlayerUI;

class GhostManager : public UserComponent
{
private:
	bool ghost;
	bool used;

	float ghostTime;
	int ghostDamage;

	Movement* movement;
	GhostMovement* ghostMovement;
	Health* health;
	Transform* transform;
	MeshRenderer* meshRenderer;
	RigidBody* rigidBody;
	PlayerUI* playerUI;

	std::string ghostMeshId, ghostMeshName;
	std::string aliveMeshId, aliveMeshName;

	Vector3 aliveScale;
	Vector3 ghostScale;
	Vector3 ghostSpawnOffset;

	Vector3 deathPosition;

	float playerGravity;

public:
	GhostManager(GameObject* gameObject);
	virtual ~GhostManager();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);
	virtual void onObjectEnter(GameObject* other);

	bool isGhost();

	void activateGhost();
	void deactivateGhost();

	void setDeathPosition(const Vector3& dPos);
};

#endif