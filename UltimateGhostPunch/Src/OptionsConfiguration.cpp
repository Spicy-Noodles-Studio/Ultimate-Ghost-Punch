#include "OptionsConfiguration.h"

#include "InputSystem.h"
#include "GameObject.h"
#include "UILayout.h"
#include "UIElement.h"
#include <Camera.h>

#include <SceneManager.h>
#include <InterfaceSystem.h>
#include "GameManager.h"
#include "ComponentRegister.h"
#include <SoundSystem.h>
#include <RenderSystem.h>
#include <SceneManager.h>
#include <Window.h>

REGISTER_FACTORY(OptionsConfiguration);

// EVENTS----

bool OptionsConfiguration::resolutionButtonClick()
{
	// set data
	//SceneManager::GetInstance()->setFullscreen(false, RESOLUTIONS[resolution].first, RESOLUTIONS[resolution].second);
	//fullscreen = false;
	if (fullscreen ) {
		if (currResolution != resolution)
		{
			RenderSystem::GetInstance()->setFullscreen(false);
			RenderSystem::GetInstance()->windowResize(RESOLUTIONS[resolution].first, RESOLUTIONS[resolution].second);
			RenderSystem::GetInstance()->setFullscreen(true);
			currResolution = resolution;
		}
	}
	else
		if (currResolution != resolution)
		{
			RenderSystem::GetInstance()->windowResize(RESOLUTIONS[resolution].first, RESOLUTIONS[resolution].second);
			currResolution = resolution;
		}
	
	return false;
}

bool OptionsConfiguration::changeMusicVolume(int value)
{
	musicVolume += value;

	if (musicVolume < MIN_VOLUME) musicVolume = MIN_VOLUME;
	if (musicVolume > MAX_VOLUME) musicVolume = MAX_VOLUME;

	findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("MusicVolume").setText(std::to_string(musicVolume));

	SoundSystem::GetInstance()->setMusicVolume((float)musicVolume/ (float)MAX_VOLUME);

	return false;
}
bool OptionsConfiguration::changeSoundVolume(int value)
{
	soundsVolume += value;

	if (soundsVolume < MIN_VOLUME) soundsVolume = MIN_VOLUME;
	if (soundsVolume > MAX_VOLUME) soundsVolume = MAX_VOLUME;

	findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("SoundVolume").setText(std::to_string(soundsVolume));
	SoundSystem::GetInstance()->setSoundEffectsVolume((float)soundsVolume / (float)MAX_VOLUME);
	return false;
}

bool OptionsConfiguration::changeGamma(int value)
{
	gamma += value;

	if (gamma < MIN_GAMMA) gamma = MIN_GAMMA;
	if (gamma > MAX_GAMMA) gamma = MAX_GAMMA;

	findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("Gamma").setText(std::to_string(gamma));

	RenderSystem::GetInstance()->changeParamOfShader("LuminancePS", "brigh", (float)gamma/ (float)MAX_GAMMA);

	return false;
}

bool OptionsConfiguration::changeResolution(int value)
{
	resolution += value;

	if (resolution < 0) resolution = 0;
	if (resolution > resolutionNames.size() - 1) resolution = resolutionNames.size() - 1;

	findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("Resolution").setText(resolutionNames[resolution]);

	return false;
}

bool OptionsConfiguration::changeFullscreen(bool value)
{
	
	//if (fullscreen != value)
	//{
		fullscreen = value;
	
		RenderSystem::GetInstance()->setFullscreen(fullscreen);
	//}
	
	//if (level < 0) level = 0;
	//if (level > levelNames.size() - 1) level = levelNames.size() - 1;
	
	//findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("Level").setText(levelNames[level]);

	return false;
}

// -----

OptionsConfiguration::OptionsConfiguration(GameObject* gameObject) :
	UserComponent(gameObject), resolutionButton(NULL)
{
	InterfaceSystem::GetInstance()->registerEvent("-musicVolumeButtonClick", UIEvent("ButtonClicked", [this]() {return changeMusicVolume(-CHANGE_VOLUME); }));
	InterfaceSystem::GetInstance()->registerEvent("+musicVolumeButtonClick", UIEvent("ButtonClicked", [this]() {return changeMusicVolume(+CHANGE_VOLUME); }));

	InterfaceSystem::GetInstance()->registerEvent("-soundVolumeButtonClick", UIEvent("ButtonClicked", [this]() {return changeSoundVolume(-CHANGE_VOLUME); }));
	InterfaceSystem::GetInstance()->registerEvent("+soundVolumeButtonClick", UIEvent("ButtonClicked", [this]() {return changeSoundVolume(+CHANGE_VOLUME); }));

	InterfaceSystem::GetInstance()->registerEvent("-gammaButtonClick", UIEvent("ButtonClicked", [this]() {return changeGamma(-CHANGE_GAMMA); }));
	InterfaceSystem::GetInstance()->registerEvent("+gammaButtonClick", UIEvent("ButtonClicked", [this]() {return changeGamma(+CHANGE_GAMMA); }));

	InterfaceSystem::GetInstance()->registerEvent("-resolutionButtonClick", UIEvent("ButtonClicked", [this]() {return changeResolution(-1); }));
	InterfaceSystem::GetInstance()->registerEvent("+resolutionButtonClick", UIEvent("ButtonClicked", [this]() {return changeResolution(+1); }));

	InterfaceSystem::GetInstance()->registerEvent("fullscreenYesButtonClick", UIEvent("ToggleClicked", [this]() {return changeFullscreen(!fullscreen); }));
	//InterfaceSystem::GetInstance()->registerEvent("fullscreenNoButtonClick", UIEvent("ButtonClicked", [this]() {return changeFullscreen(false); }));

	InterfaceSystem::GetInstance()->registerEvent("resolutionApplyButtonClick", UIEvent("ButtonClicked", [this]() {return resolutionButtonClick(); }));
	currResolution = -1;
}

OptionsConfiguration::~OptionsConfiguration()
{
	InterfaceSystem::GetInstance()->unregisterEvent("-musicVolumeButtonClick");
	InterfaceSystem::GetInstance()->unregisterEvent("+musicVolumeButtonClick");

	InterfaceSystem::GetInstance()->unregisterEvent("-soundVolumeButtonClick");
	InterfaceSystem::GetInstance()->unregisterEvent("+soundVolumeButtonClick");

	InterfaceSystem::GetInstance()->unregisterEvent("-gammaButtonClick");
	InterfaceSystem::GetInstance()->unregisterEvent("+gammaButtonClick");

	InterfaceSystem::GetInstance()->unregisterEvent("-resolutionButtonClick");
	InterfaceSystem::GetInstance()->unregisterEvent("+resolutionButtonClick");

	InterfaceSystem::GetInstance()->unregisterEvent("fullscreenYesButtonClick");
	//InterfaceSystem::GetInstance()->unregisterEvent("fullscreenNoButtonClick");

	InterfaceSystem::GetInstance()->unregisterEvent("resolutionApplyButtonClick");
}

void OptionsConfiguration::start()
{
	inputSystem = InputSystem::GetInstance();

	resolutionButton = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("ResolutionApplyButton");

	musicVolume = 100;
	soundsVolume = 100;
	gamma = 100;
	resolution = 6;
	fullscreen = false;
}
