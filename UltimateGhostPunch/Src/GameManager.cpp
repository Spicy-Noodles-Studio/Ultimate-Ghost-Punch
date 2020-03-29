#include "GameManager.h"

GameManager::GameManager() : UserComponent(nullptr)
{
	
}

GameManager::GameManager(GameObject* gameObject) : UserComponent(gameObject)
{
}

GameManager::~GameManager()
{
}

void GameManager::start()
{
	dontDestroyOnLoad(gameObject);
}

void GameManager::update(float deltaTime)
{

}

void GameManager::setLevel(std::string level)
{
	this->level = level;
}

void GameManager::setSong(std::string song)
{
	this->song = song;
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
