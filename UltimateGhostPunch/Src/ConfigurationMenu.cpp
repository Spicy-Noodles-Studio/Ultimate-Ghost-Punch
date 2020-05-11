#include "ConfigurationMenu.h"
#include <ComponentRegister.h>
#include <InterfaceSystem.h>
#include <InputSystem.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <UILayout.h>
#include <UIElement.h>

#include "GameManager.h"
#include "SongManager.h"

REGISTER_FACTORY(ConfigurationMenu);

// EVENTS----

bool ConfigurationMenu::changeHealth(int value)
{
	health += value;

	if (health < MIN_HEALTH) health = MIN_HEALTH;
	if (health > MAX_HEALTH) health = MAX_HEALTH;

	configLayout->getRoot().getChild("Health").setText(std::to_string(health));

	buttonClick(buttonSound);

	return false;
}

bool ConfigurationMenu::changeTime(int value)
{
	time += value;

	if (time < MIN_TIME) time = MIN_TIME;
	if (time > MAX_TIME) time = MAX_TIME;

	if (time == MAX_TIME)
		configLayout->getRoot().getChild("Time").setText("INFINITE");
	else
		configLayout->getRoot().getChild("Time").setText(std::to_string(time));

	buttonClick(buttonSound);

	return false;
}

bool ConfigurationMenu::changeSong(int value)
{
	stopPreview();

	auto it = songNames.find(currentSong);
	if (it == songNames.end()) return false;

	if (value == -1 && it != songNames.begin())
		it--;

	if (value == 1 && it != prev(songNames.end()))
		it++;

	currentSong = (*it).first;
	configLayout->getRoot().getChild("Song").setText(currentSong);

	buttonClick(buttonSound);

	return false;
}

bool ConfigurationMenu::changeLevel(int value)
{
	auto it = levelNames.find(currentLevel);
	if (it == levelNames.end()) return false;

	if (value == -1 && it != levelNames.begin())
		it--;

	if (value == 1 && it != prev(levelNames.end()))
		it++;

	currentLevel = (*it).first;
	configLayout->getRoot().getChild("Level").setText(currentLevel);

	buttonClick(buttonSound);

	return false;
}

bool ConfigurationMenu::previewSong()
{
	if (!songPreview) {
		songManager->pauseMenuSong();
		songManager->playSong(songNames[currentSong]);
		songPreview = true;
		timer = previewTime;
	}
	return false;
}

void ConfigurationMenu::stopPreview()
{
	if (songPreview) {
		songManager->stopSong(songNames[currentSong]);
		songManager->resumeMenuSong();
		songPreview = false;
	}
}

bool ConfigurationMenu::fightButtonClick()
{
	buttonClick(fightSound);

	stopPreview();
	songManager->stopMenuSong();

	// set data
	gameManager->setLevel(levelNames[currentLevel]);
	gameManager->setSong(songNames[currentSong]);
	gameManager->setNumPlayers(numPlayers);
	gameManager->setHealth(health);

	std::vector<int> indexes;
	for (int i = 0; i < numPlayers; i++)
		indexes.push_back(slots[i].first);

	gameManager->setPlayerIndexes(indexes);

	if (time != MAX_TIME)
		gameManager->setTime(time);
	else
		gameManager->setTime(-1);

	// change scene
	SceneManager::GetInstance()->changeScene("Game", true);
	
	return false;
}

void ConfigurationMenu::initNames()
{
	songNames["Bustin Loose"] = "bustinLoose";
	songNames["Cycles"] = "cycles";
	songNames["District Four"] = "districtFour";
	songNames["Drama"] = "tvDrama";
	songNames["Epic Battle"] = "epicBattle";
	songNames["Fight"] = "fightScene";
	songNames["Green Daze"] = "greenDaze";
	songNames["Hip Hop"] = "hipHopInstrumental";
	songNames["Hip Hop 2"] = "hipHopNoVocal";
	songNames["Opus One"] = "opusOne";
	songNames["Strength"] = "strengthOfTheTitans";
	songNames["UGP"] = "ugpTrack1";
	currentSong = "Bustin Loose";
	configLayout->getRoot().getChild("Song").setText(currentSong);


	levelNames["Coliseum"] = "level4";
	levelNames["Sewers"] = "level3";
	levelNames["Volcano"] = "level5";
	currentLevel = "Coliseum";
	configLayout->getRoot().getChild("Level").setText(currentLevel);
}

bool ConfigurationMenu::backButtonClick()
{
	stopPreview();
	return Menu::backButtonClick();
}

// -----

