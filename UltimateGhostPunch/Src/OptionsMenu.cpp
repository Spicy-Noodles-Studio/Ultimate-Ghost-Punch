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

bool OptionsMenu::resolutionButtonClick()
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

bool OptionsMenu::resetConfigButtonClick()
{
	fullscreen = false;
	resolution = 1;
	soundsVolume = 100;
	musicVolume = 100;
	gamma = 100;

	changeResolution(0);

	musicScroll.setScrollPositionScrollBar(musicVolume);
	volumeScroll.setScrollPositionScrollBar(soundsVolume);
	gammaScroll.setScrollPositionScrollBar(gamma);

	checkbox.setCheckBoxState(false);
	changeFullscreen(false);

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

bool OptionsMenu::changeFullscreen(bool value)
{
	fullscreen = value;
	windowManager->setFullscreen(fullscreen);

	return false;
}

bool OptionsMenu::changeSoundVolume()
{
	volumeText.setText(std::to_string((int)(volumeScroll.getScrollPositionScrollBar() * MAX_VOLUME + 0.5)));
	soundSystem->setSoundEffectsVolume(volumeScroll.getScrollPositionScrollBar());

	return false;
}

bool OptionsMenu::changeMusicVolume()
{
	musicText.setText(std::to_string((int)(musicScroll.getScrollPositionScrollBar() * MAX_VOLUME + 0.5)));
	soundSystem->setMusicVolume(musicScroll.getScrollPositionScrollBar());

	return false;
}

bool OptionsMenu::changeGamma()
{
	gammaText.setText(std::to_string((int)(gammaScroll.getScrollPositionScrollBar() * MAX_GAMMA + 0.5)));
	renderSystem->changeParamOfShader("LuminancePS", "brigh", gammaScroll.getScrollPositionScrollBar());
	windowManager->setBrightness(gammaScroll.getScrollPositionScrollBar());

	return false;
}

bool OptionsMenu::backButtonClick()
{
	SceneManager::GetInstance()->changeScene("MainMenu");
	return false;
}

// -----

OptionsMenu::OptionsMenu(GameObject* gameObject) : UserComponent(gameObject), resolutionButton(NULL), volumeScroll(NULL), musicScroll(NULL), gammaScroll(NULL),
interfaceSystem(nullptr), renderSystem(nullptr), soundSystem(nullptr), windowManager(nullptr),
resolutionText(NULL), volumeText(NULL), musicText(NULL), gammaText(NULL), checkbox(NULL), root(NULL)
{
	interfaceSystem = InterfaceSystem::GetInstance();
	interfaceSystem->registerEvent("-resolutionButtonClick", UIEvent("ButtonClicked", [this]() {return changeResolution(-1); }));
	interfaceSystem->registerEvent("+resolutionButtonClick", UIEvent("ButtonClicked", [this]() {return changeResolution(+1); }));

	interfaceSystem->registerEvent("fullscreenYesButtonClick", UIEvent("ToggleClicked", [this]() {return changeFullscreen(!fullscreen); }));

	interfaceSystem->registerEvent("volumeScrollChange", UIEvent("ScrollChange", [this]() {return changeSoundVolume(); }));
	interfaceSystem->registerEvent("volumeMusicScrollChange", UIEvent("ScrollChange", [this]() {return changeMusicVolume(); }));
	interfaceSystem->registerEvent("gammaScrollChange", UIEvent("ScrollChange", [this]() {return changeGamma(); }));

	interfaceSystem->registerEvent("resetConfigurationsButtonClick", UIEvent("ButtonClicked", [this]() {return resetConfigButtonClick(); }));
	interfaceSystem->registerEvent("resolutionApplyButtonClick", UIEvent("ButtonClicked", [this]() {return resolutionButtonClick(); }));
	interfaceSystem->registerEvent("backButtonClick", UIEvent("ButtonClicked", [this]() {return backButtonClick(); }));
	currResolution = -1;

	windowManager = WindowManager::GetInstance();
	renderSystem = RenderSystem::GetInstance();
	soundSystem = SoundSystem::GetInstance();

	resolutionNames = windowManager->getAvailableResolutionsStrings();
	resolutions = windowManager->getAvailableResolutionsForWindow();
}

OptionsMenu::~OptionsMenu()
{
	interfaceSystem->unregisterEvent("-resolutionButtonClick");
	interfaceSystem->unregisterEvent("+resolutionButtonClick");

	interfaceSystem->unregisterEvent("fullscreenYesButtonClick");

	interfaceSystem->unregisterEvent("volumeScrollChange");
	interfaceSystem->unregisterEvent("volumeMusicScrollChange");
	interfaceSystem->unregisterEvent("gammaScrollChange");

	interfaceSystem->unregisterEvent("resetConfigurationsButtonClick");
	interfaceSystem->unregisterEvent("resolutionApplyButtonClick");
	interfaceSystem->unregisterEvent("backButtonClick");
}

void OptionsMenu::start()
{
	root = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("OptionsBackground");
	root.setVisible(true);

	resolutionButton = root.getChild("ResolutionApplyButton");

	volumeScroll = root.getChild("SoundScroll");
	musicScroll = root.getChild("MusicScroll");
	gammaScroll = root.getChild("GammaScroll");

	resolutionText = root.getChild("Resolution");

	volumeText = root.getChild("SoundVolume");
	musicText = root.getChild("MusicVolume");
	gammaText = root.getChild("Gamma");

	checkbox = root.getChild("FullscreenYesButton");

	UIElement checkbox = root.getChild("FullscreenYesButton");

	musicVolume = soundSystem->getMusicVolume();
	soundsVolume = soundSystem->getSoundVolume();
	gamma = windowManager->getBrightness();

	resolution = windowManager->getActualResolutionId();

	if (resolution == 0)
		resolution = 1;

	changeResolution(0);

	musicScroll.setScrollPositionScrollBar(musicVolume);
	volumeScroll.setScrollPositionScrollBar(soundsVolume);
	gammaScroll.setScrollPositionScrollBar(gamma);

	checkbox.setCheckBoxState(windowManager->getFullscreen());
}