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

	InterfaceSystem::GetInstance()->clearControllerMenuInput();

	pauseMenu.setAlwaysOnTop(true);
	pauseMenu.setVisible(true);

	return false;
}

OptionsMenuScreen::OptionsMenuScreen(GameObject* gameObject) : OptionsMenu(gameObject), screen(nullptr), pauseMenu(NULL), optionsMenu(NULL)
{
	interfaceSystem->registerEvent("backToMenuButtonClick", UIEvent("ButtonClicked", [this]() {return backToMenuButtonClick(); }));
}

OptionsMenuScreen::~OptionsMenuScreen()
{
	interfaceSystem->unregisterEvent("backToMenuButtonClick");
}

void OptionsMenuScreen::start()
{
	screen = findGameObjectWithName("OptionsMenuScreen");
	root = screen->getComponent<UILayout>()->getRoot();

	optionsMenu = root.getChild("OptionsBackground");
	optionsMenu.setVisible(true);

	root.setVisible(false);
	root.setEnabled(false);

	UILayout* cameraLayout = findGameObjectWithName("MainCamera")->getComponent<UILayout>();

	if (cameraLayout != nullptr)
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

	brightness = windowManager->getBrightness();
	soundVolume = soundSystem->getSoundVolume();
	musicVolume = soundSystem->getMusicVolume();

	brightnessScroll.setScrollPositionScrollBar(brightness);
	soundScroll.setScrollPositionScrollBar(soundVolume);
	musicScroll.setScrollPositionScrollBar(musicVolume);

	fullscreen = windowManager->getFullscreen();
	resolution = windowManager->getActualResolutionId();
	currentResolution = resolution;
	initialResolution = resolution;

	changeFullscreen(fullscreen);
	changeResolution(0);
}

void OptionsMenuScreen::preUpdate(float deltaTime)
{
	if ((inputSystem->getKeyPress("ESCAPE") || checkControllersInput()) && root.isVisible())
		backToMenuButtonClick();
}