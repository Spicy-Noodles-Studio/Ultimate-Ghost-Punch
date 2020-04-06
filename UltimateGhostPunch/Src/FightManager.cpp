#include "FightManager.h"

#include "GameManager.h"
#include <GameObject.h>
#include "Health.h"
#include "PlayerController.h"
#include "UILayout.h"
#include "FightConfiguration.h"
#include <GaiaData.h>

#include "RigidBody.h"

#include "ComponentRegister.h"

REGISTER_FACTORY(FightManager);

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
	createSpikes();
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

void FightManager::playerDie()
{
	int nPlayers = GameManager::GetInstance()->getNPlayers();
	nPlayers--;
	if (nPlayers == 1)
		chooseWinner();
	else
		GameManager::GetInstance()->setNPlayers(nPlayers);
}

void FightManager::createLevel()
{
	GaiaData levelData;
	levelData.load("./Assets/Levels/" + GameManager::GetInstance()->getLevel() + ".level");

	// instantiate collider mesh
	instantiate(levelData.find("LevelBlueprint").getValue().c_str());

	// instantiate render mesh
	//...

	GameObject* backWall = instantiate("Cubo", { 0,0,-10 });
	backWall->getComponent<Transform>()->setScale({ 90,50,1 });
	backWall->getComponent<RigidBody>()->setKinematic(true);

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
	int nPlayers = GameManager::GetInstance()->getNPlayers();

	for (int i = 0; i < nPlayers; i++)
	{
		GameObject* knight = instantiate("Player", playerTransforms[i].first);
		knight->transform->setRotation(playerTransforms[i].second);

		knight->getComponent<Health>()->setHealth(GameManager::GetInstance()->getHealth());

		if (playerIndexes[i] == 5) knight->getComponent<PlayerController>()->setUsingKeyboard(true);
		else knight->getComponent<PlayerController>()->setControllerIndex(playerIndexes[i]);
		knight->getComponent<PlayerController>()->setPlayerIndex(i + 1);

		GameManager::GetInstance()->getKnights().push_back(knight);
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
