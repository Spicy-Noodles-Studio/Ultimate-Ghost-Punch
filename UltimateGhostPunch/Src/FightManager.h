#pragma once
#ifndef FIGHT_MANAGER_H
#define FIGHT_MANAGER_H

#include <UserComponent.h>
#include <UIElement.h>
#include <Vector3.h>

#include <vector>
#include <string>

class GameManager;
class UILayout;
class InputSystem;

class FightManager : public UserComponent
{
private:
	struct LightData {
		std::string type;
		Vector3 position;
		float intensity;
		Vector3 colour;
		Vector3 direction;
	};

	GameManager* gameManager;
	UILayout* fightLayout;
	UIElement timeText;
	UIElement winnerPanel;
	UIElement winnerText;

	std::vector<int> playerIndexes;

	float fightTimer; // If time is -1, then infinite
	float finishTimer; // Time taken to send us back to MainMenu

	std::vector<std::pair<Vector3, Vector3>> playerTransforms;
	std::vector<std::pair<Vector3, Vector3>> spikesTransforms;
	std::vector<LightData> lights; // position, intensity, colour, direction
	int nSpikes;
	int nLights;

	int winner;

	void createLevel();
	void createKnights();
	void createSpikes();
	void createLights();
	void playSong();

	void configureLevelRender(const std::string& name);
	void configureLevelCollider(const std::string& name);

	void chooseWinner();

public:
	FightManager(GameObject* gameObject);
	virtual ~FightManager();

	virtual void start();
	virtual void update(float deltaTime);

	void playerDie();

};

#endif