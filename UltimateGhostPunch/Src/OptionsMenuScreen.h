#pragma once
#include "OptionsConfiguration.h"



class GameObject;
class OptionsMenuScreen :
	public OptionsConfiguration
{
private:
	GameObject *screen;
	UIElement pauseMenu;
	UIElement optionsMenu;



	bool backToMenuButtonClick();

public:	
	OptionsMenuScreen(GameObject* gameObject);
	  virtual ~OptionsMenuScreen();

	  virtual void start();
};

