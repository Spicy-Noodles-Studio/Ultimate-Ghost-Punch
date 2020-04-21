#pragma once
#ifndef RESPAWN_H
#define RESPAWN_H

#include <UserComponent.h>

class Movement;
class PlayerController;

class Respawn : public UserComponent
{
private:
	PlayerController* playerController;
	Vector3 initialPos;

	float respawnTime, time;
	bool respawning;

public:
	Respawn(GameObject* gameObject);
	~Respawn();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

	float getRespawnTime();

	//Spawns in the original position;
	void respawn();
	void spawn(const Vector3& spawnPos);
	bool isRespawning();
};

#endif