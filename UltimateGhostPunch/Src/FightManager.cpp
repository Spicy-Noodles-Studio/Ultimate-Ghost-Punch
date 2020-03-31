#include "FightManager.h"

#include "GameManager.h"
#include <GameObject.h>
#include "Health.h"
#include "PlayerController.h"
#include "UILayout.h"
#include "FightConfiguration.h"

FightManager::FightManager(GameObject* gameObject) : UserComponent(gameObject), winnerPanel(NULL), winnerText(NULL)
{

}

FightManager::~FightManager()
{

}

void FightManager::start()
{
	timed = true;

	winnerPanel = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("WinnerBackground");
	winnerText = winnerPanel.getChild("Winner");

	winnerPanel.setVisible(false);

	playerIndexes = GameManager::GetInstance()->getPlayerIndexes();

	// create game
	createLevel();
	createKnights();

	time = GameManager::GetInstance()->getTime();
	if (time < 0) timed = false;
	playSong();
}

void FightManager::update(float deltaTime)
{
	if (timed)
	{
		if (time > 0)
		{
			time -= deltaTime;
			findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("Time").setText(std::to_string((int)time % 60));
		}
		else
		{
			// end game
			chooseWinner();
		}
	}
}

void FightManager::createLevel()
{
	//instantiate(GameManager::GetInstance()->getLevel());
}

void FightManager::playSong()
{
	// SoundSystem::GetInstance()->playMusic(song);
}

void FightManager::createKnights()
{
	int nPlayers = GameManager::GetInstance()->getNPlayers();

	for (int i = 0; i < nPlayers; i++)
	{
		GameObject* knight = instantiate("Player", { 50,10,-200 });
		knight->getComponent<Health>()->setHealth(GameManager::GetInstance()->getHealth());

		if (playerIndexes[i] == 5)
			knight->getComponent<PlayerController>()->setUsingKeyboard(true);
		else
			knight->getComponent<PlayerController>()->setControllerIndex(playerIndexes[i]);

		knight->getComponent<PlayerController>()->setPlayerIndex(i + 1);

		GameManager::GetInstance()->getKnights().push_back(knight);
	}
}

void FightManager::chooseWinner()
{
	std::vector<GameObject*> knights = GameManager::GetInstance()->getKnights();

	bool tie = false;
	int majorHealth = 0;
	int majorIndex = 0;
	for (int i = 0; i < knights.size(); i++)
	{
		Health* h = knights[i]->getComponent<Health>();

		if (h->isAlive())
		{
			if (h->getHealth() > majorHealth)
			{
				majorHealth = h->getHealth();
				majorIndex = i;
			}
			else if (h->getHealth() == majorHealth)
				tie = true;
		}
	}

	winnerPanel.setVisible(true);

	if (tie)
	{
		winner = -1;
		winnerText.setText("TIE");
	}
	else
	{
		winner = majorIndex;
		winnerText.setText("Winner: P" + std::to_string(winner + 1));
	}
}
