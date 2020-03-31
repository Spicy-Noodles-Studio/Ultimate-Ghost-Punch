#pragma once
#ifndef FIGHTCONFIGURATION_H
#define FIGHTCONFIGURATION_H

#include "UserComponent.h"

#include <vector>
#include <string>
#include "UIElement.h"

const int MIN_TIME = 20;
const int MAX_TIME = 240;
const int CHANGE_TIME = 20;

const int MIN_HEALTH = 1;
const int MAX_HEALTH = 10;
const int CHANGE_HEALTH = 1;

const int MIN_PLAYERS = 1;

class InputSystem;

class FightConfiguration : public UserComponent
{
private:
	InputSystem* inputSystem;

	UIElement fightButton;

	int nPlayers;
	std::vector<std::pair<int, UIElement>> slots;

	int health;
	int time;

	std::vector<std::string> levelNames = {"level 1", "dungeons", "cave"};
	std::vector<std::string> songNames = {"despacito", "never gonna", "ya tu sabe"};

	int level;
	int song;

private:
	void checkController();
	void checkKeyboard();

	void fillSlot(int slotIndex, int deviceIndex);
	void clearSlot(int index);

	int isIndexConnected(int index);

	void reorderSlots(int index);

	// events
	bool changeHealth(int value);
	bool changeTime(int value);
	bool changeSong(int value);
	bool changeLevel(int value);

	bool fightButtonClick();

public:
	FightConfiguration(GameObject* gameObject);
	~FightConfiguration();

	virtual void start();
	virtual void update(float deltaTime);

};

#endif