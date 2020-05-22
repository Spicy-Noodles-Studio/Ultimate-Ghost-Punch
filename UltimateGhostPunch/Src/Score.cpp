#include "Score.h"
#include "GameManager.h";

Score::Score()
{
	numPlayers = 0;
}

Score::~Score()
{
	clearScores();
}

void Score::clearScores()
{
	for (auto score : playerScores)
		if (score != nullptr)
			delete score;

	playerScores.clear();
}

void Score::initScorePlayer(ScorePlayer* player)
{
	player->amountOfDamageDealt = 0;
	player->amountOfDamageTakenFromSpikes = 0;
	player->lifesStolenAsGhost = 0;
	player->deathsByEnviroment = 0;
	player->numOfHits = 0;
	player->numOfKills = 0;
	player->numOfSuccessfulGrabs = 0;
	player->numOfTotalAttacks = 0;
	player->timesFallen = 0;
	player->totalDamageTaken = 0;
	player->indexesFromEnemiesKilled = std::vector<int>();
}

bool Score::correctIndex(int index)
{
	return index >= 0 && index < numPlayers;
}

void Score::initScore(int numOfPlayers)
{
	clearScores();
	this->numPlayers = numOfPlayers;

	for (int i = 0; i < numOfPlayers; i++)
	{
		ScorePlayer* score = new ScorePlayer();

		initScorePlayer(score);
		this->playerScores.push_back(score);
	}
}

void Score::attackDone(int playerIndex)
{
	if (!correctIndex(playerIndex -1) || playerScores[playerIndex - 1] == nullptr) return;

	playerScores[playerIndex - 1]->numOfTotalAttacks++;
}

void Score::attackHitted(int playerIndex)
{
	if (!correctIndex(playerIndex -1) || playerScores[playerIndex - 1] == nullptr) return;

	playerScores[playerIndex - 1]->numOfHits++;
}

void Score::damageReceivedFrom(int playerIndex, int fromIndex, int amount)
{
	if (!correctIndex(playerIndex -1) || !correctIndex(fromIndex -1) || playerScores[fromIndex - 1] == nullptr || playerScores[playerIndex - 1] == nullptr) return;

	playerScores[playerIndex - 1]->totalDamageTaken += amount;
	playerScores[fromIndex - 1]->amountOfDamageDealt += amount;

}

void Score::damagedBySpike(int playerIndex)
{
	if (!correctIndex(playerIndex -1) || playerScores[playerIndex - 1] == nullptr) return;

	playerScores[playerIndex - 1]->totalDamageTaken += 2;
	playerScores[playerIndex - 1]->amountOfDamageTakenFromSpikes += 2;
}

void Score::fall(int playerIndex)
{
	if (!correctIndex(playerIndex -1) || playerScores[playerIndex - 1] == nullptr) return;

	playerScores[playerIndex - 1]->timesFallen++;
	playerScores[playerIndex - 1]->totalDamageTaken += 2;
}

void Score::grabHitted(int playerIndex)
{
	if (!correctIndex(playerIndex -1) || playerScores[playerIndex - 1] == nullptr) return;

	playerScores[playerIndex - 1]->numOfSuccessfulGrabs++;
}

void Score::lifeStolenBy(int playerIndex, int fromIndex)
{
	if (!correctIndex(playerIndex -1) || !correctIndex(fromIndex -1) || playerScores[fromIndex - 1] == nullptr || playerScores[playerIndex - 1] == nullptr) return;

	playerScores[playerIndex - 1]->totalDamageTaken += 2;
	playerScores[fromIndex - 1]->lifesStolenAsGhost = 1;
}

void Score::killedBy(int playerIndex, int fromIndex)
{
	if (!correctIndex(fromIndex -1) || playerScores[fromIndex - 1] == nullptr) return;

	playerScores[fromIndex - 1]->numOfKills++;
	playerScores[fromIndex - 1]->indexesFromEnemiesKilled.push_back(playerIndex);
}

void Score::deathByEnviroment(int playerIndex)
{
	if (!correctIndex(playerIndex -1) || playerScores[playerIndex - 1] == nullptr) return;

	playerScores[playerIndex - 1]->deathsByEnviroment++;
}

int Score::getPercentageOfHits(int playerIndex)
{
	
	if(!correctIndex(playerIndex -1) || playerScores[playerIndex-1] == nullptr || playerScores[playerIndex - 1]->numOfTotalAttacks == 0)
		return 0;
	return (double)playerScores[playerIndex - 1]->numOfHits  / (double)playerScores[playerIndex - 1]->numOfTotalAttacks * 100;
}

int Score::getSuccessfulGrabs(int playerIndex)
{
	if (!correctIndex(playerIndex -1) || playerScores[playerIndex - 1] == nullptr) return 0;
	return playerScores[playerIndex - 1]->numOfSuccessfulGrabs;
}

int Score::getNumberOfAttacks(int playerIndex)
{
	if (!correctIndex(playerIndex -1) || playerScores[playerIndex - 1] == nullptr) return 0;
	return playerScores[playerIndex - 1]->numOfTotalAttacks;
}

int Score::getAmountOfDamageDealt(int playerIndex)
{
	if (!correctIndex(playerIndex -1) || playerScores[playerIndex - 1] == nullptr) return 0;
	return playerScores[playerIndex - 1]->amountOfDamageDealt;
}

int Score::getLifesStolen(int playerIndex)
{
	if (!correctIndex(playerIndex -1) || playerScores[playerIndex - 1] == nullptr) return 0;
	return playerScores[playerIndex - 1]->lifesStolenAsGhost;
}

int Score::getTimesHittedBySpikes(int playerIndex)
{
	if (!correctIndex(playerIndex -1) || playerScores[playerIndex - 1] == nullptr) return 0;
	return playerScores[playerIndex - 1]->amountOfDamageTakenFromSpikes / 2;
}

int Score::getAmountOfFalls(int playerIndex)
{
	if (!correctIndex(playerIndex -1) || playerScores[playerIndex - 1] == nullptr) return 0;
	return playerScores[playerIndex - 1]->timesFallen;
}

int Score::getTotalDamageReceived(int playerIndex)
{
	if (!correctIndex(playerIndex -1) || playerScores[playerIndex - 1] == nullptr) return 0;
	return playerScores[playerIndex - 1]->totalDamageTaken;
}

int Score::getNumberOfKills(int playerIndex)
{
	if (!correctIndex(playerIndex -1) || playerScores[playerIndex - 1] == nullptr) return 0;
	return playerScores[playerIndex - 1]->numOfKills;
}

int Score::getEnviromentDeaths(int playerIndex)
{
	if (!correctIndex(playerIndex -1) || playerScores[playerIndex - 1] == nullptr) return 0;
	return playerScores[playerIndex - 1]->deathsByEnviroment;
}

std::vector<int> Score::getIndexOfPlayersKilled(int playerIndex)
{
	if (!correctIndex(playerIndex -1) || playerScores[playerIndex - 1] == nullptr) return std::vector<int>();
	return playerScores[playerIndex - 1]->indexesFromEnemiesKilled;
}