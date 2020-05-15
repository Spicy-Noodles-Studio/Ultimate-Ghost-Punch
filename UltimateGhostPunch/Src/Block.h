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

	float blockDirection;

public:
	Block(GameObject* gameObject);
	virtual ~Block();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void postUpdate(float deltaTime);
	virtual void handleData(ComponentData* data);
	virtual void onObjectEnter(GameObject* other);
	virtual void onObjectExit(GameObject* other);

	bool canBlock();
	void block();
	void unblock();
	bool blockAttack(float damage, Vector3 otherPosition);

	void setMaxBlockTime(float time) { maxBlockTime = time; }
	float getMaxBlockTime() const;
	void setBlockRegenTime(int time) { blockRegenTime = time; }

	bool wasGrabBlocked() const;
	bool isBlocking() const;
	bool canBlockGrab() const;
	// Return if it has just blocked an attack (only true during current frame)
	bool hasBlocked() const;
};

#endif