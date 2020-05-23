#include "OptionsMenuScreen.h"

#include <ComponentRegister.h>
#include <InterfaceSystem.h>
#include <InputSystem.h>
#include <RenderSystem.h>
#include <SoundSystem.h>
#include <WindowManager.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <UILayout.h>
#include <UIElement.h>
#include <Window.h>
#include <Camera.h>

REGISTER_FACTORY(OptionsMenuScreen);

bool OptionsMenuScreen::backToMenuButtonClick()
{
	root.setAlwaysOnTop(false);
	root.setVisible(false);
	root.setEnabled(false);

	if (notNull(interfaceSystem))
		interfaceSystem->clearControllerMenuInput();

	pauseMenu.setAlwaysOnTop(true);
	pauseMenu.setVisible(true);

	buttonClick(backSound);

	return false;
}

OptionsMenuScreen::OptionsMenuScreen(GameObject* gameObject) : OptionsMenu(gameObject), screen(nullptr), pauseMenu(NULL), optionsMenu(NULL)
{
	if (notNull(interfaceSystem))
		interfaceSystem->registerEvent("backToMenuButtonClick", UIEvent("ButtonClicked", [this]() {return backToMenuButtonClick(); }));
}

OptionsMenuScreen::~OptionsMenuScreen()
{
	if (notNull(interfaceSystem))
		interfaceSystem->unregisterEvent("backToMenuButtonClick");
}

void OptionsMenuScreen::start()
{
	Menu::start();

	screen = findGameObjectWithName("OptionsMenuScreen");
	checkNullAndBreak(mainCamera);
	checkNullAndBreak(screen);

	UILayout* layout = screen->getComponent<UILayout>();
	if(notNull(layout)) root = layout->getRoot();

	optionsMenu = root.getChild("OptionsBackground");
	optionsMenu.setVisible(true);

	root.setVisible(false);
	root.setEnabled(false);

	UILayout* cameraLayout = mainCamera->getComponent<UILayout>();

	if (notNull(cameraLayout))
		pauseMenu = cameraLayout->getRoot().getChild("PauseBackground");

	applyButton = optionsMenu.getChild("ApplyButton");
	restoreButton = optionsMenu.getChild("RestoreButton");

	brightnessScroll = optionsMenu.getChild("BrightnessScroll");
	soundScroll = optionsMenu.getChild("SoundScroll");
	musicScroll = optionsMenu.getChild("MusicScroll");

	resolutionText = optionsMenu.getChild("Resolution");
	fullscreenText = optionsMenu.getChild("Fullscreen");
	brightnessText = optionsMenu.getChild("Brightness");
	soundText = optionsMenu.getChild("SoundVolume");
	musicText = optionsMenu.getChild("MusicVolume");

	if (notNull(windowManager)) {
		brightness = windowManager->getBrightness();
		fullscreen = windowManager->getFullscreen();
		resolution = windowManager->getActualResolutionId();
	}

	if (notNull(soundSystem)) {
		soundVolume = soundSystem->getSoundVolume();
		musicVolume = soundSystem->getMusicVolume();
	}

	brightnessScroll.setScrollPositionScrollBar(brightness);
	soundScroll.setScrollPositionScrollBar(soundVolume);
	musicScroll.setScrollPositionScrollBar(musicVolume);

	currentResolution = resolution;
	initialResolution = resolution;

	changeFullscreen(fullscreen);
	changeResolution(0);
}

void OptionsMenuScreen::preUpdate(float deltaTime)
{
	if (notNull(inputSystem) && (inputSystem->getKeyPress("ESCAPE") || checkControllersInput()) && root.isVisible())
		backToMenuButtonClick();
}