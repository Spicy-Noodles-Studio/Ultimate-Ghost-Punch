#pragma once
#include <UserComponent.h>
#include <string>

class Movement;
class Health;
class GhostMovement;
class RigidBody;

class GhostManager : public UserComponent
{
private:
	bool ghost, ghostAble;
	float ghostTime = 10;

	Movement* mov;
	GhostMovement* gMov;
	Health* health;
	RigidBody* rb;

	std::string ghostMeshId, ghostMeshName;
	std::string aliveMeshId, aliveMeshName;

	void changeMesh(std::string id, std::string name);
public:
	GhostManager(GameObject* gameObject);

	virtual void start();
	virtual void update(float deltaTime);

	virtual void handleData(ComponentData* data);

	virtual void OnObjectEnter(GameObject* other);//Para desactivar el fantasma
	virtual void onTriggerEnter(GameObject* other);

	bool isGhost();
	bool hasGhost();

	void activateGhost();
	void deactivateGhost();

};

