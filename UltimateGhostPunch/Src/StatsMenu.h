#pragma once
#ifndef STATS_MENU_H
#define STATS_MENU_H

#include "Menu.h"

#include <UIElement.h>
#include <vector>
#include <string>

class GameManager;
class Score;

class StatsMenu : public Menu
{
private:
	Score* score;

	std::vector<UIElement> texts;
	std::vector<UIElement> panels;

	void reposition(int numOfPlayers);
	void initStatistics(int numOfPlayers);

	void setPlayersKilled(int playerIndex);

	void setDamageDealt(int playerIndex);
	void setDamageReceived(int playerIndex);
	void setAccuracy(int playerIndex);

	void setTimesFallen(int playerIndex);

	void setHitsByEnviroment(int playerIndex);
	void setDeathsByEnviroment(int playerIndex);

	bool checkControllersInput();

public:
	StatsMenu(GameObject* gameObject);
	virtual ~StatsMenu();

	virtual void start();
	virtual void update(float deltaTime);
};

#endif