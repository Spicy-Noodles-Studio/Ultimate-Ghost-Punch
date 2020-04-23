#pragma once
#ifndef FIGHT_CONFIGURATION_H
#define FIGHT_CONFIGURATION_H

#include <UserComponent.h>
#include <UIElement.h>

#include <vector>
#include <string>

const int MIN_TIME = 20;
const int MAX_TIME = 240;
const int CHANGE_TIME = 20;

const int MIN_HEALTH = 1;
const int MAX_HEALTH = 10;
const int CHANGE_HEALTH = 1;

const int MIN_PLAYERS = 1;

class InputSystem;
class UILayout;

class FightConfiguration : public UserComponent
{
private:
	InputSystem* inputSystem;

	std::vector<std::pair<int, UIElement>> slots;
	UILayout* configLayout;
	UIElement fightButton;

	int numPlayers;
	int health;
	int time;

	std::vector<std::string> levelNames = {"level4", "level5", "cave"};
	std::vector<std::string> songNames = {"despacito", "never gonna", "ya tu sabe"};

	int levelIndex;
	int songIndex;

private:
	void checkInput();

	void fillSlot(int slotIndex, int deviceIndex);
	void clearSlot(int index);
	void reorderSlots(int index);

	int isIndexConnected(int index);

	// events
	bool changeHealth(int value);
	bool changeTime(int value);
	bool changeSong(int value);
	bool changeLevel(int value);

	bool fightButtonClick();
	bool backButtonClick();

public:
	FightConfiguration(GameObject* gameObject);
	virtual ~FightConfiguration();

	virtual void start();
	virtual void update(float deltaTime);
};

#endif