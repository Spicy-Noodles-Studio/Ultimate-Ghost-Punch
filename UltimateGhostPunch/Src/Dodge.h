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
	float COOLDOWN;
	float force;

	//otras variables
	float cd ;
	

	enum State {
		IDLE, CD
	};

	State state;

	Movement* movement;

	Vector3 vIzq;
	Vector3 vDer;


public:
	Dodge(GameObject* gameObject);

	virtual void start();
	virtual void update(float deltaTime);

	virtual void handleData(ComponentData* data);


	void dodgeR();
	void dodgeL();

};

#endif