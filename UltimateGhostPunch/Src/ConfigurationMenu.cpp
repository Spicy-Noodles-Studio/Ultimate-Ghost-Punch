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

ConfigurationMenu::ConfigurationMenu(GameObject* gameObject) : Menu(gameObject), configurationLayout(nullptr), startButton(NULL), settingsPanel(NULL),
nPlayers(0), health(0), time(60), mode(false), currentLevel(""), currentSong(""), previewTime(50), timer(0), songPreview(false), slots(), timeModes(), levelNames(), songNames()
{
	if (notNull(interfaceSystem)) {
		interfaceSystem->registerEvent("-healthButtonClick", UIEvent("ButtonClicked", [this]() {return changeHealth(-CHANGE_HEALTH); }));
		interfaceSystem->registerEvent("+healthButtonClick", UIEvent("ButtonClicked", [this]() {return changeHealth(+CHANGE_HEALTH); }));

		interfaceSystem->registerEvent("-timeModeButtonClick", UIEvent("ButtonClicked", [this]() {return changeTimeMode(!mode); }));
		interfaceSystem->registerEvent("+timeModeButtonClick", UIEvent("ButtonClicked", [this]() {return changeTimeMode(!mode); }));

		interfaceSystem->registerEvent("-timeButtonClick", UIEvent("ButtonClicked", [this]() {return changeTime(-CHANGE_TIME); }));
		interfaceSystem->registerEvent("+timeButtonClick", UIEvent("ButtonClicked", [this]() {return changeTime(+CHANGE_TIME); }));

		interfaceSystem->registerEvent("-levelButtonClick", UIEvent("ButtonClicked", [this]() {return changeLevel(-1); }));
		interfaceSystem->registerEvent("+levelButtonClick", UIEvent("ButtonClicked", [this]() {return changeLevel(+1); }));

		interfaceSystem->registerEvent("-songButtonClick", UIEvent("ButtonClicked", [this]() {return changeSong(-1); }));
		interfaceSystem->registerEvent("+songButtonClick", UIEvent("ButtonClicked", [this]() {return changeSong(+1); }));
		interfaceSystem->registerEvent("previewSongButtonClick", UIEvent("ButtonClicked", [this]() {return previewSong(!songPreview); }));

		interfaceSystem->registerEvent("startButtonClick", UIEvent("ButtonClicked", [this]() {return startButtonClick(); }));
		interfaceSystem->registerEvent("settingsButtonClick", UIEvent("ButtonClicked", [this]() {return settingsButtonClick(); }));
		interfaceSystem->registerEvent("backButtonClick", UIEvent("ButtonClicked", [this]() {return backButtonClick(); }));

		interfaceSystem->registerEvent("slot1ButtonClick", UIEvent("ButtonClicked", [this]() {return slotButtonClick(0, "Slot1"); }));
		interfaceSystem->registerEvent("slot2ButtonClick", UIEvent("ButtonClicked", [this]() {return slotButtonClick(1, "Slot2"); }));
		interfaceSystem->registerEvent("slot3ButtonClick", UIEvent("ButtonClicked", [this]() {return slotButtonClick(2, "Slot3"); }));
		interfaceSystem->registerEvent("slot4ButtonClick", UIEvent("ButtonClicked", [this]() {return slotButtonClick(3, "Slot4"); }));
	}

	timeModes = { "Time", "Infinite" };
}

ConfigurationMenu::~ConfigurationMenu()
{
	if (notNull(interfaceSystem)) {
		interfaceSystem->unregisterEvent("-healthButtonClick");
		interfaceSystem->unregisterEvent("+healthButtonClick");

		interfaceSystem->unregisterEvent("-timeModeButtonClick");
		interfaceSystem->unregisterEvent("+timeModeButtonClick");

		interfaceSystem->unregisterEvent("-timeButtonClick");
		interfaceSystem->unregisterEvent("+timeButtonClick");

		interfaceSystem->unregisterEvent("-levelButtonClick");
		interfaceSystem->unregisterEvent("+levelButtonClick");

		interfaceSystem->unregisterEvent("-songButtonClick");
		interfaceSystem->unregisterEvent("+songButtonClick");
		interfaceSystem->unregisterEvent("previewSongButtonClick");

		interfaceSystem->unregisterEvent("startButtonClick");
		interfaceSystem->unregisterEvent("settingsButtonClick");
		interfaceSystem->unregisterEvent("backButtonClick");

		interfaceSystem->unregisterEvent("slot1ButtonClick");
		interfaceSystem->unregisterEvent("slot2ButtonClick");
		interfaceSystem->unregisterEvent("slot3ButtonClick");
		interfaceSystem->unregisterEvent("slot4ButtonClick");
	}

	configurationLayout = nullptr;

	slots.clear();
	timeModes.clear();
	levelNames.clear();
	songNames.clear();
}

