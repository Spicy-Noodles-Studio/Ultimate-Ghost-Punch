#include "OptionsMenu.h"
#include <ComponentRegister.h>
#include <InterfaceSystem.h>
#include <InputSystem.h>
#include <RenderSystem.h>
#include <SoundSystem.h>
#include <WindowManager.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <UILayout.h>
#include <Camera.h>
#include <Window.h>
#include <SoundEmitter.h>

#include "GameManager.h"

REGISTER_FACTORY(OptionsMenu);

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
		
	buttonClick(buttonSound);

	return false;
}

bool OptionsMenu::restoreButtonClick()
{
	brightness = gameManager->getInitialBrightness();
	soundVolume = 100;
	musicVolume = 100;

	fullscreen = false;
	resolution = initialResolution;

	changeResolution(0);
	changeFullscreen(fullscreen);

	brightnessScroll.setScrollPositionScrollBar(brightness);
	soundScroll.setScrollPositionScrollBar(soundVolume);
	musicScroll.setScrollPositionScrollBar(musicVolume);

	buttonClick(buttonSound);

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

	buttonClick(buttonSound);

	return false;
}

bool OptionsMenu::changeFullscreen(int value)
{
	fullscreen = value;
	fullscreenText.setText(screenNames[fullscreen]);

	buttonClick(buttonSound);

	return false;
}

bool OptionsMenu::changeBrightness()
{
	brightnessText.setText(std::to_string((int)(brightnessScroll.getScrollPositionScrollBar() * MAX_VALUE + 0.5)));
	renderSystem->changeParamOfShader("Brightness", "bright", brightnessScroll.getScrollPositionScrollBar() + 0.5);
	windowManager->setBrightness(brightnessScroll.getScrollPositionScrollBar());

	buttonClick(buttonSound);

	return false;
}

bool OptionsMenu::changeSoundVolume()
{
	soundText.setText(std::to_string((int)(soundScroll.getScrollPositionScrollBar() * MAX_VALUE + 0.5)));
	soundSystem->setSoundEffectsVolume(soundScroll.getScrollPositionScrollBar());

	buttonClick(buttonSound);

	return false;
}

bool OptionsMenu::changeMusicVolume()
{
	musicText.setText(std::to_string((int)(musicScroll.getScrollPositionScrollBar() * MAX_VALUE + 0.5)));
	soundSystem->setMusicVolume(musicScroll.getScrollPositionScrollBar());

	buttonClick(buttonSound);

	return false;
}

OptionsMenu::OptionsMenu(GameObject* gameObject) : Menu(gameObject), applyButton(NULL), restoreButton(NULL), brightnessScroll(NULL), soundScroll(NULL), musicScroll(NULL),
renderSystem(nullptr), soundSystem(nullptr), windowManager(nullptr), resolutionText(NULL), fullscreenText(NULL), brightnessText(NULL), soundText(NULL), musicText(NULL), root(NULL)
{
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
	Menu::start();

	if (mainCamera == nullptr) return;

	root = mainCamera->getComponent<UILayout>()->getRoot().getChild("OptionsBackground");
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
	initialResolution = resolution;

	changeFullscreen(fullscreen);
	changeResolution(0);
}

void OptionsMenu::update(float deltaTime)
{
	if ((inputSystem->getKeyPress("ESCAPE") || checkControllersInput()) && SceneManager::GetInstance()->getCurrentScene()->getName() == "OptionsMenu")
		backButtonClick();
}

bool OptionsMenu::checkControllersInput()
{
	bool result = false;

	int i = 0;
	while (i < 4 && !result)
	{
		if (inputSystem->getButtonPress(i, "B"))
			result = true;

		i++;
	}

	return result;
}