#include "Game.h"
#include <ComponentRegister.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <MeshRenderer.h>
#include <RigidBody.h>
#include <Light.h>
#include <Strider.h>
#include <UILayout.h>
#include <ParticleEmitter.h>
#include <GaiaData.h>

#include "PlayerController.h"
#include "PlayerIndex.h"
#include "Score.h"
#include "Health.h"
#include "GhostManager.h"
#include "Countdown.h"
#include "ConfigurationMenu.h"
#include "GameManager.h"
#include "SongManager.h"

REGISTER_FACTORY(Game);

Game::Game(GameObject* gameObject) : UserComponent(gameObject), gameManager(nullptr), songManager(nullptr), gameLayout(nullptr), countdown(nullptr), timePanel(NULL),
nLights(0), nSpikes(0), winner(-1), timer(-1.0f)
{

}

Game::~Game()
{

}

void Game::start()
{
	gameManager = GameManager::GetInstance();
	songManager = SongManager::GetInstance();

	GameObject* mainCamera = findGameObjectWithName("MainCamera");
	if (mainCamera != nullptr)
		gameLayout = mainCamera->getComponent<UILayout>();

	if (gameLayout != nullptr)
		timePanel = gameLayout->getRoot().getChild("TimeBackground");

	countdown = findGameObjectWithName("Countdown")->getComponent<Countdown>();

	playerIndexes = gameManager->getPlayerIndexes();
	playerColours = gameManager->getPlayerColours();

	// create game
	createLevel();
	createSpikes();
	createKnights();
	createLights();
	#ifndef RECORD_PATH
		createAI();
		gameManager->getScore()->initScore(/*4, gameManager->getPlayerIndexes()*/gameManager->getInitialPlayers());
	#else
		gameManager->getScore()->initScore(gameManager->getInitialPlayers());
	#endif


	timer = gameManager->getTime();
	gameManager->setPaused(false);

	playSong();
}

void Game::update(float deltaTime)
{
	if (!countdown->isCounting() && timer > 0)
	{
		if (!timePanel.isVisible())
			timePanel.setVisible(true);

		timePanel.getChild("Time").setText(timeToText().first + " : " + timeToText().second);

		timer -= deltaTime;
		if (timer < 0.0f)
			timer = 0;
	}
	else if (timer == 0) // If its negative it means match its not timed
		chooseWinner();
}

void Game::playerDie(int index)
{
	int nPlayers = gameManager->getPlayersAlive();
	gameManager->setPlayerRanking(index, nPlayers);

	nPlayers--;

	if (nPlayers <= 1)
		chooseWinner();
	else
		gameManager->setPlayersAlive(nPlayers);
}

void Game::createLevel()
{
	GaiaData levelData;
	levelData.load("./Assets/Levels/" + gameManager->getLevel().first + ".level");

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

	// Read Particles
	GaiaData particlesData = levelData.find("Particles");
	GameObject* levelParticles = findGameObjectWithName("LevelParticles");
	if (levelParticles == nullptr)
	{
		LOG("LevelParticles not found");
		return;
	}

	// Create Particles
	for (int i = 0; i < particlesData.size(); i++)
	{
		if (particlesData[i].size() < 2) continue;

		// Get name and position
		std::string name = particlesData[i][0].getValue();
		std::stringstream ss(particlesData[i][1].getValue());
		Vector3 position; ss >> position.x >> position.y >> position.z;

		// Create Particle Emitter through blueprint
		GameObject* particlesObject = instantiate("ParticleEmitter");
		if (particlesObject == nullptr) continue;

		levelParticles->addChild(particlesObject);
		particlesObject->transform->setPosition(position);

		ParticleEmitter* particleEmitter = particlesObject->getComponent<ParticleEmitter>();
		if (particleEmitter == nullptr) continue;

		particleEmitter->newEmitter(name);
		particleEmitter->start();
	}
}

void Game::createKnights()
{
	int nPlayers = gameManager->getInitialPlayers();

	gameManager->emptyKnights();

	for (int i = 0; i < nPlayers; i++)
	{
		GameObject* knight = instantiate("Player", playerTransforms[i].first);
		if (knight == nullptr) break;

		knight->transform->setRotation(playerTransforms[i].second);

		knight->getComponent<PlayerController>()->setControllerIndex(playerIndexes[i]);
		knight->getComponent<PlayerIndex>()->setIndex(i + 1);

		knight->getComponent<MeshRenderer>()->setDiffuse(0, playerColours[i], 1);
		knight->getComponent<MeshRenderer>()->setDiffuse("sword", 0, playerColours[i], 1);

		knight->getComponent<Health>()->setHealth(gameManager->getHealth());

		knight->getComponent<GhostManager>()->setPlayerColour(playerColours[i]);
		gameManager->getKnights().push_back(knight);
	}
}

void Game::createAI()
{
	int nPlayers = gameManager->getNumPlayers();
	int nAIPlayers = 1; // MAX_PLAYERS - nPlayers;

	for (int i = 0; i < nAIPlayers; i++)
	{
		GameObject* knight = instantiate("EnemyAI", playerTransforms[nPlayers + i].first);
		knight->transform->setRotation(playerTransforms[nPlayers + i].second);

		knight->getComponent<Health>()->setHealth(gameManager->getHealth());

		//knight->getComponent<PlayerController>()->setActive(false);
		knight->getComponent<PlayerIndex>()->setIndex(nPlayers + i + 1);

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

void Game::playSong()
{
	songManager->playSong(gameManager->getSong().first);
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
	PlatformGraph* graph = gameObject->getComponent<PlatformGraph>();
	if (graph != nullptr)
	{
		graph->setLoadFileName(GameManager::GetInstance()->getLevel() + ".graph");
		graph->setSaveFileName(GameManager::GetInstance()->getLevel() + ".graph");
	}
}

void Game::chooseWinner()
{
	std::vector<GameObject*> knights = gameManager->getKnights();

	bool tie = false;
	int majorHealth = 0;
	int majorIndex = 0;
	int tieIndex = 0;

	for (int i = 0; i < knights.size(); i++)
	{
		Health* health = knights[i]->getComponent<Health>();
		if (health == nullptr)
			continue;

		if (health->isAlive())
		{
			if (health->getHealth() > majorHealth)
			{
				majorHealth = health->getHealth();
				majorIndex = i;
				tie = false;
			}
			else if (health->getHealth() == majorHealth)
			{
				tieIndex = i;
				tie = true;
			}
		}
	}

	if (tie)
	{
		for (int i = 0; i < knights.size(); i++)
		{
			if (i == majorIndex || i == tieIndex)
				gameManager->setPlayerRanking(i + 1, 1);
			else
				gameManager->setPlayerRanking(i + 1, gameManager->getPlayerRanking(i + 1) - 1);
		}
		gameManager->setWinner(-1);
	}
	else
	{
		gameManager->setPlayerRanking(majorIndex + 1, 1);
		gameManager->setWinner(majorIndex + 1);
	}

	gameManager->emptyKnights();
	gameManager->pauseAllSounds();

	songManager->play2DSound("victory4");
	songManager->pauseSong(gameManager->getSong().first);

	SceneManager::GetInstance()->changeScene("StatsMenu");
}

std::pair<std::string, std::string> Game::timeToText()
{
	std::string minutes = std::to_string((int)timer / 60);
	std::string seconds;

	if ((int)timer % 60 < 10)
		seconds = "0" + std::to_string((int)timer % 60);
	else
		seconds = std::to_string((int)timer % 60);

	return std::pair<std::string, std::string>(minutes, seconds);
}