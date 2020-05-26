#pragma once
#ifndef CONFIGURATION_MENU_H
#define CONFIGURATION_MENU_H

#include "Menu.h"

#include <UIElement.h>

#include <vector>
#include <string>

const int MIN_TIME = 60;
const int MAX_TIME = 300;
const int CHANGE_TIME = 10;

const int MIN_HEALTH = 1;
const int MAX_HEALTH = 5;
const int CHANGE_HEALTH = 1;

#ifdef RECORD_PATH
const int MIN_PLAYERS = 1;
#else
const int MIN_PLAYERS = 2;
#endif

class InputSystem;
class UILayout;

class ConfigurationMenu : public Menu
{
private:
	UILayout* configurationLayout;

	UIElement settingsPanel;
	UIElement startButton;

	std::vector<std::pair<int, UIElement>> slots;

	int nPlayers;
	int health;

	int time;
	bool mode;

	std::vector<std::string> timeModes;

	float previewTime;
	float timer;

	bool songPreview;

	std::map<std::string, std::string> levelNames;
	std::map<std::string, std::string> songNames;

	std::string currentLevel;
	std::string currentSong;

private:
	void initNames();
	void checkInput();

	void fillSlot(int index);
	void clearSlot(int index);

	int isIndexConnected(int index);

	bool changeHealth(int value);

	bool changeTimeMode(int value);
	bool changeTime(int value);

	bool changeLevel(int value);
	bool changeSong(int value);
	bool previewSong(bool value);

	void stopPreview();

	bool slotButtonClick(int index, std::string name);

	bool startButtonClick();
	bool settingsButtonClick();
	virtual bool backButtonClick();

protected:
	virtual void start();
	virtual void update(float deltaTime);

public:
	ConfigurationMenu(GameObject* gameObject);
	virtual ~ConfigurationMenu();
};

#endif