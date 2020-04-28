#include "OptionsConfiguration.h"


#include <InterfaceSystem.h>
#include <SoundSystem.h>
#include <WindowManager.h>
#include <RenderSystem.h>
#include <Window.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <UILayout.h>
#include <UIElement.h>
#include <Camera.h>

#include "GameManager.h"
#include "ComponentRegister.h"

REGISTER_FACTORY(OptionsConfiguration);

// EVENTS----

bool OptionsConfiguration::resolutionButtonClick()
{
	if (currResolution != resolution)
	{
		if (fullscreen)
		{
			windowManager->setFullscreen(false);
			windowManager->windowResize(resolutions[resolution].first, resolutions[resolution].second);
			windowManager->setFullscreen(true);
			currResolution = resolution;
			windowManager->setActualResolutionId(currResolution);
		}
		else
		{
			windowManager->windowResize(resolutions[resolution].first, resolutions[resolution].second);
			currResolution = resolution;
			windowManager->setActualResolutionId(currResolution);
		}
	}
	
	return false;
}

bool OptionsConfiguration::changeResolution(int value)
{
	resolution += value;

	if (resolution < 0) resolution = 0;
	if (resolution > resolutionNames.size() - 1) resolution = resolutionNames.size() - 1;

	resolutionText.setText(resolutionNames[resolution]);

	return false;
}

bool OptionsConfiguration::changeFullscreen(bool value)
{
	fullscreen = value;
	windowManager->setFullscreen(fullscreen);

	return false;
}

bool OptionsConfiguration::changeSoundVolume()
{
	volumeText.setText(std::to_string((int)(volumeScroll.getScrollPositionScrollBar() *MAX_VOLUME+0.5)));
	soundSystem->setSoundEffectsVolume(volumeScroll.getScrollPositionScrollBar());
	
	return false;
}

bool OptionsConfiguration::changeMusicVolume()
{
	musicText.setText(std::to_string((int)(musicScroll.getScrollPositionScrollBar() * MAX_VOLUME+0.5)));
	soundSystem->setMusicVolume(musicScroll.getScrollPositionScrollBar());

	return false;
}

bool OptionsConfiguration::changeGamma()
{
	gammaText.setText(std::to_string((int)(gammaScroll.getScrollPositionScrollBar() * MAX_GAMMA +0.5)));
	renderSystem->changeParamOfShader("LuminancePS", "brigh", gammaScroll.getScrollPositionScrollBar());
	windowManager->setBrightness(gammaScroll.getScrollPositionScrollBar());

	return false;
}

bool OptionsConfiguration::backButtonClick()
{
	SceneManager::GetInstance()->changeScene("mainMenu");
	return false;
}

// -----

OptionsConfiguration::OptionsConfiguration(GameObject* gameObject) : UserComponent(gameObject), resolutionButton(NULL), volumeScroll(NULL),musicScroll(NULL),gammaScroll(NULL), interfaceSystem(nullptr),windowManager(nullptr),renderSystem(nullptr),soundSystem(nullptr),
resolutionText(NULL), volumeText(NULL), musicText(NULL), gammaText(NULL),root(NULL)
{
	interfaceSystem = InterfaceSystem::GetInstance();
	interfaceSystem->registerEvent("-resolutionButtonClick", UIEvent("ButtonClicked", [this]() {return changeResolution(-1); }));
	interfaceSystem->registerEvent("+resolutionButtonClick", UIEvent("ButtonClicked", [this]() {return changeResolution(+1); }));

	interfaceSystem->registerEvent("fullscreenYesButtonClick", UIEvent("ToggleClicked", [this]() {return changeFullscreen(!fullscreen); }));

	interfaceSystem->registerEvent("volumeScrollChange", UIEvent("ScrollChange", [this]() {return changeSoundVolume(); }));
	interfaceSystem->registerEvent("volumeMusicScrollChange", UIEvent("ScrollChange", [this]() {return changeMusicVolume(); }));
	interfaceSystem->registerEvent("gammaScrollChange", UIEvent("ScrollChange", [this]() {return changeGamma(); }));

	interfaceSystem->registerEvent("resolutionApplyButtonClick", UIEvent("ButtonClicked", [this]() {return resolutionButtonClick(); }));
	interfaceSystem->registerEvent("backButtonClick", UIEvent("ButtonClicked", [this]() {return backButtonClick(); }));
	currResolution = -1;

	windowManager = WindowManager::GetInstance();
	renderSystem = RenderSystem::GetInstance();
	soundSystem = SoundSystem::GetInstance();

	resolutionNames = windowManager->getAvailableResolutionsStrings();
	resolutions = windowManager->getAvailableResolutionsForWindow();
}

OptionsConfiguration::~OptionsConfiguration()
{
	interfaceSystem->unregisterEvent("-resolutionButtonClick");
	interfaceSystem->unregisterEvent("+resolutionButtonClick");

	interfaceSystem->unregisterEvent("fullscreenYesButtonClick");

	interfaceSystem->unregisterEvent("volumeScrollChange");
	interfaceSystem->unregisterEvent("volumeMusicScrollChange");
	interfaceSystem->unregisterEvent("gammaScrollChange");

	interfaceSystem->unregisterEvent("resolutionApplyButtonClick");
	interfaceSystem->unregisterEvent("backButtonClick");
}

void OptionsConfiguration::start()
{
	root = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot();
	resolutionButton = root.getChild("ResolutionApplyButton");
	
	volumeScroll = root.getChild("SoundScroll");
	musicScroll= root.getChild("MusicScroll");
	gammaScroll= root.getChild("GammaScroll");
	resolutionText = root.getChild("Resolution");
	volumeText = root.getChild("SoundVolume");
	musicText = root.getChild("MusicVolume");
	gammaText=root.getChild("Gamma");
	UIElement checkbox = root.getChild("FullscreenYesButton");




	musicVolume = soundSystem->getMusicVolume();
	soundsVolume = soundSystem->getSoundVolume();
	gamma = windowManager->getBrightness();

	resolution = windowManager->getActualResolutionId();
	//fullscreen = WindowManager::GetInstance()->getFullscreen();

	changeResolution(0);
	musicScroll.setScrollPositionScrollBar(musicVolume);
	volumeScroll.setScrollPositionScrollBar(soundsVolume);
	gammaScroll.setScrollPositionScrollBar(gamma);
	checkbox.setCheckBoxState(windowManager->getFullscreen());

	
}