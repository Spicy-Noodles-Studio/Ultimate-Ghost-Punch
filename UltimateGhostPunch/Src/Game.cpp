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
#include "GameManager.h"
#include "SongManager.h"
#include "PlatformGraph.h"
#include "CameraEffects.h"
#include "CameraController.h"

REGISTER_FACTORY(Game);

Game::Game(GameObject* gameObject) : UserComponent(gameObject), gameManager(nullptr), songManager(nullptr), gameLayout(nullptr), countdown(nullptr), cameraEffects(nullptr),
timePanel(NULL), winner(-1), timer(-1.0f), fadeIn(true), darkness(false), end(false)
{

}

Game::~Game()
{

}

void Game::start()
{
	gameManager = GameManager::GetInstance();
	songManager = SongManager::GetInstance();
	checkNull(gameManager);
	checkNull(songManager);

	GameObject* mainCamera = findGameObjectWithName("MainCamera");
	if (mainCamera != nullptr) {
		setCameraLimits(mainCamera);
		cameraEffects = mainCamera->getComponent<CameraEffects>();
		gameLayout = mainCamera->getComponent<UILayout>();
		if (gameLayout != nullptr)
			timePanel = gameLayout->getRoot().getChild("TimeBackground");
	}
	checkNull(mainCamera);
	checkNull(gameLayout);
	checkNull(cameraEffects);

	GameObject* countdownObject = findGameObjectWithName("Countdown");
	if (countdownObject != nullptr) countdown = countdownObject->getComponent<Countdown>();
	checkNull(countdown);

	if (gameManager != nullptr) {
		playerIndexes = gameManager->getPlayerIndexes();
		playerColours = gameManager->getPlayerColours();

		Score* score = gameManager->getScore();
		if (score != nullptr) score->initScore(gameManager->getInitialPlayers());
		checkNull(score);

		timer = gameManager->getTime();
		gameManager->setPaused(false);
	}

	// create game
	createLevel();
	createSpikes();
	createKnights();
	createLights();

	playSong();
}

void Game::update(float deltaTime)
{
	if (countdown != nullptr && !countdown->isCounting() && timer > 0)
	{
		if (!timePanel.isVisible())
			timePanel.setVisible(true);

		timePanel.getChild("Time").setText(timeToText().first + " : " + timeToText().second);

		timer -= deltaTime;
		if (timer < 0.0f)
			timer = 0;
	}
	else if (timer == 0 && !end) // If its negative it means match its not timed
		chooseWinner();

	if (!darkness)
	{
		if (cameraEffects != nullptr) cameraEffects->setDarkness();
		darkness = true;
	}
	else if (fadeIn && countdown != nullptr && countdown->getRemainingTime() < 2.6)
	{
		if (cameraEffects != nullptr) cameraEffects->fadeIn();
		fadeIn = false;
	}

	if (end && cameraEffects != nullptr && !cameraEffects->isFading())
	{
		if (gameManager != nullptr) {
			if (songManager != nullptr) songManager->pauseSong(gameManager->getSong().first);
			gameManager->pauseAllSounds();
			gameManager->emptyKnights();
		}
		SceneManager::GetInstance()->changeScene("StatsMenu");
	}
}

void Game::playerDie(int index)
{
	if (gameManager == nullptr) return;
	int nPlayers = gameManager->getPlayersAlive();
	gameManager->setPlayerRanking(index, nPlayers);

	nPlayers--;

	if (nPlayers <= 1) chooseWinner();
	else gameManager->setPlayersAlive(nPlayers);
}

Vector3 Game::getPlayerInitialPosition(int player)
{
	if (player > 0 && player <= playerTransforms.size())
		return playerTransforms[player - 1].first;

	return Vector3::ZERO;
}

CameraEffects* Game::getCameraEffects()
{
	return cameraEffects;
}

