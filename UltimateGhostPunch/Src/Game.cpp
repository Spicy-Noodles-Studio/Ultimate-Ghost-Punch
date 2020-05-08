#include "Game.h"
#include <ComponentRegister.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <UILayout.h>
#include <MeshRenderer.h>
#include <RigidBody.h>
#include <Light.h>
#include <Strider.h>
#include <GaiaData.h>

#include "PlayerController.h"
#include "PlayerIndex.h"
#include "Health.h"
#include "GhostManager.h"
#include "Score.h"
#include "ConfigurationMenu.h"
#include "GameManager.h"

REGISTER_FACTORY(Game);

Game::Game(GameObject* gameObject) : UserComponent(gameObject), gameManager(nullptr), fightLayout(nullptr), timeText(NULL), winnerPanel(NULL), winnerText(NULL),
fightTimer(-1.0f), finishTimer(-1.0f), winner(-1), nLights(0), nSpikes(0)
{

}

Game::~Game()
{

}

void Game::start()
{
	gameManager = GameManager::GetInstance();

	GameObject* mainCamera = findGameObjectWithName("MainCamera");
	if (mainCamera != nullptr)
		fightLayout = mainCamera->getComponent<UILayout>();

	if (fightLayout != nullptr)
	{
		timeText = fightLayout->getRoot().getChild("Time");
		winnerPanel = fightLayout->getRoot().getChild("WinnerBackground");
		winnerText = winnerPanel.getChild("Winner");
	}
	winnerPanel.setVisible(false);

	playerIndexes = gameManager->getPlayerIndexes();
	playerColours = gameManager->getPlayerColours();

	// create game
	createLevel();
	createSpikes();
	createKnights();
	createLights();
	gameManager->getScore()->initScore(gameManager->getNumPlayers(), gameManager->getPlayerIndexes());

	fightTimer = gameManager->getTime();
	finishTimer = 4.0f; // Hard Coded

	gameManager->pauseGame(false);
	playSong();
}

void Game::update(float deltaTime)
{
	if (fightTimer > 0)
	{
		fightTimer -= deltaTime;
		if (fightTimer < 0.0f)
			fightTimer = 0.0f;

		timeText.setText(std::to_string((int)fightTimer % 60));
	}
	else if (fightTimer == 0)
	{
		// If its negative it means match its not timed
		// End game
		if (winner == -1)
			chooseWinner();

		finishTimer -= deltaTime;
		if (finishTimer <= 0.0f)
		{
			gameManager->getKnights().clear();
			SceneManager::GetInstance()->changeScene("StatsMenu");
		}
	}
}

void Game::playerDie()
{
	int nPlayers = gameManager->getNumPlayers();
	nPlayers--;

	if (nPlayers == 1)
		chooseWinner();
	else
		gameManager->setNumPlayers(nPlayers);
}

void Game::createLevel()
{
	GaiaData levelData;
	levelData.load("./Assets/Levels/" + GameManager::GetInstance()->getLevel() + ".level");

	std::string renderName = levelData.find("RenderMesh").getValue();
	std::string colliderName = levelData.find("ColliderMesh").getValue();

	// Configuramos el mesh visual
	configureLevelRender(renderName);

	// Configuramos el mesh de colision
	configureLevelCollider(colliderName);

	// read player initial transforms
	GaiaData playerData = levelData.find("PlayerTransforms");
	for (int i = 0; i < playerData.size(); i++)
	{
		std::stringstream ss(playerData[i][0].getValue());
		double posX, posY, posZ;
		if (!(ss >> posX >> posY >> posZ))
		{
			LOG_ERROR("GAME", "invalid player position \"%s\"", playerData[i][0].getValue().c_str());
			continue;
		}

		ss = std::stringstream(playerData[i][1].getValue());
		double rotX, rotY, rotZ;
		if (!(ss >> rotX >> rotY >> rotZ))
		{
			LOG_ERROR("GAME", "invalid player rotation \"%s\"", playerData[i][1].getValue().c_str());
			continue;
		}
		playerTransforms.push_back({ { posX, posY, posZ }, { rotX, rotY, rotZ } });
	}

	// read spikes transforms
	GaiaData spikesData = levelData.find("SpikesTransforms");
	nSpikes = spikesData.size();
	for (int i = 0; i < nSpikes; i++)
	{
		std::stringstream ss(spikesData[i][0].getValue());
		double posX, posY, posZ;
		if (!(ss >> posX >> posY >> posZ))
		{
			LOG_ERROR("GAME", "invalid spikes position \"%s\"", spikesData[i][0].getValue().c_str());
			continue;
		}

		ss = std::stringstream(spikesData[i][1].getValue());
		double rotX, rotY, rotZ;
		if (!(ss >> rotX >> rotY >> rotZ))
		{
			LOG_ERROR("GAME", "invalid spikes rotation \"%s\"", spikesData[i][1].getValue().c_str());
			continue;
		}
		spikesTransforms.push_back({ { posX, posY, posZ }, { rotX, rotY, rotZ } });
	}

	// read lights data
	GaiaData lightsData = levelData.find("Lights");
	nLights = lightsData.size();
	for (int i = 0; i < nLights; i++)
	{
		std::stringstream ss(lightsData[i][0].getValue());
		std::string type;
		if (!(ss >> type))
		{
			LOG_ERROR("GAME", "invalid light type \"%s\"", lightsData[i][0].getValue().c_str());
			continue;
		}

		ss = std::stringstream(lightsData[i][1].getValue());
		double posX, posY, posZ;
		if (!(ss >> posX >> posY >> posZ))
		{
			LOG_ERROR("GAME", "invalid light position \"%s\"", lightsData[i][1].getValue().c_str());
			continue;
		}

		ss = std::stringstream(lightsData[i][2].getValue());
		float intensity;
		if (!(ss >> intensity))
		{
			LOG_ERROR("GAME", "invalid light intensity \"%s\"", lightsData[i][2].getValue().c_str());
			continue;
		}

		ss = std::stringstream(lightsData[i][3].getValue());
		double colX, colY, colZ;
		if (!(ss >> colX >> colY >> colZ))
		{
			LOG_ERROR("GAME", "invalid light colour \"%s\"", lightsData[i][3].getValue().c_str());
			continue;
		}

		ss = std::stringstream(lightsData[i][4].getValue());
		double dirX, dirY, dirZ;
		if (!(ss >> dirX >> dirY >> dirZ))
		{
			LOG_ERROR("GAME", "invalid light direction \"%s\"", lightsData[i][4].getValue().c_str());
			continue;
		}

		lights.push_back({ type, { posX, posY, posZ }, intensity, { colX, colY, colZ }, { dirX, dirY, dirZ } });
	}
}

