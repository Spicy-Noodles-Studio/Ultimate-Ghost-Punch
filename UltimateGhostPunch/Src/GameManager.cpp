#include "GameManager.h"
#include <ComponentRegister.h>
#include <SoundEmitter.h>
#include <GameObject.h>

#include "Timer.h"
#include "GhostManager.h"

REGISTER_FACTORY(GameManager);

GameManager* GameManager::instance = nullptr;

void GameManager::pauseAllSounds()
{
	for (GameObject* knight : knights) {
		SoundEmitter* emitter = knight->getComponent<SoundEmitter>();
		if (emitter != nullptr) emitter->pauseAll();
	}
}

void GameManager::resumeAllSound()
{
	for (GameObject* knight : knights) {
		SoundEmitter* emitter = knight->getComponent<SoundEmitter>();
		if (emitter != nullptr) emitter->resumeAll();
	}
}

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
	/*level = "";
	song = "";*/
	playerColours = { {1,1,1}, {0,0,1}, {0,1,0}, {0,1,1} };

	dontDestroyOnLoad(gameObject);
}

void GameManager::reset()
{
	Timer::GetInstance()->setTimeScale(1.0f);
	paused = false;
}

void GameManager::pauseGame(bool setPaused)
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

bool GameManager::gameIsPaused()
{
	return paused;
}

void GameManager::setNumPlayers(int nPlayers)
{
	this->numPlayers = nPlayers;
}

int GameManager::getNumPlayers()
{
	return numPlayers;
}

void GameManager::setPlayerIndexes(std::vector<int> playerIndexes)
{
	this->playerIndexes = playerIndexes;
}

std::vector<int>& GameManager::getPlayerIndexes()
{
	return playerIndexes;
}

std::vector<GameObject*>& GameManager::getKnights()
{
	return knights;
}

std::vector<Vector3>& GameManager::getPlayerColours()
{
	return playerColours;
}

void GameManager::emptyKnights()
{
	knights.clear();
}

void GameManager::setLevel(std::string level)
{
	this->level = level;
	this->lastLevel = level;
}

std::string GameManager::getLevel()
{
	return level;
}

std::string GameManager::getLastLevel()
{

	return lastLevel;
}

void GameManager::setSong(std::string song)
{
	this->song = song;
	this->lastSong = song;
}

std::string GameManager::getSong()
{
	return song;
}

std::string GameManager::getLastSong()
{
	return lastSong;
}

void GameManager::setHealth(int health)
{
	this->health = health;
}

int GameManager::getHealth()
{
	return health;
}

void GameManager::setTime(int time)
{
	this->time = time;
	this->maxTime = time;
}

int GameManager::getTime()
{
	return time;
}

int GameManager::getInitialTime()
{
	return maxTime;
}

Score* GameManager::getScore()
{
	return &scores;
}

bool GameManager::isAnyGhost() const
{
	bool anyGhost = false;
	for (GameObject* knight : knights) {
		if (knight != nullptr) {
			GhostManager* ghostManager = knight->getComponent<GhostManager>();
			if (ghostManager != nullptr)
				anyGhost = ghostManager->isGhost() || anyGhost;
		}
	}
	return anyGhost;
}
