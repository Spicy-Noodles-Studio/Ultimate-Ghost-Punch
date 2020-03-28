#include "GameManager.h"

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

void GameManager::update()
{
	LOG("je");
}
