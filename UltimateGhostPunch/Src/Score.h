#pragma once
#ifndef SCORE_H
#define SCORE_H

#include <vector>
#include <string>

class Score
{
	class ScorePlayer
	{
	public:
		int numOfTotalAttacks;
		int numOfHits;

		int numOfSuccessfulGrabs;

		int amountOfDamageDealt;
		int lifesStolenAsGhost;

		int amountOfDamageTakenFromSpikes;
		int timesFallen;
		int totalDamageTaken;
		int deathsByEnviroment;

		int numOfKills;
		std::vector<int> indexesFromEnemiesKilled;
	};

private:
	int numPlayers;
	std::vector<ScorePlayer*> playerScores;
	std::vector<int> playerID;

	void initScorePlayer(ScorePlayer* player);
	bool correctIndex(int index);

public:
	Score();
	virtual ~Score();

	void clearScores();

	void setPlayerID(std::vector<int>& playerID);
	std::vector<int>& getPlayerID();

	void initScore(int numOfPlayers);
	void attackDone(int playerIndex);
	void attackHitted(int playerIndex);
	void damageReceivedFrom(int playerIndex, int fromIndex, int amount);
	void damagedBySpike(int playerIndex);
	void fall(int playerIndex);
	void grabHitted(int playerIndex);
	void lifeStolenBy(int playerIndex, int fromIndex);
	void killedBy(int playerIndex, int fromIndex);
	void deathByEnviroment(int playerIndex);

	int getPercentageOfHits(int playerIndex);
	int getSuccessfulGrabs(int playerIndex);
	int getNumberOfAttacks(int playerIndex);
	int getAmountOfDamageDealt(int playerIndex);
	int getLifesStolen(int playerIndex);
	int getTimesHittedBySpikes(int playerIndex);
	int getAmountOfFalls(int playerIndex);
	int getTotalDamageReceived(int playerIndex);
	int getNumberOfKills(int playerIndex);
	int getEnviromentDeaths(int playerIndex);
	std::vector<int> getIndexOfPlayersKilled(int playerIndex);
};

#endif