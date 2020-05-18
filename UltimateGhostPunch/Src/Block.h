#pragma once
#ifndef BLOCK_H
#define BLOCK_H

#include <UserComponent.h>

class Block : public UserComponent
{
private:
	GameObject* parent;

	float maxBlockTime; //Duration of block when started
	float blockTime; //Remaining time of blocking
	float blockRegenTime; //Time to regen max blockTime
	float blockGrabMargin; //Time margin to block grabs

	float timeElapsed; //Time elapsed for regen

	bool grounded;
	bool blocking;
	bool blocked;

	int blockDirection;

public:
	Block(GameObject* gameObject);
	virtual ~Block();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void postUpdate(float deltaTime);
	virtual void handleData(ComponentData* data);
	virtual void onObjectEnter(GameObject* other);
	virtual void onObjectExit(GameObject* other);

	void block();
	void unblock();
	bool blockAttack(Vector3 otherPosition);

	bool canBlockGrab() const;
	bool isBlocking() const;
	bool hasBlocked() const; // Return if it has just blocked an attack (only true during current frame)
};

#endif