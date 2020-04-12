#pragma once
#ifndef SCORE_MANAGER_H
#define SCORE_MANAGER_H

#include <UserComponent.h>
#include <vector>
#include <string>
#include "UIElement.h"

class GameManager;

class ScoreManager :
	public UserComponent
{
	GameManager* manager;
	UIElement player1Text;
	UIElement player2Text;
	UIElement player3Text;
	UIElement player4Text;
	
	

private:
		// events
	bool backButtonClick();
	bool resetButtonClick();

	


public:
	ScoreManager(GameObject* gameObject);
	virtual ~ScoreManager();

	virtual void start();
};


#endif