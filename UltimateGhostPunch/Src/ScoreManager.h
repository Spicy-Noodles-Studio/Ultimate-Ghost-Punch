#pragma once
#ifndef SCORE_MANAGER_H
#define SCORE_MANAGER_H

#include <UserComponent.h>
#include <vector>
#include <string>
#include "UIElement.h"

class GameManager;

class ScoreManager :
	public UserComponent
{
	GameManager* manager;
	UIElement player1Text;
	UIElement player2Text;
	UIElement player3Text;
	UIElement player4Text;

	UIElement player1Panel;
	UIElement player2Panel;
	UIElement player3Panel;
	UIElement player4Panel;
	

private:
		// events
	bool resetButtonClick();
	bool backButtonClick();

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