#pragma once
#ifndef OPTIONS_CONFIGURATION_H
#define OPTIONS_CONFIGURATION_H

#include "UserComponent.h"

#include <vector>
#include <string>
#include "UIElement.h"

const int MAX_VOLUME = 100;
const int MAX_GAMMA = 100;



class InterfaceSystem;
class WindowManager;
class RenderSystem;
class SoundSystem;

class OptionsConfiguration : public UserComponent
{
protected:
	InterfaceSystem* interfaceSystem;
	WindowManager* windowManager;
	RenderSystem* renderSystem;
	SoundSystem* soundSystem;

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
	// events
	bool resolutionButtonClick();
	bool resetConfigButtonClick();

	bool changeResolution(int value);
	bool changeFullscreen(bool value);

	bool changeSoundVolume();
	bool changeMusicVolume();
	bool changeGamma();

	bool backButtonClick();

public:
	OptionsConfiguration(GameObject* gameObject);
	virtual ~OptionsConfiguration();

	virtual void start();
};

#endif