void ConfigurationMenu::start()
{
	Menu::start();

	if (notNull(mainCamera))
	{
		configurationLayout = mainCamera->getComponent<UILayout>();
		std::vector<GameObject*> aux = mainCamera->getChildren();
		if (aux.size() > 0 && notNull(aux[0])) {
			UILayout* settingsLayout = aux[0]->getComponent<UILayout>();
			if (notNull(settingsLayout))
				settingsPanel = settingsLayout->getRoot();
		}
	}

	if (notNull(configurationLayout))
		startButton = configurationLayout->getRoot().getChild("StartButton");

	slots = std::vector<std::pair<int, UIElement>>(4, { -1, NULL });
	std::vector<int> indexes = (notNull(gameManager)) ? gameManager->getPlayerIndexes() : std::vector<int>();

	for (int i = 0; i < 4; i++)
	{
		if (notNull(configurationLayout))
		{
			if (i < indexes.size()) {
				slots[i] = { indexes[i] , configurationLayout->getRoot().getChild("Slot" + std::to_string(i + 1)) };

				if (indexes[i] != -1)
				{
					if (indexes[i] == 9)
					{
						slots[i].second.getChild("Slot" + std::to_string(i + 1) + "Text").setText("IA");
						slots[i].second.getChild("Slot" + std::to_string(i + 1) + "Button").setText("Clear IA");
					}
					else
					{
						slots[i].second.getChild("Slot" + std::to_string(i + 1) + "Text").setText("Player " + std::to_string(i + 1));
						slots[i].second.getChild("Slot" + std::to_string(i + 1) + "Button").setVisible(false);
					}
				}
			}
		}
	}

	if (notNull(gameManager))
	{
		nPlayers = gameManager->getInitialPlayers();
		health = gameManager->getHealth() / 2;
		mode = gameManager->getTimeMode();
		time = gameManager->getInitialTime();
	}

	if (!startButton.isVisible() && nPlayers >= MIN_PLAYERS)
		startButton.setVisible(true);


	changeHealth(0);
	changeTimeMode(mode);
	changeTime(0);

	initNames();

	changeLevel(0);
	changeSong(0);
}

void ConfigurationMenu::update(float deltaTime)
{
	checkInput();

	if (songPreview && timer > 0)
		timer -= deltaTime;
	else if (songPreview)
		stopPreview();
}

void ConfigurationMenu::initNames()
{
	songNames["Bustin Loose"] = "bustinLoose";
	songNames["District Four"] = "districtFour";
	songNames["Drama"] = "tvDrama";
	songNames["Epic Battle"] = "epicBattle";
	songNames["Fight"] = "fightScene";
	songNames["Green Daze"] = "greenDaze";
	songNames["Opus One"] = "opusOne";
	songNames["Strength"] = "strengthOfTheTitans";
	songNames["UGP"] = "ugpTrack1";

	std::string song = (notNull(gameManager)) ? gameManager->getSong().second : "";
	auto it = songNames.find(song);
	if (it == songNames.end())
		currentSong = "Bustin Loose";
	else
		currentSong = it->first;

	if (notNull(configurationLayout))
		configurationLayout->getRoot().getChild("PreviewSongButton").setText(currentSong);


	levelNames["Coliseum"] = "level4";
	levelNames["Sewers"] = "level3";
	levelNames["Volcano"] = "level5";

	std::string level = (notNull(gameManager)) ? gameManager->getLevel().second : "";
	it = levelNames.find(level);
	if (it == levelNames.end())
		currentLevel = "Coliseum";
	else
		currentLevel = it->first;

	if (notNull(configurationLayout)) {
		configurationLayout->getRoot().getChild("Level").setText(currentLevel);
		configurationLayout->getRoot().getChild("LevelImage").setProperty("Image", levelNames[currentLevel]);
	}
}

