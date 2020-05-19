#pragma once
#ifndef BLOCK_H
#define BLOCK_H

#include <UserComponent.h>

class PlayerFX;

class Block : public UserComponent
{
private:
	GameObject* parent;
	PlayerFX* playerFX;

	float maxBlockTime; //Duration of block when started
	float blockTime; //Remaining time of blocking
	float blockRegenTime; //Time to regen max blockTime
	float blockGrabMargin; //Time margin to block grabs

	float timeElapsed; //Time elapsed for regen

	bool grounded;
	bool blocking;

	int blocked;
	int blockedGrab;

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

	bool canBlock();
	void block();
	void unblock();
	bool blockAttack(Vector3 otherPosition);

	void setMaxBlockTime(float time) { maxBlockTime = time; }
	float getMaxBlockTime() const;
	void setBlockRegenTime(int time) { blockRegenTime = time; }
	void grabBlocked();

	bool wasGrabBlocked() const;
	bool isBlocking() const;
	bool canBlockGrab() const;
	bool hasBlocked() const; // Return if it has just blocked an attack (only true during current frame)
	bool hasBlockedGrab() const; // Return if it has just blocked a grab attack (only true during current frame)
};

#endif