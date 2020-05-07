#pragma once
#ifndef CONFIGURATION_MENU_H
#define CONFIGURATION_MENU_H

#include "Menu.h"

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

class ConfigurationMenu : public Menu
{
private:
	InputSystem* inputSystem;

	std::vector<std::pair<int, UIElement>> slots;
	UILayout* configLayout;
	UIElement fightButton;

	int numPlayers;
	int health;
	int time;

	std::vector<std::string> levelNames = {"level3", "level5", "cave"};
	std::vector<std::string> songNames = {"despacito", "never gonna", "ya tu sabe"};

	std::string fightSound = "fight";

	int levelIndex;
	int songIndex;

private:
	void checkInput();

	void fillSlot(int slotIndex, int deviceIndex);
	void clearSlot(int index);
	void reorderSlots(int index);

	int isIndexConnected(int index);

	bool changeHealth(int value);
	bool changeTime(int value);
	bool changeSong(int value);
	bool changeLevel(int value);

	bool fightButtonClick();

public:
	ConfigurationMenu(GameObject* gameObject);
	virtual ~ConfigurationMenu();

	virtual void start();
	virtual void update(float deltaTime);
};

#endif