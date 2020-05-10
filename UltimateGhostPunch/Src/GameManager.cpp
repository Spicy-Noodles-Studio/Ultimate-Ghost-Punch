#include "GameManager.h"
#include <ComponentRegister.h>

#include "Timer.h"

REGISTER_FACTORY(GameManager);

GameManager* GameManager::instance = nullptr;

GameManager::GameManager() : UserComponent(nullptr), paused(false)
{

}

GameManager::GameManager(GameObject* gameObject) : UserComponent(gameObject)
{
	if (instance == nullptr)
		instance = this;
	else
		destroy(gameObject);
}

GameManager::~GameManager()
{
	if (instance == this)
		instance = nullptr;
}

GameManager* GameManager::GetInstance()
{
	return instance;
}

void GameManager::start()
{
	playerColours = { {1,1,1}, {0,0,1}, {0,1,0}, {0,1,1} };

	dontDestroyOnLoad(gameObject);
}

void GameManager::reset()
{
	Timer::GetInstance()->setTimeScale(1.0f);
	paused = false;
}

void GameManager::setPaused(bool setPaused)
{
	if (paused == setPaused) return;

	paused = setPaused;

	if (paused)
		Timer::GetInstance()->setTimeScale(0.0f); //Pause the game
	else
		Timer::GetInstance()->setTimeScale(1.0f); //Resume the game
}

bool GameManager::isPaused() const
{
	return paused;
}

Score* GameManager::getScore()
{
	return &scores;
}

void GameManager::setPlayerIndexes(std::vector<int>& playerIndexes)
{
	this->playerIndexes = playerIndexes;
}

std::vector<int>& GameManager::getPlayerIndexes()
{
	return playerIndexes;
}

void GameManager::initPlayerRanking(int tam)
{
	playerRanking = std::vector<int>(tam, 0);
}

void GameManager::setPlayerRanking(int index, int rank)
{
	if (index > 0 && (index - 1) < playerRanking.size())
		playerRanking[index - 1] = rank;
}

int GameManager::getPlayerRanking(int index) const
{
	if (index > 0 && (index - 1) < playerRanking.size())
		return playerRanking[index - 1];

	return -1;
}

std::vector<Vector3>& GameManager::getPlayerColours()
{
	return playerColours;
}

std::vector<GameObject*>& GameManager::getKnights()
{
	return knights;
}

void GameManager::setLevel(std::string level)
{
	this->level = level;
	this->lastLevel = level;
}

std::string GameManager::getLevel() const
{
	return level;
}

std::string GameManager::getLastLevel() const
{

	return lastLevel;
}

void GameManager::setSong(std::string song)
{
	this->song = song;
	this->lastSong = song;
}

std::string GameManager::getSong() const
{
	return song;
}

std::string GameManager::getLastSong() const
{
	return lastSong;
}

void GameManager::setHealth(int health)
{
	this->health = health;
}

int GameManager::getHealth() const
{
	return health;
}

void GameManager::setTime(int time)
{
	this->time = time;
	this->maxTime = time;
}

int GameManager::getTime() const
{
	return time;
}

int GameManager::getInitialTime() const
{
	return maxTime;
}

void GameManager::setPlayersAlive(int players)
{
	playersAlive = players;
}

int GameManager::getPlayersAlive() const
{
	return playersAlive;
}

void GameManager::setWinner(int winner)
{
	this->winner = winner;
}

int GameManager::getWinner() const
{
	return winner;
}