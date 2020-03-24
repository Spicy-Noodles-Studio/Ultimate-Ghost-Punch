#pragma once
#ifndef BLOCK_H
#define BLOCK_H

#include <UserComponent.h>
#include <GameObject.h>
#include <RigidBody.h>

class Block : public UserComponent
{
private:
	RigidBody* rigidBody;

	float maxBlockTime; //Duration of block when started
	float blockTime; //Remaining time of blocking
	float blockRegenTime; //Time to regen max blockTime

	float timeElapsed; //Time elapsed for regen

	bool isGrounded;
	bool isBlocking;

	Vector3 blockDirection;

public:
	Block(GameObject* gameObject);

	bool block();
	bool unblock();
	bool blockAttack(float damage, Vector3 otherPosition);
	
	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);
	virtual void onObjectEnter(GameObject* other);
	virtual void onObjectExit(GameObject* other);

	void setMaxBlockTime(float time) { maxBlockTime = time; }
	void setBlockRegenTime(int time) { blockRegenTime = time; }
};

#endif