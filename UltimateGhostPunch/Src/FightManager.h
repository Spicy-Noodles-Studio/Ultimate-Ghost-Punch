#pragma once
#ifndef FIGHTMANAGER_H
#define FIGHTMANAGER_H

#include "UserComponent.h"

#include <vector>
#include <string>

class InputSystem;
class UIElement;

class FightManager : public UserComponent
{
private:


private:
	void createLevel();
	void createKnights();

public:
	FightManager(GameObject* gameObject);
	~FightManager();

	virtual void start();
	virtual void update(float deltaTime);

};

#endif