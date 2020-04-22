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
		int percertOfHits;

		int numOfSuccessfullGrabs;

		int amountOfDamageDealt;
		int livesStolenAsGhost;

		int AmountOfDamageTakenFromSpikes;	
		int timesFallen;
		int TotalDamgeTaken;
		int deathByEnviroment;

		int numOfKills;
		std::vector<int> indexesFromEnemiesKilled;
	};
private:
	int numPlayers;
	std::vector<int> playerIndexes;
	std::vector<ScorePlayer*> playerScores;

	void initScorePlayer(ScorePlayer* player);

public:
	Score();
	~Score();
	void initScore(int numOfPlayers, std::vector<int> playerIndexes);
	void attackDone(int playerIndex,bool groundAttack);
	void receiveHitFrom(int playerIndex, int fromIndex);
	void damageRecivedFrom(int playerIndex, int fromIndex,int amount);
	void damagedBySpike(int playerIndex);
	void fall(int playerIndex);
	void grabbedBy(int playerIndex, int fromIndex);
	void lifeStolenBy(int playerIndex,int fromIndex);
	void killedBy(int playerIndex, int fromIndex);
	void deathByEnviromentHazard(int playerIndex);

	int getPercentOfHits(int playerIndex);
	int getSuccessfullGrabs(int playerIndex);
	int getNumberOfGroundAttacks(int playerIndex);
	int getNumberOfAirAttacks(int playerIndex);
	int getAmountOfDamageDealt(int playerIndex);
	int getLifesAsGhost(int playerIndex);
	int getTimesHittedBySpikes(int playerIndex);
	int getAmountOfFalls(int playerIndex);
	int getTotalDamageSuffer(int playerIndex);
	int getNumberOfKills(int playerIndex);
	int getEnviromentDeaths(int playerIndex);
	std::vector<int> getIndexOfPlayersKilled(int playerIndex);


};


#endif