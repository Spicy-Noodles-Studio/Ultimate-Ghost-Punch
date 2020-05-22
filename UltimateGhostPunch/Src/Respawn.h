#pragma once
#ifndef RESPAWN_H
#define RESPAWN_H

#include <UserComponent.h>

class Movement;
class PlayerState;

class Respawn : public UserComponent
{
private:
	PlayerState* playerState;
	Vector3 initialPos;

	float respawnTime;
	float time;

	bool respawning;

protected:
	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);

public:
	Respawn(GameObject* gameObject);
	~Respawn();
	void respawn(); //Spawns in the original position
	void spawn(const Vector3& spawnPos);

	bool isRespawning() const;
};

#endif