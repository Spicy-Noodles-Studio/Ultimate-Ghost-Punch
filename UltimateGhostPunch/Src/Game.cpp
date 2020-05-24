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
#include "PlayerState.h"
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
timePanel(NULL), players(0), winner(-1), timer(-1.0f), fadeIn(true), darkness(false), end(false)
{

}

Game::~Game()
{
	gameManager = nullptr;
	songManager = nullptr;
	gameLayout = nullptr;
	countdown = nullptr;
	cameraEffects = nullptr;

	playerColours.clear();
	playerIndexes.clear();
	lights.clear();
	playerTransforms.clear();
	spikesTransforms.clear();
}

void Game::start()
{
	gameManager = GameManager::GetInstance();
	songManager = SongManager::GetInstance();
	checkNull(gameManager);
	checkNull(songManager);

	GameObject* mainCamera = findGameObjectWithName("MainCamera");
	if (notNull(mainCamera)) {
		setCameraLimits(mainCamera);
		cameraEffects = mainCamera->getComponent<CameraEffects>();
		gameLayout = mainCamera->getComponent<UILayout>();
		if (notNull(gameLayout))
			timePanel = gameLayout->getRoot().getChild("TimeBackground");
	}
	checkNull(cameraEffects);

	GameObject* countdownObject = findGameObjectWithName("Countdown");
	if (notNull(countdownObject)) countdown = countdownObject->getComponent<Countdown>();
	checkNull(countdown);

	if (notNull(gameManager)) {
		playerIndexes = gameManager->getPlayerIndexes();
		playerColours = gameManager->getPlayerColours();

		Score* score = gameManager->getScore();
		if (notNull(score)) score->initScore(gameManager->getInitialPlayers());

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
	if (notNull(countdown) && !countdown->isCounting() && timer > 0)
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
		if (notNull(cameraEffects)) cameraEffects->setDarkness();
		darkness = true;
	}
	else if (fadeIn && notNull(countdown) && countdown->getRemainingTime() < 2.6)
	{
		if (notNull(cameraEffects)) cameraEffects->fadeIn();
		fadeIn = false;
	}

	if (end && notNull(cameraEffects) && !cameraEffects->isFading())
	{
		if (notNull(gameManager)) {
			if (notNull(songManager)) songManager->pauseSong(gameManager->getSong().first);
			gameManager->pauseAllSounds();
			gameManager->emptyKnights();
		}
		if (notNull(SceneManager::GetInstance()))
			SceneManager::GetInstance()->changeScene("StatsMenu");
	}
}

void Game::playerDeath()
{
	players--;

	if (players <= 1)
		chooseWinner();
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
	if (notNull(mainCamera) || notNull(gameManager)) return;

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
	checkNullAndBreak(gameManager);

	GaiaData levelData;
	levelData.load("./Assets/Levels/" + gameManager->getLevel().first + ".level");

	std::string renderName = levelData.find("RenderMesh").getValue();
	std::string colliderName = levelData.find("ColliderMesh").getValue();

	// Configuration of the visual mesh
	configureLevelRender(renderName);

	//  Configuration of the collision mesh
	configureLevelCollider(colliderName);

	// Read player initial transforms
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

	// Read spikes transforms
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

	// Read lights data
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
		if (!notNull(particlesObject) || !notNull(particlesObject->transform)) continue;

		levelParticles->addChild(particlesObject);
		particlesObject->transform->setPosition(position);

		ParticleEmitter* particleEmitter = particlesObject->getComponent<ParticleEmitter>();
		if (!notNull(particleEmitter)) continue;

		particleEmitter->newEmitter(name);
		particleEmitter->start();
	}
}

void Game::createKnights()
{
	if (notNull(gameManager)) gameManager->emptyKnights();

	for (int i = 0; i < playerIndexes.size(); i++)
	{
		if (playerIndexes[i] != -1)
		{
			players++;
			GameObject* knight = nullptr;

			if (playerIndexes[i] != 9 && i < playerTransforms.size())
			{
				knight = instantiate("Player", playerTransforms[i].first);
				if (!notNull(knight) || !notNull(knight->transform) || !notNull(knight->getComponent<PlayerController>())) break;

				knight->transform->setRotation(playerTransforms[i].second);
				knight->getComponent<PlayerController>()->setControllerIndex(playerIndexes[i]);
			}
			else
			{
				knight = instantiate("EnemyAI", playerTransforms[i].first);
				if (!notNull(knight) || !notNull(knight->transform)) return;

				knight->transform->setRotation(playerTransforms[i].second);
			}

			checkNullAndBreak(knight);
			PlayerIndex* index = knight->getComponent<PlayerIndex>();
			if (notNull(index)) index->setIndex(i + 1);

			if (i < playerColours.size()) {
				MeshRenderer* mesh = knight->getComponent<MeshRenderer>();
				if (notNull(mesh)) {
					mesh->setDiffuse(0, playerColours[i], 1);
					mesh->setDiffuse("sword", 0, playerColours[i], 1);
				}

				GhostManager* ghostManager = knight->getComponent<GhostManager>();
				if (notNull(ghostManager)) ghostManager->setPlayerColour(playerColours[i]);
			}

			if (notNull(gameManager)) {
				Health* health = knight->getComponent<Health>();
				if (notNull(health)) health->setHealth(gameManager->getHealth());

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
		if (notNull(spikes) && notNull(spikes->transform))
			spikes->transform->setRotation(spikesTransforms[i].second);
	}
}

void Game::createLights()
{
	for (int i = 0; i < lights.size(); i++)
	{
		GameObject* light = instantiate("Light", lights[i].position);
		if (notNull(light)) {
			Light* lightComp = light->getComponent<Light>();

			if (notNull(lightComp)) {
				if (lights[i].type == "Point")
					lightComp->setType(Light::Point);
				else if (lights[i].type == "Spotlight")
					lightComp->setType(Light::Spotlight);
				else if (lights[i].type == "Directional")
					lightComp->setType(Light::Directional);

				lightComp->setIntensity(lights[i].intensity);
				lightComp->setColour(lights[i].colour.x, lights[i].colour.y, lights[i].colour.z);
			}
			if (light->transform) light->transform->setDirection(lights[i].direction);
		}
	}
}

void Game::playSong()
{
	if (notNull(songManager) && notNull(gameManager))
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
	if (notNull(graph) && notNull(gameManager))
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

void Game::setRanking()
{
	checkNullAndBreak(gameManager);
	std::vector<GameObject*> knights = gameManager->getKnights();

	for (int i = 0; i < knights.size(); i++)
	{
		Health* health = knights[i]->getComponent<Health>();
		if (notNull(health))
			gameManager->getRanking().push(ii(i + 1, health->getHealth()));

		PlayerState* state = knights[i]->getComponent<PlayerState>();
		if (notNull(state))
			state->setIgnoringInput(true);
	}

	std::priority_queue<ii, std::vector<ii>, Less> aux = gameManager->getRanking();

	int cont = 0;
	bool tie = false;
	ii last = ii(0, 0);

	while (!aux.empty())
	{
		ii info = aux.top();
		aux.pop();

		if (info.second != 0 && info.second == last.second)
			tie = true;
		else
			cont++;

		gameManager->setPlayerRanking(info.first, cont);
		last = info;
	}

	if (tie)
		gameManager->setWinner(-1);
	else
		gameManager->setWinner(gameManager->getRanking().top().first);

	gameManager->emptyRanking();
}

void Game::chooseWinner()
{
	end = true;
	setRanking();

	if (notNull(cameraEffects))
		cameraEffects->fadeOut();

	if (notNull(songManager))
		songManager->play2DSound("victory4");
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