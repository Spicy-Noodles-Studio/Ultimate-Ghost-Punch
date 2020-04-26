#pragma once
#ifndef CAMERAEFFECTS_H
#define CAMERAEFFECTS_H

#include <UserComponent.h>
#include <Ogre.h>
#include <OgreTextureUnitState.h>
#include <OgreMaterial.h>
#include <OgreMaterialManager.h>


class CameraEffects : public UserComponent
{
private:
	float alpha;

	
	Ogre::ResourcePtr resptr;
	Ogre::Material* mat;


	enum State { IDLE, FADEIN, FADEOUT, SHAKE };

	State state;

public: 
	CameraEffects(GameObject* gameObject);
	virtual ~CameraEffects();
	virtual void fixedUpdate(float deltaTime);
	
};

#endif