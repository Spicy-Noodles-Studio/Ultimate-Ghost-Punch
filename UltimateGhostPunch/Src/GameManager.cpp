#include "GameManager.h"

#include <ComponentRegister.h>
#include <SoundEmitter.h>
#include <GameObject.h>
#include <WindowManager.h>
#include <SoundSystem.h>

#include "PlayerState.h"
#include "Timer.h"
#include "GhostManager.h"
#include "Health.h"

REGISTER_FACTORY(GameManager);

GameManager* GameManager::instance = nullptr;

GameManager::GameManager() : UserComponent(nullptr)
{

}

GameManager::GameManager(GameObject* gameObject) : UserComponent(gameObject), level(""), song(""), health(6), time(60), initialTime(time), timeMode(false), paused(false), initialBrightness(0.5f), initialMusicVolume(0.5f), initialSoundVolume(0.5f)
{
	if (instance == nullptr) {
		instance = this;
		if (notNull(WindowManager::GetInstance()))
			WindowManager::GetInstance()->setBrightness(initialBrightness);
		if (notNull(SoundSystem::GetInstance())) {
			SoundSystem::GetInstance()->setMusicVolume(initialMusicVolume);
			SoundSystem::GetInstance()->setSoundEffectsVolume(initialSoundVolume);
		}
	}
	else
		destroy(gameObject);

	playerIndexes = std::vector<int>(4, -1);
	playerRanking = std::vector<int>(4, 0);
	ranking = std::priority_queue<ii, std::vector<ii>, Less>();
}

GameManager::~GameManager()
{
	if (instance == this)
		instance = nullptr;

	knights.clear();
}

GameManager* GameManager::GetInstance()
{
	return instance;
}

void GameManager::start()
{
	playerColours = { {1,0,0}, {0,0,1}, {1,1,0}, {0,1,0} };

	dontDestroyOnLoad(gameObject);
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

std::priority_queue<ii, std::vector<ii>, Less>& GameManager::getRanking()
{
	return ranking;
}

void GameManager::emptyRanking()
{
	while (!ranking.empty())
		ranking.pop();
}

std::vector<Vector3>& GameManager::getPlayerColours()
{
	return playerColours;
}

std::vector<GameObject*>& GameManager::getKnights()
{
	return knights;
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

void GameManager::setInitialPlayers(int players)
{
	initialPlayers = players;
}

int GameManager::getInitialPlayers() const
{
	return initialPlayers;
}

void GameManager::setWinner(int winner)
{
	this->winner = winner;
}

int GameManager::getWinner() const
{
	return winner;
}

void GameManager::setPaused(bool setPaused)
{
	if (paused == setPaused) return;

	paused = setPaused;

	Timer* timeManager = Timer::GetInstance();
	checkNullAndBreak(timeManager);

	if (paused) {
		pauseAllSounds();
		timeManager->setTimeScale(0.0f); //Pause the game
	}
	else {
		resumeAllSound();
		timeManager->setTimeScale(1.0f); //Resume the game
	}
}

bool GameManager::isPaused() const
{
	return paused;
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

float GameManager::getInitialBrightness() const
{
	return initialBrightness;
}

float GameManager::getInitialSoundVolume() const
{
	return initialSoundVolume;
}

float GameManager::getInitialMusicVolume() const
{
	return initialMusicVolume;
}

GameObject* GameManager::getAnyGhost()
{
	int i = 0;
	GameObject* anyGhost = nullptr;
	while (i < knights.size() && anyGhost == nullptr)
	{
		if (notNull(knights[i]))
		{
			GhostManager* ghostManager = knights[i]->getComponent<GhostManager>();
			anyGhost = (notNull(ghostManager) && ghostManager->isGhost()) ? knights[i] : nullptr;
		}
		i++;
	}
	return anyGhost;
}

std::vector<GameObject*> GameManager::getAlivePlayers(bool getGhosts)
{
	std::vector<GameObject*> alive;
	for (GameObject* player : knights)
	{
		if (notNull(player))
		{
			PlayerState* state = player->getComponent<PlayerState>();
			if (getGhosts)
			{
				if (notNull(state) && !state->isDead())
					alive.push_back(player);
			}
			else
			{
				if (notNull(state) && !state->isDead() && !state->isGhost())
					alive.push_back(player);
			}
		}
	}
	return alive;
}

void GameManager::pauseAllSounds()
{
	for (GameObject* knight : knights)
	{
		if (notNull(knight)) {
			SoundEmitter* emitter = knight->getComponent<SoundEmitter>();
			if (notNull(emitter)) emitter->pauseAll();
		}
	}
}

void GameManager::resumeAllSound()
{
	for (GameObject* knight : knights)
	{
		if (notNull(knight)) {
			SoundEmitter* emitter = knight->getComponent<SoundEmitter>();
			if (notNull(emitter)) emitter->resumeAll();
		}
	}
}

bool GameManager::playerUsingKeyboard() const
{
	for (int a : playerIndexes) {
		if (a == 4)
			return true;
	}
	return false;
}