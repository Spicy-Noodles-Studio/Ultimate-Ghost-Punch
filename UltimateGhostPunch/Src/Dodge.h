#pragma once
#ifndef DODGE_H
#define DODGE_H

#include <UserComponent.h>

class RigidBody;
class GameObject;
class Movement;

class Dodge : public UserComponent
{
private:
	//VARIABLES A BALANCEAR
	float COOLDOWN = 0.5f;
	float DURATION = 0.15f;
	float VELOCITY = 13.0f;

	//otras variables
	float cd = 0.0f;
	float duration = 0.0f;

	enum State {
		IDLE, DODGING_R, DODGING_L, CD
	};

	State state = IDLE;

	Movement* movement;

public:
	Dodge(GameObject* gameObject);

	virtual void start();
	virtual void update(float deltaTime);

	virtual void handleData(ComponentData* data);


	void dodgeR();
	void dodgeL();

};

#endif