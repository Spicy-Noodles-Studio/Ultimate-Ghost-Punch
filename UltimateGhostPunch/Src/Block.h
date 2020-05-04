#pragma once
#ifndef BLOCK_H
#define BLOCK_H

#include <UserComponent.h>

class Attack;

class Block : public UserComponent
{
private:
	float maxBlockTime; //Duration of block when started
	float blockTime; //Remaining time of blocking
	float blockRegenTime; //Time to regen max blockTime
	float blockGrabMargin; //Time margin to block grabs

	float timeElapsed; //Time elapsed for regen

	bool isGrounded;
	bool isBlocking;

	float blockDirection;

	Attack* attack; // To check if player is attacking

public:
	Block(GameObject* gameObject);
	virtual ~Block();

	virtual void start();
	virtual void update(float deltaTime);
	virtual void handleData(ComponentData* data);
	virtual void onObjectEnter(GameObject* other);
	virtual void onObjectExit(GameObject* other);

	void block();
	void unblock();
	bool blockAttack(float damage, Vector3 otherPosition);

	void setMaxBlockTime(float time) { maxBlockTime = time; }
	void setBlockRegenTime(int time) { blockRegenTime = time; }

	bool getGrabBlock() const;
	bool blocking() const;
};

#endif