void ConfigurationMenu::checkInput()
{
	checkNullAndBreak(inputSystem);

	int i = 0;
	bool pressed = false;
	while (i < 5 && !pressed)
	{
		// Fill or clear slots
		int slotIndex = isIndexConnected(i);
		bool enterButton = (i < 4 && inputSystem->getButtonPress(i, "X")) || (i == 4 && inputSystem->getKeyPress("SPACE"));
		bool exitButton = (i < 4 && (inputSystem->getButtonPress(i, "X") || !inputSystem->isControllerConnected(i))) || (i == 4 && inputSystem->getKeyPress("SPACE"));

		if (nPlayers < 4 && slotIndex == -1 && enterButton)
			fillSlot(i);
		else if (slotIndex != -1 && exitButton)
			clearSlot(slotIndex);

		// Fill or clear with IA
		int enterIASlot = isIndexConnected(-1);
		bool enterIAButton = (i < 4 && inputSystem->getButtonPress(i, "RB"));

		int exitIASlot = isIndexConnected(9);
		bool exitIAButton = (i < 4 && inputSystem->getButtonPress(i, "LB"));

		if (nPlayers < 4 && enterIASlot != -1 && enterIAButton)
			slotButtonClick(enterIASlot, "Slot" + std::to_string(enterIASlot + 1));
		else if (exitIASlot != -1 && exitIAButton)
			slotButtonClick(exitIASlot, "Slot" + std::to_string(exitIASlot + 1));

		// Close Settings Panel or back to Main Menu
		bool escape = i == 4 && inputSystem->getKeyPress("ESCAPE");
		bool b = i < 4 && inputSystem->getButtonPress(i, "B");

		if (escape || b)
		{
			pressed = true;

			if (settingsPanel.isVisible())
				settingsButtonClick();
			else
				backButtonClick();
		}

		// Enter or Start to initiate the game
		bool enter = i == 4 && inputSystem->getKeyPress("RETURN");
		bool start = i < 4 && inputSystem->getButtonPress(i, "START");

		if ((enter || start) && startButton.isVisible())
		{
			pressed = true;
			startButtonClick();
		}

		i++;
	}
}

void ConfigurationMenu::fillSlot(int index)
{
	int i = 0;
	bool find = false;

	while (i < slots.size() && !find)
	{
		if (slots[i].first == -1)
		{
			find = true;

			slots[i].first = index;
			slots[i].second.getChild("Slot" + std::to_string(i + 1) + "Text").setText("Player " + std::to_string(i + 1));
			slots[i].second.getChild("Slot" + std::to_string(i + 1) + "Button").setVisible(false);
		}
		i++;
	}

	nPlayers++;

	if (!startButton.isVisible() && nPlayers >= MIN_PLAYERS)
		startButton.setVisible(true);

	buttonClick(buttonSound);
}

void ConfigurationMenu::clearSlot(int index)
{
	slots[index].first = -1;
	slots[index].second.getChild("Slot" + std::to_string(index + 1) + "Text").setText("Press SPACE or X");
	slots[index].second.getChild("Slot" + std::to_string(index + 1) + "Button").setVisible(true);

	nPlayers--;

	if (startButton.isVisible() && nPlayers < MIN_PLAYERS)
		startButton.setVisible(false);

	buttonClick(buttonSound);
}

int ConfigurationMenu::isIndexConnected(int index)
{
	int i = 0;

	if (index == 9)
	{
		i = slots.size() - 1;
		while (i >= 0 && slots[i].first != index) i--;
	}
	else
	{
		i = 0;
		while (i < slots.size() && slots[i].first != index) i++;
	}

	if (i < slots.size() && slots[i].first == index)
		return i;

	return -1;
}

bool ConfigurationMenu::changeHealth(int value)
{
	health += value;

	if (health < MIN_HEALTH) health = MIN_HEALTH;
	if (health > MAX_HEALTH) health = MAX_HEALTH;

	settingsPanel.getChild("SettingsBackground").getChild("Health").setText(std::to_string(health));

	buttonClick(buttonSound);

	return false;
}

bool ConfigurationMenu::changeTimeMode(int value)
{
	mode = value;
	settingsPanel.getChild("SettingsBackground").getChild("TimeMode").setText(timeModes[mode]);

	settingsPanel.getChild("SettingsBackground").getChild("Time").setVisible(!mode);
	settingsPanel.getChild("SettingsBackground").getChild("-TimeButton").setVisible(!mode);
	settingsPanel.getChild("SettingsBackground").getChild("+TimeButton").setVisible(!mode);

	buttonClick(buttonSound);

	return false;
}

bool ConfigurationMenu::changeTime(int value)
{
	time += value;

	if (time < MIN_TIME) time = MIN_TIME;
	if (time > MAX_TIME) time = MAX_TIME;

	settingsPanel.getChild("SettingsBackground").getChild("Time").setText(std::to_string(time));

	buttonClick(buttonSound);

	return false;
}

