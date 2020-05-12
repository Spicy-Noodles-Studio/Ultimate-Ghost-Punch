#include "GameManager.h"
#include <ComponentRegister.h>

#include "Timer.h"

REGISTER_FACTORY(GameManager);

GameManager* GameManager::instance = nullptr;

GameManager::GameManager() : UserComponent(nullptr)
{

}

GameManager::GameManager(GameObject* gameObject) : UserComponent(gameObject), level(0), song(0), health(4), time(60), initialTime(time), timeMode(false), paused(false)
{
	if (instance == nullptr)
		instance = this;
	else
		destroy(gameObject);

	playerIndexes = std::vector<int>(4, -1);
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
	playerColours = { {0,0,1}, {0,1,0}, {1,1,0}, {0,0,0} };

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

	initialPlayers = 0;
	for (int i = 0; i < playerIndexes.size(); i++)
	{
		if (playerIndexes[i] != -1)
			initialPlayers++;
	}
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

int GameManager::getInitialPlayers() const
{
	return initialPlayers;
}

std::vector<Vector3>& GameManager::getPlayerColours()
{
	return playerColours;
}

std::vector<GameObject*>& GameManager::getKnights()
{
	return knights;
}

void GameManager::setLevel(int level)
{
	this->level = level;
}

int GameManager::getLevel() const
{
	return level;
}

void GameManager::setLevelName(std::string name)
{
	levelName = name;
}

std::string GameManager::getLevelName() const
{
	return levelName;
}

void GameManager::setSong(int song)
{
	this->song = song;
}

int GameManager::getSong() const
{
	return song;
}

void GameManager::setSongName(std::string name)
{
	songName = name;
}

std::string GameManager::getSongName() const
{
	return songName;
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
	this->initialTime = time;
}

int GameManager::getTime() const
{
	return time;
}

int GameManager::getInitialTime() const
{
	return initialTime;
}

void GameManager::setTimeMode(bool mode)
{
	timeMode = mode;
}

bool GameManager::getTimeMode() const
{
	return timeMode;
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