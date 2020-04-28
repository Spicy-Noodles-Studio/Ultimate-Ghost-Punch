#pragma once
#ifndef CAMERAEFFECTS_H
#define CAMERAEFFECTS_H

#include <UserComponent.h>
#include <Ogre.h>
#include <OgreTextureUnitState.h>
#include <OgreMaterial.h>
#include <OgreMaterialManager.h>


class Transform;
class InputSystem;


class CameraEffects : public UserComponent
{
private:
	float max, min, current;

	
	enum State { IDLE, FADEIN, FADEOUT, SHAKE };

	State state;
	Transform* cam;
	InputSystem* input;
	Vector3 shakeDir;
	Vector3 initialRotation;
	Vector3 rotationDir;
	float dir;
	float moves;
	float time;
	float vel;
	float minRange;
	float maxRange;
	float duration;


public: 
	CameraEffects(GameObject* gameObject);
	virtual ~CameraEffects();
	virtual void fixedUpdate(float deltaTime);
	virtual void start();
	void fadeOut();
	void fadeIn();
	void shake(Vector3 rotDir);
	void checkInput();
	virtual void handleData(ComponentData* data);
	
};

#endif