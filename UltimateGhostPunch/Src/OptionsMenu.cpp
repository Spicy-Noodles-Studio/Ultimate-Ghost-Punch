#include "OptionsMenu.h"
#include <ComponentRegister.h>
#include <InterfaceSystem.h>
#include <RenderSystem.h>
#include <SoundSystem.h>
#include <WindowManager.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <UILayout.h>
#include <Camera.h>
#include <Window.h>

#include "GameManager.h"

REGISTER_FACTORY(OptionsMenu);

// EVENTS----

bool OptionsMenu::backButtonClick()
{
	SceneManager::GetInstance()->changeScene("MainMenu");
	return false;
}

bool OptionsMenu::applyButtonClick()
{
	windowManager->setFullscreen(false);

	if (currentResolution != resolution)
	{
		windowManager->windowResize(resolutions[resolution].first, resolutions[resolution].second);
		currentResolution = resolution;
		windowManager->setActualResolutionId(currentResolution);
	}

	if (fullscreen)
		windowManager->setFullscreen(true);

	return false;
}

bool OptionsMenu::restoreButtonClick()
{
	brightness = 100;
	soundVolume = 100;
	musicVolume = 100;

	fullscreen = false;
	resolution = 0;
	currentResolution = 0;

	changeResolution(0);
	changeFullscreen(false);

	brightnessScroll.setScrollPositionScrollBar(brightness);
	soundScroll.setScrollPositionScrollBar(soundVolume);
	musicScroll.setScrollPositionScrollBar(musicVolume);

	return false;
}

bool OptionsMenu::changeResolution(int value)
{
	resolution += value;

	if (resolution < 0)
		resolution = 0;

	if (resolution > resolutionNames.size() - 1)
		resolution = resolutionNames.size() - 1;

	resolutionText.setText(resolutionNames[resolution]);

	return false;
}

bool OptionsMenu::changeFullscreen(int value)
{
	fullscreen = value;

	if (fullscreen < 0)
		fullscreen = 0;

	if (fullscreen > screenNames.size() - 1)
		fullscreen = screenNames.size() - 1;

	fullscreenText.setText(screenNames[fullscreen]);

	return false;
}

bool OptionsMenu::changeBrightness()
{
	brightnessText.setText(std::to_string((int)(brightnessScroll.getScrollPositionScrollBar() * MAX_VALUE + 0.5)));
	renderSystem->changeParamOfShader("LuminancePS", "brigh", brightnessScroll.getScrollPositionScrollBar());
	windowManager->setBrightness(brightnessScroll.getScrollPositionScrollBar());

	return false;
}

bool OptionsMenu::changeSoundVolume()
{
	soundText.setText(std::to_string((int)(soundScroll.getScrollPositionScrollBar() * MAX_VALUE + 0.5)));
	soundSystem->setSoundEffectsVolume(soundScroll.getScrollPositionScrollBar());

	return false;
}

bool OptionsMenu::changeMusicVolume()
{
	musicText.setText(std::to_string((int)(musicScroll.getScrollPositionScrollBar() * MAX_VALUE + 0.5)));
	soundSystem->setMusicVolume(musicScroll.getScrollPositionScrollBar());

	return false;
}

// -----

OptionsMenu::OptionsMenu(GameObject* gameObject) : UserComponent(gameObject), applyButton(NULL), restoreButton(NULL), brightnessScroll(NULL), soundScroll(NULL), musicScroll(NULL),
interfaceSystem(nullptr), renderSystem(nullptr), soundSystem(nullptr), windowManager(nullptr),
resolutionText(NULL), fullscreenText(NULL), brightnessText(NULL), soundText(NULL), musicText(NULL), root(NULL)
{
	interfaceSystem = InterfaceSystem::GetInstance();
	renderSystem = RenderSystem::GetInstance();
	soundSystem = SoundSystem::GetInstance();
	windowManager = WindowManager::GetInstance();

	interfaceSystem->registerEvent("-resolutionButtonClick", UIEvent("ButtonClicked", [this]() {return changeResolution(-1); }));
	interfaceSystem->registerEvent("+resolutionButtonClick", UIEvent("ButtonClicked", [this]() {return changeResolution(+1); }));

	interfaceSystem->registerEvent("-fullscreenButtonClick", UIEvent("ButtonClicked", [this]() {return changeFullscreen(!fullscreen); }));
	interfaceSystem->registerEvent("+fullscreenButtonClick", UIEvent("ButtonClicked", [this]() {return changeFullscreen(!fullscreen); }));

	interfaceSystem->registerEvent("brightnessScrollChange", UIEvent("ScrollChange", [this]() {return changeBrightness(); }));
	interfaceSystem->registerEvent("soundScrollChange", UIEvent("ScrollChange", [this]() {return changeSoundVolume(); }));
	interfaceSystem->registerEvent("musicScrollChange", UIEvent("ScrollChange", [this]() {return changeMusicVolume(); }));

	interfaceSystem->registerEvent("applyButtonClick", UIEvent("ButtonClicked", [this]() {return applyButtonClick(); }));
	interfaceSystem->registerEvent("restoreButtonClick", UIEvent("ButtonClicked", [this]() {return restoreButtonClick(); }));
	interfaceSystem->registerEvent("backButtonClick", UIEvent("ButtonClicked", [this]() {return backButtonClick(); }));

	resolutionNames = windowManager->getAvailableResolutionsStrings();
	screenNames = { "Windowed", "Fullscreen" };
	resolutions = windowManager->getAvailableResolutionsForWindow();
}

OptionsMenu::~OptionsMenu()
{
	interfaceSystem->unregisterEvent("-resolutionButtonClick");
	interfaceSystem->unregisterEvent("+resolutionButtonClick");

	interfaceSystem->unregisterEvent("-fullscreenButtonClick");
	interfaceSystem->unregisterEvent("+fullscreenButtonClick");

	interfaceSystem->unregisterEvent("brightnessScrollChange");
	interfaceSystem->unregisterEvent("soundScrollChange");
	interfaceSystem->unregisterEvent("musicScrollChange");

	interfaceSystem->unregisterEvent("applyButtonClick");
	interfaceSystem->unregisterEvent("restoreButtonClick");
	interfaceSystem->unregisterEvent("backButtonClick");
}

void OptionsMenu::start()
{
	root = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("OptionsBackground");
	root.setVisible(true);

	applyButton = root.getChild("ApplyButton");
	restoreButton = root.getChild("RestoreButton");

	brightnessScroll = root.getChild("BrightnessScroll");
	soundScroll = root.getChild("SoundScroll");
	musicScroll = root.getChild("MusicScroll");

	resolutionText = root.getChild("Resolution");
	fullscreenText = root.getChild("Fullscreen");
	brightnessText = root.getChild("Brightness");
	soundText = root.getChild("SoundVolume");
	musicText = root.getChild("MusicVolume");

	brightness = windowManager->getBrightness();
	musicVolume = soundSystem->getMusicVolume();
	soundVolume = soundSystem->getSoundVolume();

	brightnessScroll.setScrollPositionScrollBar(brightness);
	musicScroll.setScrollPositionScrollBar(musicVolume);
	soundScroll.setScrollPositionScrollBar(soundVolume);

	fullscreen = windowManager->getFullscreen();
	resolution = windowManager->getActualResolutionId();
	currentResolution = resolution;

	changeFullscreen(fullscreen);
	changeResolution(0);
}