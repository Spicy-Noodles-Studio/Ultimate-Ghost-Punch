#pragma once
#ifndef CONFIGURATION_MENU_H
#define CONFIGURATION_MENU_H

#include <UserComponent.h>
#include <UIElement.h>

#include <vector>
#include <string>

const int MIN_TIME = 60;
const int MAX_TIME = 300;
const int CHANGE_TIME = 10;

const int MIN_HEALTH = 2;
const int MAX_HEALTH = 10;
const int CHANGE_HEALTH = 1;

const int MIN_PLAYERS = 1;

class InputSystem;
class UILayout;

class ConfigurationMenu : public UserComponent
{
private:
	InputSystem* inputSystem;
	UILayout* configurationLayout;

	UIElement settingsPanel;
	UIElement startButton;

	std::vector<std::pair<int, UIElement>> slots;

	int nPlayers;
	int health;

	int time;
	bool mode;

	std::vector<std::string> timeModes;
	std::vector<std::string> levelNames;
	std::vector<std::string> songNames;

	int levelIndex;
	int songIndex;

private:
	void checkInput();

	void fillSlot(int slotIndex, int deviceIndex);
	void clearSlot(int index);
	void reorderSlots(int index);

	int isIndexConnected(int index);

	bool changeHealth(int value);

	bool changeTimeMode(int value);
	bool changeTime(int value);

	bool changeLevel(int value);
	bool changeSong(int value);

	bool startButtonClick();
	bool settingsButtonClick();
	bool backButtonClick();

public:
	ConfigurationMenu(GameObject* gameObject);
	virtual ~ConfigurationMenu();

	virtual void start();
	virtual void update(float deltaTime);
};

#endif