#include "OptionsMenuScreen.h"
#include <ComponentRegister.h>
#include <InterfaceSystem.h>
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

OptionsMenuScreen::OptionsMenuScreen(GameObject* gameObject) : OptionsConfiguration(gameObject), screen(nullptr), pauseMenu(NULL), optionsMenu(NULL)
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

	resolutionButton = optionsMenu.getChild("ResolutionApplyButton");

	volumeScroll = optionsMenu.getChild("SoundScroll");
	musicScroll = optionsMenu.getChild("MusicScroll");
	gammaScroll = optionsMenu.getChild("GammaScroll");

	resolutionText = optionsMenu.getChild("Resolution");

	volumeText = optionsMenu.getChild("SoundVolume");
	musicText = optionsMenu.getChild("MusicVolume");
	gammaText = optionsMenu.getChild("Gamma");

	checkbox = optionsMenu.getChild("FullscreenYesButton");

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