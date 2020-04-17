#include "FightManager.h"
#include "Score.h"
#include <ComponentRegister.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <UILayout.h>
#include <GaiaData.h>
#include <RigidBody.h>

#include "PlayerController.h"
#include "PlayerIndex.h"
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
	playerPositions = { {-20,20,0}, {20,20,0}, {-17.5,0,0}, {17.5,0,0} };

	// create game
	createLevel();
	createSpikes();
	createKnights();
	gameManager->getScore()->initScore(gameManager->getNumPlayers(), gameManager->getPlayerIndexes());

	fightTimer = gameManager->getTime();
	finishTimer = 4.0f; // Hard Coded
	gameManager->pauseGame(false);
	playSong();
}

void FightManager::update(float deltaTime)
{
	if (fightTimer > 0)	{
		fightTimer -= deltaTime;
		if (fightTimer < 0.0f) fightTimer = 0.0f;
		timeText.setText(std::to_string((int)fightTimer % 60));
	}
	else if(fightTimer == 0) {//If its negative it means match its not timed
		// end game
		if (winner == -1) chooseWinner();
		finishTimer -= deltaTime;
		if (finishTimer <= 0.0f) { 
			gameManager->getKnights().clear();
			SceneManager::GetInstance()->changeScene("leaderBoard");
		}
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
	GaiaData levelData;
	levelData.load("./Assets/Levels/" + GameManager::GetInstance()->getLevel() + ".level");

	// instantiate collider mesh
	instantiate(levelData.find("LevelBlueprint").getValue().c_str());

	// instantiate render mesh
	instantiate(levelData.find("LevelRenderBlueprint").getValue().c_str());

	/*GameObject* backWall = instantiate("Cubo", { 0,0,-10 });
	backWall->getComponent<Transform>()->setScale({ 190,150,1 });
	backWall->getComponent<RigidBody>()->setKinematic(true);*/

	// player initial transforms
	GaiaData playerData = levelData.find("PlayerTransforms");
	for (int i = 0; i < playerData.size(); i++)
	{
		std::stringstream ss(playerData[i][0].getValue());
		double posX, posY, posZ;
		if (!(ss >> posX >> posY >> posZ)) {
			LOG_ERROR("FIGHT MANAGER", "invalid player position \"%s\"", playerData[i][0].getValue().c_str());
			continue;
		}

		ss = std::stringstream(playerData[i][1].getValue());
		double rotX, rotY, rotZ;
		if (!(ss >> rotX >> rotY >> rotZ)) {
			LOG_ERROR("FIGHT MANAGER", "invalid player rotation \"%s\"", playerData[i][1].getValue().c_str());
			continue;
		}
		playerTransforms.push_back({ { posX, posY, posZ }, { rotX, rotY, rotZ } });
	}

	// spikes transforms
	GaiaData spikesData = levelData.find("SpikesTransforms");
	nSpikes = spikesData.size();
	for (int i = 0; i < nSpikes; i++)
	{
		std::stringstream ss(spikesData[i][0].getValue());
		double posX, posY, posZ;
		if (!(ss >> posX >> posY >> posZ)) {
			LOG_ERROR("FIGHT MANAGER", "invalid spikes position \"%s\"", spikesData[i][0].getValue().c_str());
			continue;
		}

		ss = std::stringstream(spikesData[i][1].getValue());
		double rotX, rotY, rotZ;
		if (!(ss >> rotX >> rotY >> rotZ)) {
			LOG_ERROR("FIGHT MANAGER", "invalid spikes rotation \"%s\"", spikesData[i][1].getValue().c_str());
			continue;
		}
		spikesTransforms.push_back({ { posX, posY, posZ }, { rotX, rotY, rotZ } });
	}
}

void FightManager::playSong()
{
	//findGameObjectWithName("MainCamera")->getComponent<SoundEmitter>()->play(GameManager::GetInstance()->getSong());
}

void FightManager::createKnights()
{
	int nPlayers = gameManager->getNumPlayers();

	gameManager->getKnights().clear();

	for (int i = 0; i < nPlayers; i++)
	{
		GameObject* knight = instantiate("Player", playerTransforms[i].first);
		knight->transform->setRotation(playerTransforms[i].second);

		knight->getComponent<Health>()->setHealth(gameManager->getHealth());

		knight->getComponent<PlayerController>()->setControllerIndex(playerIndexes[i]);
		knight->getComponent<PlayerIndex>()->setIndex(i + 1);

		gameManager->getKnights().push_back(knight);
	}
}

void FightManager::createSpikes()
{
	for (int i = 0; i < nSpikes; i++)
	{
		GameObject* spikes = instantiate("Spikes", spikesTransforms[i].first);
		spikes->transform->setRotation(spikesTransforms[i].second);
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
