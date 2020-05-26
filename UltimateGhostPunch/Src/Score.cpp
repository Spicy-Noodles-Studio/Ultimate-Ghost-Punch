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
		if (notNull(score))
			delete score;

	playerScores.clear();
}

void Score::setPlayerID(std::vector<int>& playerID)
{
	this->playerID = playerID;
}

std::vector<int>& Score::getPlayerID()
{
	return playerID;
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
	if (!correctIndex(playerIndex) || !notNull(playerScores[playerIndex])) return;

	playerScores[playerIndex]->numOfTotalAttacks++;
}

void Score::attackHitted(int playerIndex)
{
	if (!correctIndex(playerIndex) || !notNull(playerScores[playerIndex])) return;

	playerScores[playerIndex]->numOfHits++;
}

void Score::damageReceivedFrom(int playerIndex, int fromIndex, int amount)
{
	if (!correctIndex(playerIndex) || !correctIndex(fromIndex) || !notNull(playerScores[fromIndex]) || !notNull(playerScores[playerIndex])) return;

	playerScores[playerIndex]->totalDamageTaken += amount;
	playerScores[fromIndex]->amountOfDamageDealt += amount;

}

void Score::damagedBySpike(int playerIndex)
{
	if (!correctIndex(playerIndex) || !notNull(playerScores[playerIndex])) return;

	playerScores[playerIndex]->totalDamageTaken += 2;
	playerScores[playerIndex]->amountOfDamageTakenFromSpikes += 2;
}

void Score::fall(int playerIndex)
{
	if (!correctIndex(playerIndex) || !notNull(playerScores[playerIndex])) return;

	playerScores[playerIndex]->timesFallen++;
	playerScores[playerIndex]->totalDamageTaken += 2;
}

void Score::grabHitted(int playerIndex)
{
	if (!correctIndex(playerIndex) || !notNull(playerScores[playerIndex])) return;

	playerScores[playerIndex]->numOfSuccessfulGrabs++;
}

void Score::lifeStolenBy(int playerIndex, int fromIndex)
{
	if (!correctIndex(playerIndex) || !correctIndex(fromIndex) || !notNull(playerScores[fromIndex]) || !notNull(playerScores[playerIndex])) return;

	playerScores[playerIndex]->totalDamageTaken += 2;
	playerScores[fromIndex]->lifesStolenAsGhost = 1;
}

void Score::killedBy(int playerIndex, int fromIndex)
{
	if (!correctIndex(fromIndex) || !notNull(playerScores[fromIndex])) return;

	playerScores[fromIndex]->numOfKills++;
	playerScores[fromIndex]->indexesFromEnemiesKilled.push_back(playerIndex);
}

void Score::deathByEnviroment(int playerIndex)
{
	if (!correctIndex(playerIndex) || !notNull(playerScores[playerIndex])) return;

	playerScores[playerIndex]->deathsByEnviroment++;
}

int Score::getPercentageOfHits(int playerIndex)
{

	if (!correctIndex(playerIndex) || !notNull(playerScores[playerIndex]) || playerScores[playerIndex]->numOfTotalAttacks == 0)
		return 0;
	return (double)playerScores[playerIndex]->numOfHits / (double)playerScores[playerIndex]->numOfTotalAttacks * 100;
}

int Score::getSuccessfulGrabs(int playerIndex)
{
	if (!correctIndex(playerIndex) || !notNull(playerScores[playerIndex])) return 0;
	return playerScores[playerIndex]->numOfSuccessfulGrabs;
}

int Score::getNumberOfAttacks(int playerIndex)
{
	if (!correctIndex(playerIndex) || !notNull(playerScores[playerIndex])) return 0;
	return playerScores[playerIndex]->numOfTotalAttacks;
}

int Score::getAmountOfDamageDealt(int playerIndex)
{
	if (!correctIndex(playerIndex) || !notNull(playerScores[playerIndex])) return 0;
	return playerScores[playerIndex]->amountOfDamageDealt;
}

int Score::getLifesStolen(int playerIndex)
{
	if (!correctIndex(playerIndex) || !notNull(playerScores[playerIndex])) return 0;
	return playerScores[playerIndex]->lifesStolenAsGhost;
}

int Score::getTimesHittedBySpikes(int playerIndex)
{
	if (!correctIndex(playerIndex) || !notNull(playerScores[playerIndex])) return 0;
	return playerScores[playerIndex]->amountOfDamageTakenFromSpikes / 2;
}

int Score::getAmountOfFalls(int playerIndex)
{
	if (!correctIndex(playerIndex) || !notNull(playerScores[playerIndex])) return 0;
	return playerScores[playerIndex]->timesFallen;
}

int Score::getTotalDamageReceived(int playerIndex)
{
	if (!correctIndex(playerIndex) || !notNull(playerScores[playerIndex])) return 0;
	return playerScores[playerIndex]->totalDamageTaken;
}

int Score::getNumberOfKills(int playerIndex)
{
	if (!correctIndex(playerIndex) || !notNull(playerScores[playerIndex])) return 0;
	return playerScores[playerIndex]->numOfKills;
}

int Score::getEnviromentDeaths(int playerIndex)
{
	if (!correctIndex(playerIndex) || !notNull(playerScores[playerIndex])) return 0;
	return playerScores[playerIndex]->deathsByEnviroment;
}

std::vector<int> Score::getIndexOfPlayersKilled(int playerIndex)
{
	if (!correctIndex(playerIndex) || !notNull(playerScores[playerIndex])) return std::vector<int>();
	return playerScores[playerIndex]->indexesFromEnemiesKilled;
}