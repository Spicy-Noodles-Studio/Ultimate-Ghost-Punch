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
	checkNullAndBreak(windowManager, false);
	windowManager->setFullscreen(false);

	if (currentResolution != resolution && resolution >= 0 && resolution < resolutions.size())
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
	if (notNull(gameManager)) {
		brightness = gameManager->getInitialBrightness();
		soundVolume = gameManager->getInitialSoundVolume();
		musicVolume = gameManager->getInitialMusicVolume();
	}

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
	if (notNull(windowManager)) windowManager->setBrightness(brightnessScroll.getScrollPositionScrollBar());

	buttonClick(buttonSound);

	return false;
}

bool OptionsMenu::changeSoundVolume()
{
	soundText.setText(std::to_string((int)(soundScroll.getScrollPositionScrollBar() * MAX_VALUE + 0.5)));
	if (notNull(soundSystem)) soundSystem->setSoundEffectsVolume(soundScroll.getScrollPositionScrollBar());

	buttonClick(buttonSound);

	return false;
}

bool OptionsMenu::changeMusicVolume()
{
	musicText.setText(std::to_string((int)(musicScroll.getScrollPositionScrollBar() * MAX_VALUE + 0.5)));
	if (notNull(soundSystem)) soundSystem->setMusicVolume(musicScroll.getScrollPositionScrollBar());

	buttonClick(buttonSound);

	return false;
}

OptionsMenu::OptionsMenu(GameObject* gameObject) : Menu(gameObject), applyButton(NULL), restoreButton(NULL), brightnessScroll(NULL), soundScroll(NULL), musicScroll(NULL),
renderSystem(nullptr), soundSystem(nullptr), windowManager(nullptr), resolutionText(NULL), fullscreenText(NULL), brightnessText(NULL), soundText(NULL), musicText(NULL), root(NULL)
{
	renderSystem = RenderSystem::GetInstance();
	soundSystem = SoundSystem::GetInstance();
	windowManager = WindowManager::GetInstance();

	checkNull(renderSystem);
	checkNull(soundSystem);
	checkNull(windowManager);

	if (notNull(interfaceSystem)) {
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
	}

	if (notNull(windowManager)) {
		resolutionNames = windowManager->getAvailableResolutionsStrings();
		screenNames = { "Windowed", "Fullscreen" };
		resolutions = windowManager->getAvailableResolutionsForWindow();
	}
}

OptionsMenu::~OptionsMenu()
{
	if (notNull(interfaceSystem)) {
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

	renderSystem = nullptr;
	soundSystem = nullptr;
	windowManager = nullptr;

	resolutionNames.clear();
	resolutions.clear();
	screenNames.clear();
}

void OptionsMenu::start()
{
	Menu::start();

	checkNullAndBreak(mainCamera);

	UILayout* layout = mainCamera->getComponent<UILayout>();
	if (notNull(layout))
		root = layout->getRoot().getChild("OptionsBackground");
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

	if (notNull(windowManager)) {
		brightness = windowManager->getBrightness();
		fullscreen = windowManager->getFullscreen();
		resolution = windowManager->getActualResolutionId();
	}

	if (notNull(soundSystem)) {
		musicVolume = soundSystem->getMusicVolume();
		soundVolume = soundSystem->getSoundVolume();
	}

	brightnessScroll.setScrollPositionScrollBar(brightness);
	musicScroll.setScrollPositionScrollBar(musicVolume);
	soundScroll.setScrollPositionScrollBar(soundVolume);

	currentResolution = resolution;
	initialResolution = resolution;

	changeFullscreen(fullscreen);
	changeResolution(0);
}

void OptionsMenu::update(float deltaTime)
{
	if (((notNull(inputSystem) && inputSystem->getKeyPress("ESCAPE")) || checkControllersInput()) &&
		notNull(sceneManager) && notNull(sceneManager->getCurrentScene()) && sceneManager->getCurrentScene()->getName() == "OptionsMenu")
		backButtonClick();
}

bool OptionsMenu::checkControllersInput()
{
	bool result = false;

	int i = 0;
	while (i < 4 && !result)
	{
		if (notNull(inputSystem) && inputSystem->getButtonPress(i, "B"))
			result = true;

		i++;
	}

	return result;
}