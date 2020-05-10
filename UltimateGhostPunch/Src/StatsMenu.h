#pragma once
#ifndef STATS_MENU_H
#define STATS_MENU_H

#include <UserComponent.h>
#include <UIElement.h>
#include <vector>
#include <string>

class GameManager;

class StatsMenu : public UserComponent
{
private:
	GameManager* gameManager;
	std::vector<UIElement> texts;
	std::vector<UIElement> panels;

	void reposition(int numOfPlayers);
	void initStatistics(int numOfPlayers);

	void setPlayersKilled(int playerIndex);

	void setDamageDealt(int playerIndex);
	void setDamageReceived(int playerIndex);
	void setAcuracyPercentage(int playerIndex);

	void setTimesFallen(int playerIndex);

	void setHitsByEnviroment(int playerIndex);
	void setDeathsByEnviroment(int playerIndex);


public:
	StatsMenu(GameObject* gameObject);
	virtual ~StatsMenu();

	virtual void start();
	virtual void update(float deltaTime);
};

#endif