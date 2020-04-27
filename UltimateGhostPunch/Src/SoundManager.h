#pragma once
#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <UserComponent.h>
#include <UIElement.h>
#include <Vector3.h>

#include <vector>
#include <string>


class SoundManager : public UserComponent
{

public:
	SoundManager(GameObject* gameObject);
	virtual ~SoundManager();

	virtual void start();
	virtual void update(float deltaTime);


};

#endif