ConfigurationMenu::ConfigurationMenu(GameObject* gameObject) : Menu(gameObject), configLayout(nullptr), fightButton(NULL),
numPlayers(0), health(0), time(0), currentLevel(""), currentSong(""), previewTime(50), songPreview(false), timer(0)
{
	interfaceSystem->registerEvent("-healthButtonClick", UIEvent("ButtonClicked", [this]() {return changeHealth(-1); }));
	interfaceSystem->registerEvent("+healthButtonClick", UIEvent("ButtonClicked", [this]() {return changeHealth(+1); }));

	interfaceSystem->registerEvent("-timeButtonClick", UIEvent("ButtonClicked", [this]() {return changeTime(-20); }));
	interfaceSystem->registerEvent("+timeButtonClick", UIEvent("ButtonClicked", [this]() {return changeTime(+20); }));

	interfaceSystem->registerEvent("-songButtonClick", UIEvent("ButtonClicked", [this]() {return changeSong(-1); }));
	interfaceSystem->registerEvent("+songButtonClick", UIEvent("ButtonClicked", [this]() {return changeSong(+1); }));

	interfaceSystem->registerEvent("-levelButtonClick", UIEvent("ButtonClicked", [this]() {return changeLevel(-1); }));
	interfaceSystem->registerEvent("+levelButtonClick", UIEvent("ButtonClicked", [this]() {return changeLevel(+1); }));

	interfaceSystem->registerEvent("fightButtonClick", UIEvent("ButtonClicked", [this]() {return fightButtonClick(); }));
	interfaceSystem->registerEvent("backButtonClick", UIEvent("ButtonClicked", [this]() {return backButtonClick(); }));

	interfaceSystem->registerEvent("previewSongButtonClick", UIEvent("ButtonClicked", [this]() {return previewSong(); }));
}

ConfigurationMenu::~ConfigurationMenu()
{
	interfaceSystem->unregisterEvent("-healthButtonClick");
	interfaceSystem->unregisterEvent("+healthButtonClick");

	interfaceSystem->unregisterEvent("-timeButtonClick");
	interfaceSystem->unregisterEvent("+timeButtonClick");

	interfaceSystem->unregisterEvent("-songButtonClick");
	interfaceSystem->unregisterEvent("+songButtonClick");

	interfaceSystem->unregisterEvent("-levelButtonClick");
	interfaceSystem->unregisterEvent("+levelButtonClick");

	interfaceSystem->unregisterEvent("fightButtonClick");
	interfaceSystem->unregisterEvent("backButtonClick");

	interfaceSystem->unregisterEvent("previewSongClick");
}

void ConfigurationMenu::start()
{
	Menu::start();

	if (mainCamera != nullptr)
		configLayout = mainCamera->getComponent<UILayout>();

	if (configLayout != nullptr)
		fightButton = configLayout->getRoot().getChild("FightButton");

	initNames();

	timer = 0;
	time = 60;
	health = 4;
	numPlayers = 0;

	slots = std::vector<std::pair<int, UIElement>>(4, { -1, NULL });
	for (int i = 0; i < 4; i++)
	{
		if (configLayout != nullptr)
			slots[i] = { -1 , configLayout->getRoot().getChild("Slot" + std::to_string(i + 1)).getChild("Connected" + std::to_string(i + 1)) };
	}

}

void ConfigurationMenu::update(float deltaTime)
{
	checkInput();

	if (songPreview && timer > 0)
		timer -= deltaTime;
	else if (songPreview)
		stopPreview();
}

void ConfigurationMenu::checkInput()
{
	for (int i = 0; i < 5; i++)
	{
		int slotIndex = isIndexConnected(i);

		bool enterButton = (i < 4 && inputSystem->getButtonPress(i, "A"))	// Controller
			|| (i == 4 && inputSystem->getKeyPress("Space"));	// Keyboard

		bool exitButton = (i < 4 && (inputSystem->getButtonPress(i, "B") || !inputSystem->isControllerConnected(i)))	// Controller
			|| (i == 4 && inputSystem->getKeyPress("ESCAPE"));												// Keyboard

		if (numPlayers < 4 && slotIndex == -1 && enterButton)
			fillSlot(numPlayers, i);
		else if (slotIndex != -1 && exitButton)
		{
			clearSlot(slotIndex);
			reorderSlots(slotIndex);
		}
	}
}

void ConfigurationMenu::fillSlot(int slotIndex, int deviceIndex)
{
	slots[slotIndex].first = deviceIndex;

	slots[slotIndex].second.setVisible(true);
	slots[slotIndex].second.getChild("PlayerText").setText("P" + std::to_string(slotIndex + 1));
	slots[slotIndex].second.getChild("IndexText").setText("Index: " + std::to_string(slots[slotIndex].first));

	if (deviceIndex == 4)
		slots[slotIndex].second.getChild("TypeText").setText("Keyboard");
	else
		slots[slotIndex].second.getChild("TypeText").setText("Controller");

	numPlayers++;

	if (!fightButton.isVisible() && numPlayers >= MIN_PLAYERS)
		fightButton.setVisible(true);
}

void ConfigurationMenu::clearSlot(int index)
{
	slots[index].first = -1;
	slots[index].second.setVisible(false);

	numPlayers--;

	if (fightButton.isVisible() && numPlayers < MIN_PLAYERS)
		fightButton.setVisible(false);
}

void ConfigurationMenu::reorderSlots(int index)
{
	for (int i = index; i < numPlayers; i++)
	{
		fillSlot(i, slots[i + 1].first);
		clearSlot(i + 1);
	}
}

int ConfigurationMenu::isIndexConnected(int index)
{
	int i = 0;
	while (i < numPlayers - 1 && slots[i].first != index)
		i++;

	if (slots[i].first == index)
		return i;

	return -1;
}