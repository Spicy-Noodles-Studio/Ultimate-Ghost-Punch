#pragma once
#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Singleton.h"
#include "UserComponent.h"

#include <vector>
#include <string>

class GameObject;

class GameManager : public Singleton<GameManager>, public UserComponent
{
private:

	int currentScene;

	bool paused;

	// FIGHT DATA
	int nPlayers;
	std::vector<GameObject*> knights;
	int winner;

	std::string level;
	std::string song;

	// OPTIONS DATA

public:
	GameManager();
	GameManager(GameObject* gameObject);
	~GameManager();

	virtual void start();
	virtual void update(float deltaTime);

	void setLevel(std::string level);
	void setSong(std::string song);

	void setNPlayers(int nPlayers);
	int getNPlayers();
	
	std::vector<GameObject*>& getKnights();

};

#endif