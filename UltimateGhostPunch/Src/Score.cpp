#include "Score.h"

Score::Score()
{
	numPlayers = 0;
}

Score::~Score()
{
	for (auto score:playerScores)
		delete score;

	playerScores.clear();
}

void Score::initScorePlayer(ScorePlayer* player)
{
	player->amountOfDamageDealt = 0;
	player->AmountOfDamageTakenFromSpikes = 0;
	player->livesStolenAsGhost = 0;
	player->deathByEnviroment = 0;
	player->numOfHits = 0;
	player->numOfKills = 0;
	player->numOfSuccessfullGrabs = 0;
	player->numOfTotalAttacks = 0;
	player->percertOfHits = 0;
	player->timesFallen = 0;
	player->TotalDamgeTaken = 0;
}

void Score::initScore(int numOfPlayers)
{
	playerScores.clear();
	this->numPlayers = numOfPlayers;

	for (int i = 0; i < numOfPlayers; i++)
	{
		ScorePlayer* score = new ScorePlayer();
		
		initScorePlayer(score);
		this->playerScores.push_back(score);
	}
}

void Score::attackDone(int playerIndex, bool groundAttack)
{
	playerScores.at(playerIndex - 1)->numOfTotalAttacks++;
}

void Score::receiveHitFrom(int playerIndex, int fromIndex)
{
	playerScores.at(fromIndex - 1)->numOfHits++;
	playerScores.at(fromIndex - 1)->percertOfHits = playerScores.at(fromIndex-1)->numOfHits / playerScores.at(fromIndex-1)->numOfTotalAttacks * 100;
}

void Score::damageRecivedFrom(int playerIndex, int fromIndex, int amount)
{
	playerScores.at(playerIndex - 1)->TotalDamgeTaken+=amount;
	playerScores.at(fromIndex - 1)->amountOfDamageDealt+=amount;

}

void Score::damagedBySpike(int playerIndex)
{
	playerScores.at(playerIndex - 1)->TotalDamgeTaken += 2;
	playerScores.at(playerIndex - 1)->AmountOfDamageTakenFromSpikes += 2;
}

void Score::fall(int playerIndex)
{
	playerScores.at(playerIndex - 1)->timesFallen++;
	playerScores.at(playerIndex - 1)->TotalDamgeTaken += 2;
}

void Score::grabbedBy(int playerIndex, int fromIndex)
{
	playerScores.at(fromIndex - 1)->numOfSuccessfullGrabs++;
}

void Score::lifeStolenBy(int playerIndex, int fromIndex)
{
	playerScores.at(playerIndex - 1)->TotalDamgeTaken+=2;
	playerScores.at(fromIndex - 1)->livesStolenAsGhost = 1;
}

void Score::killedBy(int playerIndex, int fromIndex)
{
	playerScores.at(fromIndex - 1)->numOfKills++;
	playerScores.at(fromIndex - 1)->indexesFromEnemiesKilled.push_back(playerIndex);
}

void Score::deathByEnviromentHazard(int playerIndex)
{
	playerScores.at(playerIndex-1)->deathByEnviroment++;
}

int Score::getPercentOfHits(int playerIndex)
{
	return playerScores.at(playerIndex - 1)->percertOfHits;
}

int Score::getSuccessfullGrabs(int playerIndex)
{
	return playerScores.at(playerIndex - 1)->numOfSuccessfullGrabs;
}

int Score::getNumberOfGroundAttacks(int playerIndex)
{
	return playerScores.at(playerIndex - 1)->numOfTotalAttacks;
}

int Score::getNumberOfAirAttacks(int playerIndex)
{
	return playerScores.at(playerIndex - 1)->numOfTotalAttacks;
}

int Score::getAmountOfDamageDealt(int playerIndex)
{
	return playerScores.at(playerIndex - 1)->amountOfDamageDealt;
}

int Score::getLifesAsGhost(int playerIndex)
{
	return playerScores.at(playerIndex - 1)->livesStolenAsGhost;
}

int Score::getTimesHittedBySpikes(int playerIndex)
{
	return playerScores.at(playerIndex - 1)->AmountOfDamageTakenFromSpikes/2;
}

int Score::getAmountOfFalls(int playerIndex)
{
	return playerScores.at(playerIndex - 1)->timesFallen;
}

int Score::getTotalDamageSuffer(int playerIndex)
{
	return playerScores.at(playerIndex - 1)->TotalDamgeTaken;
}

int Score::getNumberOfKills(int playerIndex)
{
	return playerScores.at(playerIndex - 1)->numOfKills;
}

int Score::getEnviromentDeaths(int playerIndex)
{
	return playerScores.at(playerIndex - 1)->deathByEnviroment;
}

std::vector<int> Score::getIndexOfPlayersKilled(int playerIndex)
{
	return playerScores.at(playerIndex - 1)->indexesFromEnemiesKilled;
}