void Game::setCameraLimits(GameObject* mainCamera)
{
	if (mainCamera == nullptr || gameManager == nullptr) return;
	CameraController* camController = mainCamera->getComponent<CameraController>();
	checkNullAndBreak(camController);

	if (gameManager->getLevel().first == "level2")
	{
		camController->setMaxZ(60); // Increase max zoom away for largest level
		camController->setMinY(0);
		camController->setMaxY(40);
	}
	else
	{
		camController->setMaxZ(50);
		camController->setMinY(-10);
		camController->setMaxY(20);
	}
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
		if (playerData[i].size() < 2) continue;

		std::stringstream ss(playerData[i][0].getValue());
		Vector3 pos = Vector3::ZERO;
		if (!(ss >> pos.x >> pos.y >> pos.z))
		{
			LOG_ERROR("GAME", "invalid player position \"%s\"", playerData[i][0].getValue().c_str());
			continue;
		}

		ss = std::stringstream(playerData[i][1].getValue());
		Vector3 rot = Vector3::ZERO;
		if (!(ss >> rot.x >> rot.y >> rot.z))
		{
			LOG_ERROR("GAME", "invalid player rotation \"%s\"", playerData[i][1].getValue().c_str());
			continue;
		}
		playerTransforms.push_back({ pos, rot });
	}

	// read spikes transforms
	GaiaData spikesData = levelData.find("SpikesTransforms");
	for (int i = 0; i < spikesData.size(); i++)
	{
		if (spikesData[i].size() < 2) continue;

		std::stringstream ss(spikesData[i][0].getValue());
		Vector3 pos = Vector3::ZERO;
		if (!(ss >> pos.x >> pos.y >> pos.z))
		{
			LOG_ERROR("GAME", "invalid spikes position \"%s\"", spikesData[i][0].getValue().c_str());
			continue;
		}

		ss = std::stringstream(spikesData[i][1].getValue());
		Vector3 rot = Vector3::ZERO;
		if (!(ss >> rot.x >> rot.y >> rot.z))
		{
			LOG_ERROR("GAME", "invalid spikes rotation \"%s\"", spikesData[i][1].getValue().c_str());
			continue;
		}
		spikesTransforms.push_back({ pos, rot });
	}

	// read lights data
	GaiaData lightsData = levelData.find("Lights");
	for (int i = 0; i < lightsData.size(); i++)
	{
		if (lightsData[i].size() < 5) continue;

		std::stringstream ss(lightsData[i][0].getValue());
		std::string type;
		if (!(ss >> type))
		{
			LOG_ERROR("GAME", "invalid light type \"%s\"", lightsData[i][0].getValue().c_str());
			continue;
		}

		ss = std::stringstream(lightsData[i][1].getValue());
		Vector3 pos = Vector3::ZERO;
		if (!(ss >> pos.x >> pos.y >> pos.z))
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
		Vector3 color = Vector3::ZERO;
		if (!(ss >> color.x >> color.y >> color.z))
		{
			LOG_ERROR("GAME", "invalid light colour \"%s\"", lightsData[i][3].getValue().c_str());
			continue;
		}

		ss = std::stringstream(lightsData[i][4].getValue());
		Vector3 dir = Vector3::ZERO;
		if (!(ss >> dir.x >> dir.y >> dir.z))
		{
			LOG_ERROR("GAME", "invalid light direction \"%s\"", lightsData[i][4].getValue().c_str());
			continue;
		}

		lights.push_back({ type, pos, intensity, color, dir });
	}

	// Read Particles
	GaiaData particlesData = levelData.find("Particles");
	GameObject* levelParticles = findGameObjectWithName("LevelParticles");
	checkNullAndBreak(levelParticles);

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
		if (particlesObject == nullptr || particlesObject->transform == nullptr) continue;

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
	if(gameManager!=nullptr) gameManager->emptyKnights();

	for (int i = 0; i < playerIndexes.size(); i++)
	{
		if (playerIndexes[i] != -1)
		{
			GameObject* knight = nullptr;

			if (playerIndexes[i] != 9 && i < playerTransforms.size())
			{
				knight = instantiate("Player", playerTransforms[i].first);
				if (knight == nullptr || knight->transform == nullptr || knight->getComponent<PlayerController>() == nullptr) break;

				knight->transform->setRotation(playerTransforms[i].second);
				knight->getComponent<PlayerController>()->setControllerIndex(playerIndexes[i]);
			}
			else
			{
				knight = instantiate("EnemyAI", playerTransforms[i].first);
				if (knight == nullptr || knight->transform == nullptr) return;

				knight->transform->setRotation(playerTransforms[i].second);
			}
			PlayerIndex* index = knight->getComponent<PlayerIndex>();
			if(index != nullptr) index->setIndex(i + 1);

			if (i < playerColours.size()) {
				MeshRenderer* mesh = knight->getComponent<MeshRenderer>();
				if (mesh != nullptr) {
					mesh->setDiffuse(0, playerColours[i], 1);
					mesh->setDiffuse("sword", 0, playerColours[i], 1);
				}

				GhostManager* ghostManager = knight->getComponent<GhostManager>();
				if (ghostManager != nullptr) ghostManager->setPlayerColour(playerColours[i]);
			}

			if (gameManager != nullptr) {
				Health* health = knight->getComponent<Health>();
				if (health != nullptr) health->setHealth(gameManager->getHealth());

				gameManager->getKnights().push_back(knight);
			}
		}
	}
}