void Game::playSong()
{
	//findGameObjectWithName("MainCamera")->getComponent<SoundEmitter>()->play(GameManager::GetInstance()->getSong());
}

void Game::configureLevelRender(const std::string& name)
{
	GameObject* levelRender = findGameObjectWithName("LevelRender");
	if (levelRender == nullptr)
	{
		LOG_ERROR("GAME", "LevelRender object not found on scene");
		return;
	}

	MeshRenderer* meshRenderer = levelRender->getComponent<MeshRenderer>();
	if (meshRenderer == nullptr)
	{
		LOG_ERROR("GAME", "MeshRenderer not found"); return;
	}

	meshRenderer->setMesh("levelRender", name);
	meshRenderer->attachEntityToNode("levelRender");
}

void Game::configureLevelCollider(const std::string& name)
{
	GameObject* levelCollider = findGameObjectWithName("LevelCollider");
	if (levelCollider == nullptr)
	{
		LOG_ERROR("GAME", "LevelCollider object not found on scene"); return;
	}

	MeshRenderer* meshRenderer = levelCollider->getComponent<MeshRenderer>();
	if (meshRenderer == nullptr)
	{
		LOG_ERROR("GAME", "MeshRenderer not found"); return;
	}

	Strider* strider = levelCollider->getComponent<Strider>();
	if (strider == nullptr)
	{
		LOG_ERROR("GAME", "Strider not found"); return;
	}

	meshRenderer->setMesh("levelCollider", name);
	meshRenderer->attachEntityToNode("levelCollider");
	meshRenderer->setVisible(false);
	strider->stride("levelCollider");
	strider->setFriction(0.5f);
}

void Game::createKnights()
{
	int nPlayers = gameManager->getNumPlayers();

	gameManager->getKnights().clear();

	for (int i = 0; i < nPlayers; i++)
	{
		GameObject* knight = instantiate("Player", playerTransforms[i].first);
		if (knight == nullptr) break;
		knight->transform->setRotation(playerTransforms[i].second);

		knight->getComponent<Health>()->setHealth(gameManager->getHealth());

		knight->getComponent<PlayerController>()->setControllerIndex(playerIndexes[i]);
		knight->getComponent<PlayerIndex>()->setIndex(i + 1);
		knight->getComponent<MeshRenderer>()->setDiffuse(0, playerColours[i], 1);
		knight->getComponent<GhostManager>()->setPlayerColour(playerColours[i]);

		gameManager->getKnights().push_back(knight);
	}
}

void Game::createSpikes()
{
	for (int i = 0; i < nSpikes; i++)
	{
		GameObject* spikes = instantiate("Spikes", spikesTransforms[i].first);
		spikes->transform->setRotation(spikesTransforms[i].second);
	}
}

void Game::createLights()
{
	for (int i = 0; i < nLights; i++)
	{
		GameObject* light = instantiate("Light", lights[i].position);
		Light* lightComp = light->getComponent<Light>();

		if (lights[i].type == "Point")
			lightComp->setType(Light::Point);
		else if (lights[i].type == "Spotlight")
			lightComp->setType(Light::Spotlight);
		else if (lights[i].type == "Directional")
			lightComp->setType(Light::Directional);

		lightComp->setIntensity(lights[i].intensity);
		lightComp->setColour(lights[i].colour.x, lights[i].colour.y, lights[i].colour.z);
		light->transform->setDirection(lights[i].direction);
	}
}

void Game::chooseWinner()
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

		if (health->isAlive())
		{
			if (health->getHealth() > majorHealth)
			{
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