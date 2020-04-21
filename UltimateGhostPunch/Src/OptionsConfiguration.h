#pragma once
#ifndef OPTIONS_CONFIGURATION_H
#define OPTIONS_CONFIGURATION_H

#include "UserComponent.h"

#include <vector>
#include <string>
#include "UIElement.h"

const int MIN_VOLUME = 0;
const int MAX_VOLUME = 100;
const int CHANGE_VOLUME = 5;

const int MIN_GAMMA = 0;
const int MAX_GAMMA = 100;
const int CHANGE_GAMMA = 5;

const std::vector<std::pair<int, int>> RESOLUTIONS = { {640,480},{800,600},{960,720},{1024,576},{1024,768},{1152,648},{1280,720},{1280,800},{1280,960},{1366,768},
{1400,1050},{1440,900},{1440,1080},{1600,900},{1600,1200},
{1680,1050},{1856,1392},{1920,1080}, {1920,1200},{1920,1440} };


class OptionsConfiguration : public UserComponent
{
	UIElement resolutionButton;
	UIElement volumeScroll;
	UIElement musicScroll;
	UIElement gammaScroll;

	int musicVolume;
	int soundsVolume;
	int gamma;

	bool fullscreen;
	int resolution;
	int currResolution;

	std::vector<std::string> resolutionNames = { "640x480","800x600","960x720","1024x576","1024x768","1152x648","1280x720","1280x800","1280x960","1366x768",
												 "1400x1050","1440x900","1440x1080","1600x900","1600x1200","1680x1050","1856x1392","1920x1080", "1920x1200",
												 "1920x1440" };


private:
	// events
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