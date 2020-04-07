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
	float COOLDOWN = 1.0f;
	float force = 3000.0f;

	//otras variables
	float cd = 0.0f;
	

	enum State {
		IDLE, CD
	};

	State state = IDLE;

	Movement* movement;

	Vector3 vIzq = { -1,0,0 };
	Vector3 vDer = { 1,0,0 };


public:
	Dodge(GameObject* gameObject);

	virtual void start();
	virtual void update(float deltaTime);

	virtual void handleData(ComponentData* data);


	void dodgeR();
	void dodgeL();

};

#endif