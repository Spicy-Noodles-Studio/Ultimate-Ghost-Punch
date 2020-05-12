#pragma once
#ifndef SCORE_MANAGER_H
#define SCORE_MANAGER_H

#include "Menu.h"

#include <UIElement.h>
#include <vector>
#include <string>

class ScoreManager : public Menu
{
private:
	std::vector<UIElement> texts;
	std::vector<UIElement> panels;

	// events
	bool resetButtonClick();

	void initStatistics(int numOfPlayers);
	void reposition(int numOfPlayers);

	void setIndexOfPlayersKilled(int playerIndex);

	void setPercentOfHits(int playerIndex);
	void setSuccessfullGrabs(int playerIndex);
	void setTotalAttacks(int playerIndex);
	void setAmountOfDamageDealt(int playerIndex);
	void setLifesAsGhost(int playerIndex);

	void setTimesHittedBySpikes(int playerIndex);
	void setAmountOfFalls(int playerIndex);
	void setTotalDamageSuffer(int playerIndex);
	void setEnviromentDeaths(int playerIndex);

public:
	ScoreManager(GameObject* gameObject);
	virtual ~ScoreManager();

	virtual void start();
};

#endif