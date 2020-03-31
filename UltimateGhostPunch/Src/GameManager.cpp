#include "GameManager.h"

#include "ComponentRegister.h"

REGISTER_FACTORY(GameManager);

GameManager* GameManager::instance = nullptr;

GameManager::GameManager() : UserComponent(nullptr)
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
	level = "";
	song = "";

	dontDestroyOnLoad(gameObject);
}

void GameManager::update(float deltaTime)
{

}

void GameManager::setLevel(std::string level)
{
	this->level = level;
}

std::string GameManager::getLevel()
{
	return level;
}

void GameManager::setSong(std::string song)
{
	this->song = song;
}

std::string GameManager::getSong()
{
	return song;
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
}

int GameManager::getTime()
{
	return time;
}

void GameManager::setPlayerIndexes(std::vector<int> playerIndexes)
{
	this->playerIndexes = playerIndexes;
}

std::vector<int>& GameManager::getPlayerIndexes()
{
	return playerIndexes;
}

void GameManager::setNPlayers(int nPlayers)
{
	this->nPlayers = nPlayers;
}

int GameManager::getNPlayers()
{
	return nPlayers;
}

std::vector<GameObject*>& GameManager::getKnights()
{
	return knights;
}