void Game::createSpikes()
{
	for (int i = 0; i < spikesTransforms.size(); i++)
	{
		GameObject* spikes = instantiate("Spikes", spikesTransforms[i].first);
		if (spikes != nullptr && spikes->transform != nullptr)
			spikes->transform->setRotation(spikesTransforms[i].second);
	}
}

void Game::createLights()
{
	for (int i = 0; i < lights.size(); i++)
	{
		GameObject* light = instantiate("Light", lights[i].position);
		if (light != nullptr) {
			Light* lightComp = light->getComponent<Light>();

			if (lightComp != nullptr) {
				if (lights[i].type == "Point")
					lightComp->setType(Light::Point);
				else if (lights[i].type == "Spotlight")
					lightComp->setType(Light::Spotlight);
				else if (lights[i].type == "Directional")
					lightComp->setType(Light::Directional);

				lightComp->setIntensity(lights[i].intensity);
				lightComp->setColour(lights[i].colour.x, lights[i].colour.y, lights[i].colour.z);
			}
			light->transform->setDirection(lights[i].direction);
		}
	}
}

void Game::playSong()
{
	if(songManager != nullptr && gameManager != nullptr)
	songManager->playSong(gameManager->getSong().first);
}

void Game::configureLevelRender(const std::string& name)
{
	GameObject* levelRender = findGameObjectWithName("LevelRender");
	checkNullAndBreak(levelRender);

	MeshRenderer* meshRenderer = levelRender->getComponent<MeshRenderer>();
	checkNullAndBreak(meshRenderer);

	meshRenderer->setMesh("levelRender", name);
	meshRenderer->attachEntityToNode("levelRender");
}

void Game::configureLevelCollider(const std::string& name)
{
	GameObject* levelCollider = findGameObjectWithName("LevelCollider");
	checkNullAndBreak(levelCollider);

	MeshRenderer* meshRenderer = levelCollider->getComponent<MeshRenderer>();
	checkNullAndBreak(meshRenderer);

	Strider* strider = levelCollider->getComponent<Strider>();
	checkNullAndBreak(strider);

	meshRenderer->setMesh("levelCollider", name);
	meshRenderer->attachEntityToNode("levelCollider");
	meshRenderer->setVisible(false);

	strider->stride("levelCollider");
	strider->setFriction(0.5f);

	int i = 0;
	bool ia = false;
	while (i < playerIndexes.size() && !ia)
	{
		if (playerIndexes[i] == 9)
			ia = true;
		i++;
	}

	PlatformGraph* graph = levelCollider->getComponent<PlatformGraph>();
	if (graph != nullptr && gameManager != nullptr)
	{
		if (!ia)
			graph->setActive(false);
		else
		{
			graph->setLoadFileName(gameManager->getLevel().second + "Graph.graph");
			graph->setSaveFileName(gameManager->getLevel().second + "Graph.graph");
		}
	}
}

void Game::chooseWinner()
{
	if(cameraEffects != nullptr) cameraEffects->fadeOut();
	end = true;

	if (gameManager == nullptr) return;
	std::vector<GameObject*> knights =  gameManager->getKnights();

	bool tie = false;
	int majorHealth = 0;
	int majorIndex = 0;
	int tieIndex = 0;

	for (int i = 0; i < knights.size(); i++)
	{
		if (knights[i] == nullptr) continue;

		Health* health = knights[i]->getComponent<Health>();
		if (health == nullptr) continue;

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

	if(songManager != nullptr) songManager->play2DSound("victory4");
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