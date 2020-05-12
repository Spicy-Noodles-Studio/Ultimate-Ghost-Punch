#pragma once
#ifndef OPTIONS_MENU_H
#define OPTIONS_MENU_H

#include "Menu.h"

#include <UIElement.h>

#include <vector>
#include <string>

const int MAX_VOLUME = 100;
const int MAX_GAMMA = 100;

class InterfaceSystem;
class RenderSystem;
class SoundSystem;
class WindowManager;

class OptionsMenu : public Menu
{
protected:
	RenderSystem* renderSystem;
	SoundSystem* soundSystem;
	WindowManager* windowManager;

	UIElement resolutionButton;
	UIElement volumeScroll;
	UIElement musicScroll;
	UIElement gammaScroll;

	UIElement resolutionText;
	UIElement volumeText;
	UIElement musicText;
	UIElement gammaText;
	UIElement checkbox;

	UIElement root;

	float musicVolume;
	float soundsVolume;
	float gamma;

	bool fullscreen;
	int resolution;
	int currResolution;

	std::vector<std::string> resolutionNames;
	std::vector<std::pair<int, int>> resolutions;

protected:
	bool resolutionButtonClick();
	bool resetConfigButtonClick();

	bool changeResolution(int value);
	bool changeFullscreen(bool value);

	bool changeSoundVolume();
	bool changeMusicVolume();
	bool changeGamma();

public:
	OptionsMenu(GameObject* gameObject);
	virtual ~OptionsMenu();

	virtual void start();
};

#endif