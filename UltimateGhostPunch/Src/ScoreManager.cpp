#include "ComponentRegister.h"
#include "ScoreManager.h"

REGISTER_FACTORY(ScoreManager);

bool ScoreManager::backButtonClick()
{
	return false;
}

bool ScoreManager::resetButtonClick()
{
	return false;
}

ScoreManager::ScoreManager(GameObject* gameObject) : UserComponent(gameObject), player1Text(NULL), player2Text(NULL), player3Text(NULL), player4Text(NULL)
{
}

ScoreManager::~ScoreManager()
{
}

void ScoreManager::start()
{
}