bool ConfigurationMenu::changeLevel(int value)
{
	auto it = levelNames.find(currentLevel);
	if (it == levelNames.end())
		return false;

	if (value == -1 && it != levelNames.begin())
		it--;
	else if (value == -1)
		it = prev(levelNames.end());

	if (value == 1 && it != prev(levelNames.end()))
		it++;
	else if (value == 1)
		it = levelNames.begin();

	currentLevel = (*it).first;
	if (notNull(configurationLayout)) {
		configurationLayout->getRoot().getChild("Level").setText(currentLevel);
		configurationLayout->getRoot().getChild("LevelImage").setProperty("Image", levelNames[currentLevel]);
	}

	buttonClick(buttonSound);

	return false;
}

bool ConfigurationMenu::changeSong(int value)
{
	stopPreview();

	auto it = songNames.find(currentSong);
	if (it == songNames.end())
		return false;

	if (value == -1 && it != songNames.begin())
		it--;
	else if (value == -1)
		it = prev(songNames.end());

	if (value == 1 && it != prev(songNames.end()))
		it++;
	else if (value == 1)
		it = songNames.begin();

	currentSong = (*it).first;
	if (notNull(configurationLayout))
		configurationLayout->getRoot().getChild("PreviewSongButton").setText(currentSong);

	buttonClick(buttonSound);

	return false;
}

bool ConfigurationMenu::previewSong(bool value)
{
	if (value && notNull(songManager))
	{
		songManager->pauseMenuSong();
		songManager->playSong(songNames[currentSong]);
		songPreview = true;
		timer = previewTime;
	}
	else
		stopPreview();

	return false;
}

void ConfigurationMenu::stopPreview()
{
	if (songPreview && notNull(songManager))
	{
		songManager->stopSong(songNames[currentSong]);
		songManager->resumeMenuSong();
		songPreview = false;
	}
}

bool ConfigurationMenu::slotButtonClick(int index, std::string name)
{
	if (slots[index].first == -1)
	{
		slots[index].first = 9;
		slots[index].second.getChild(name + "Text").setText("IA");
		slots[index].second.getChild(name + "Button").setText("Clear IA");

		nPlayers++;

		if (!startButton.isVisible() && nPlayers >= MIN_PLAYERS)
			startButton.setVisible(true);
	}
	else
	{
		slots[index].first = -1;
		slots[index].second.getChild(name + "Text").setText("Press SPACE or X");
		slots[index].second.getChild(name + "Button").setText("Insert IA");

		nPlayers--;

		if (startButton.isVisible() && nPlayers < MIN_PLAYERS)
			startButton.setVisible(false);
	}

	buttonClick(buttonSound);

	return false;
}

bool ConfigurationMenu::startButtonClick()
{
	checkNullAndBreak(songManager, false);
	checkNullAndBreak(gameManager, false);
	checkNullAndBreak(sceneManager, false);

	buttonClick("fight");

	stopPreview();
	songManager->stopMenuSong();

	gameManager->setInitialPlayers(nPlayers);

	std::vector<int> indexes;
	std::vector<int> aux;

	for (int i = 0; i < 4; i++)
	{
		indexes.push_back(slots[i].first);

		if (slots[i].first != -1)
			aux.push_back(i + 1);
	}

	gameManager->getScore()->setPlayerID(aux);

	gameManager->setPlayerIndexes(indexes);

	gameManager->setHealth(health * 2);
	gameManager->setTimeMode(mode);

	gameManager->setLevel(levelNames[currentLevel], currentLevel);
	gameManager->setSong(songNames[currentSong], currentSong);

	if (mode)
		gameManager->setTime(-1);
	else
		gameManager->setTime(time);

	sceneManager->changeScene("Game", true);
	return false;
}

bool ConfigurationMenu::settingsButtonClick()
{
	if (!settingsPanel.isVisible())
	{
		settingsPanel.setVisible(true);
		settingsPanel.setAlwaysOnTop(true);

		if (notNull(interfaceSystem)) {
			interfaceSystem->clearControllerMenuInput();
			interfaceSystem->initControllerMenuInput(&settingsPanel);
		}
	}
	else
	{
		settingsPanel.setVisible(false);
		settingsPanel.setAlwaysOnTop(false);

		if (notNull(interfaceSystem))
			interfaceSystem->clearControllerMenuInput();
	}

	return false;
}

bool ConfigurationMenu::backButtonClick()
{
	stopPreview();
	return Menu::backButtonClick();
}