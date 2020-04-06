#include "FightManager.h"

#include <ComponentRegister.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <UILayout.h>

#include "PlayerController.h"
#include "Health.h"
#include "FightConfiguration.h"
#include "GameManager.h"


REGISTER_FACTORY(FightManager);

FightManager::FightManager(GameObject* gameObject) :	UserComponent(gameObject), gameManager(nullptr), fightLayout(nullptr), timeText(NULL),
														winnerPanel(NULL), winnerText(NULL), fightTimer(-1.0f), finishTimer(-1.0f), winner(-1)
{

}

FightManager::~FightManager()
{

}

void FightManager::start()
{
	gameManager = GameManager::GetInstance();

	GameObject* mainCamera = findGameObjectWithName("MainCamera");
	if (mainCamera != nullptr)
		fightLayout = mainCamera->getComponent<UILayout>();

	if (fightLayout != nullptr) {
		timeText = fightLayout->getRoot().getChild("Time");
		winnerPanel = fightLayout->getRoot().getChild("WinnerBackground");
		winnerText = winnerPanel.getChild("Winner");
	}
	winnerPanel.setVisible(false);

	playerIndexes = gameManager->getPlayerIndexes();
	playerPositions = { {-20,25,0}, {20,25,0}, {-17.5,5,0}, {17.5,5,0} };

	// create game
	createLevel();
	createKnights();

	fightTimer = gameManager->getTime();
	finishTimer = 4.0f; // Hard Coded
	playSong();
}

void FightManager::update(float deltaTime)
{
	if (fightTimer > 0)	{
		fightTimer -= deltaTime;
		if (fightTimer < 0.0f) fightTimer = 0.0f;
		timeText.setText(std::to_string((int)fightTimer % 60));
	}
	else {
		// end game
		if (winner != -1) chooseWinner();
		finishTimer -= deltaTime;
		if (finishTimer <= 0.0f) SceneManager::GetInstance()->changeScene("mainMenu");
	}
}

void FightManager::playerDie()
{
	int nPlayers = gameManager->getNumPlayers();
	nPlayers--;
	if (nPlayers == 1)
		chooseWinner();
	else
		gameManager->setNumPlayers(nPlayers);
}

void FightManager::createLevel()
{
	//instantiate(GameManager::GetInstance()->getLevel());
}

void FightManager::playSong()
{
	//findGameObjectWithName("MainCamera")->getComponent<SoundEmitter>()->play(GameManager::GetInstance()->getSong());
}

void FightManager::createKnights()
{
	int nPlayers = gameManager->getNumPlayers();

	for (int i = 0; i < nPlayers; i++)
	{
		GameObject* knight = instantiate("Player", playerPositions[i]);
		knight->getComponent<Health>()->setHealth(gameManager->getHealth());
		knight->getComponent<PlayerController>()->setControllerIndex(playerIndexes[i]);
		knight->getComponent<PlayerController>()->setPlayerIndex(i + 1);

		gameManager->getKnights().push_back(knight);
	}
}

void FightManager::chooseWinner()
{
	fightTimer = 0.0f;

	std::vector<GameObject*> knights = gameManager->getKnights();

	bool tie = false;
	int majorHealth = 0;
	int majorIndex = 0;
	for (int i = 0; i < knights.size(); i++)
	{
		Health* health = knights[i]->getComponent<Health>();
		if (health == nullptr) continue;

		if (health->isAlive()) {
			if (health->getHealth() > majorHealth) {
				majorHealth = health->getHealth();
				majorIndex = i;
			}
			else if (health->getHealth() == majorHealth)
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