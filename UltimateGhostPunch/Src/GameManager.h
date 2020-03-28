#pragma once
#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Singleton.h"

#include <vector>
#include <string>

class GameObject;

class GameManager : public Singleton<GameManager>
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
	~GameManager();

};

#endif