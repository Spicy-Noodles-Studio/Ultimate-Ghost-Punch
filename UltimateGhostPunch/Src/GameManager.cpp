#include "GameManager.h"
#include <ComponentRegister.h>
#include <SoundEmitter.h>
#include <GameObject.h>

#include "Timer.h"
#include "GhostManager.h"
#include "Health.h"

REGISTER_FACTORY(GameManager);

GameManager* GameManager::instance = nullptr;

GameManager::GameManager() : UserComponent(nullptr)
{

}

GameManager::GameManager(GameObject* gameObject) : UserComponent(gameObject), level(""), song(""), health(4), time(60), initialTime(time), timeMode(false), paused(false)
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

void GameManager::setPaused(bool setPaused)
{
	if (paused == setPaused) return;

	paused = setPaused;

	if (paused) {
		pauseAllSounds();
		Timer::GetInstance()->setTimeScale(0.0f); //Pause the game
	}
	else {
		resumeAllSound();
		Timer::GetInstance()->setTimeScale(1.0f); //Resume the game
	}
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

void GameManager::setInitialPlayers(int players)
{
	initialPlayers = players;
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


std::vector<GameObject*> GameManager::getAlivePlayers()
{
	std::vector<GameObject*> alive;
	for (GameObject* p : knights) {
		if (p->getComponent<Health>()->isAlive() || p->getComponent<GhostManager>()->isGhost()) alive.push_back(p);
	}
	return alive;
}

void GameManager::emptyKnights()
{
	knights.clear();
}

void GameManager::setLevel(std::string level, std::string name)
{
	this->level = level;
	levelName = name;
}

std::pair<std::string, std::string> GameManager::getLevel() const
{
	return std::pair<std::string, std::string>(level, levelName);
}

void GameManager::setSong(std::string song, std::string name)
{
	this->song = song;
	songName = name;
}

std::pair<std::string, std::string> GameManager::getSong() const
{
	return std::pair<std::string, std::string>(song, songName);
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

bool GameManager::isAnyGhost() const
{
	int i = 0;
	bool anyGhost = false;
	while (i < knights.size() && !anyGhost)
	{
		if (knights[i] != nullptr)
		{
			GhostManager* ghostManager = knights[i]->getComponent<GhostManager>();
			anyGhost = ghostManager != nullptr && ghostManager->isGhost();
		}
		i++;
	}

	return anyGhost;
}

GameObject* GameManager::getAnyGhost()
{
	int i = 0;
	GameObject* anyGhost = nullptr;
	while (i < knights.size() && anyGhost == nullptr)
	{
		if (knights[i] != nullptr)
		{
			GhostManager* ghostManager = knights[i]->getComponent<GhostManager>();
			anyGhost = (ghostManager != nullptr && ghostManager->isGhost()) ? knights[i] : nullptr;
		}
		i++;
	}

	return anyGhost;
}

void GameManager::pauseAllSounds()
{
	for (GameObject* knight : knights)
	{
		SoundEmitter* emitter = knight->getComponent<SoundEmitter>();
		if (emitter != nullptr) emitter->pauseAll();
	}
}

void GameManager::resumeAllSound()
{
	for (GameObject* knight : knights)
	{
		SoundEmitter* emitter = knight->getComponent<SoundEmitter>();
		if (emitter != nullptr) emitter->resumeAll();
	}
}