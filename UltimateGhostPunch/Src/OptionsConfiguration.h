#pragma once
#ifndef OPTIONS_CONFIGURATION_H
#define OPTIONS_CONFIGURATION_H

#include <UserComponent.h>
#include <UIElement.h>

#include <vector>
#include <string>

const int MAX_VOLUME = 100;
const int MAX_GAMMA = 100;

class InterfaceSystem;
class RenderSystem;
class SoundSystem;
class WindowManager;

class OptionsConfiguration : public UserComponent
{
	InterfaceSystem* interfaceSystem;
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

	UIElement root;

	float musicVolume;
	float soundsVolume;
	float gamma;

	bool fullscreen;
	int resolution;
	int currResolution;

	std::vector<std::string> resolutionNames;
	std::vector<std::pair<int, int>> resolutions;

private:
	bool resolutionButtonClick();

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