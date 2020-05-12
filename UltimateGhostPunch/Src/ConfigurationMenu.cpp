#include "ConfigurationMenu.h"
#include <ComponentRegister.h>
#include <InterfaceSystem.h>
#include <InputSystem.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <UILayout.h>
#include <UIElement.h>

#include "GameManager.h"

REGISTER_FACTORY(ConfigurationMenu);

ConfigurationMenu::ConfigurationMenu(GameObject* gameObject) : UserComponent(gameObject), inputSystem(nullptr), configurationLayout(nullptr), startButton(NULL), settingsPanel(NULL),
nPlayers(0), health(4), time(60), mode(false), levelIndex(0), songIndex(0)
{
	InterfaceSystem* interfaceSystem = InterfaceSystem::GetInstance();

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

	interfaceSystem->registerEvent("startButtonClick", UIEvent("ButtonClicked", [this]() {return startButtonClick(); }));
	interfaceSystem->registerEvent("settingsButtonClick", UIEvent("ButtonClicked", [this]() {return settingsButtonClick(); }));
	interfaceSystem->registerEvent("backButtonClick", UIEvent("ButtonClicked", [this]() {return backButtonClick(); }));

	timeModes = { "Time", "Infinite" };
	levelNames = { "level3", "level4", "level5" };
	songNames = { "Canción 1", "Canción 2", "Canción 3" };
}

ConfigurationMenu::~ConfigurationMenu()
{
	InterfaceSystem* interfaceSystem = InterfaceSystem::GetInstance();
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

	interfaceSystem->unregisterEvent("startButtonClick");
	interfaceSystem->unregisterEvent("settingsButtonClick");
	interfaceSystem->unregisterEvent("backButtonClick");
}

void ConfigurationMenu::start()
{
	inputSystem = InputSystem::GetInstance();

	GameManager* gameManager = GameManager::GetInstance();
	GameObject* mainCamera = findGameObjectWithName("MainCamera");

	if (mainCamera != nullptr)
	{
		configurationLayout = mainCamera->getComponent<UILayout>();
		settingsPanel = mainCamera->getChildren()[0]->getComponent<UILayout>()->getRoot();
	}

	if (configurationLayout != nullptr)
		startButton = configurationLayout->getRoot().getChild("StartButton");

	slots = std::vector<std::pair<int, UIElement>>(4, { -1, NULL });
	std::vector<int> indexes = gameManager->getPlayerIndexes();

	for (int i = 0; i < 4; i++)
	{
		if (configurationLayout != nullptr)
		{
			slots[i] = { indexes[i] , configurationLayout->getRoot().getChild("Slot" + std::to_string(i + 1)) };

			if (indexes[i] != -1)
				slots[i].second.getChild("Slot" + std::to_string(i + 1) + "Text").setText("Player " + std::to_string(i + 1));
		}
	}

	nPlayers = gameManager->getInitialPlayers();

	if (!startButton.isVisible() && nPlayers >= MIN_PLAYERS)
		startButton.setVisible(true);

	health = gameManager->getHealth();
	mode = gameManager->getTimeMode();
	time = gameManager->getInitialTime();

	changeHealth(0);
	changeTimeMode(mode);
	changeTime(0);

	levelIndex = gameManager->getLevel();
	songIndex = gameManager->getSong();

	changeLevel(0);
	changeSong(0);
}

void ConfigurationMenu::update(float deltaTime)
{
	checkInput();
}

void ConfigurationMenu::checkInput()
{
	bool pressed = false;

	int i = 0;
	while(i < 5 && !pressed)
	{
		// Clear or reorder Slots
		int slotIndex = isIndexConnected(i);
		bool enterButton = isIndexConnected(i) == -1 && (i < 4 && inputSystem->getButtonPress(i, "X")) || (i == 4 && inputSystem->getKeyPress("SPACE"));
		bool exitButton = isIndexConnected(i) != -1 && (i < 4 && (inputSystem->getButtonPress(i, "X") || !inputSystem->isControllerConnected(i))) || (i == 4 && inputSystem->getKeyPress("SPACE"));

		if (nPlayers < 4 && slotIndex == -1 && enterButton)
			fillSlot(nPlayers, i);
		else if (slotIndex != -1 && exitButton)
		{
			clearSlot(slotIndex);
			reorderSlots(slotIndex);
		}

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

void ConfigurationMenu::fillSlot(int slotIndex, int deviceIndex)
{
	slots[slotIndex].first = deviceIndex;
	slots[slotIndex].second.getChild("Slot" + std::to_string(slotIndex + 1) + "Text").setText("Player " + std::to_string(slotIndex + 1));

	nPlayers++;

	if (!startButton.isVisible() && nPlayers >= MIN_PLAYERS)
		startButton.setVisible(true);
}

void ConfigurationMenu::clearSlot(int index)
{
	slots[index].first = -1;
	slots[index].second.getChild("Slot" + std::to_string(index + 1) + "Text").setText("Press SPACE or X");

	nPlayers--;

	if (startButton.isVisible() && nPlayers < MIN_PLAYERS)
		startButton.setVisible(false);
}

void ConfigurationMenu::reorderSlots(int index)
{
	for (int i = index; i < nPlayers; i++)
	{
		fillSlot(i, slots[i + 1].first);
		clearSlot(i + 1);
	}
}

int ConfigurationMenu::isIndexConnected(int index)
{
	int i = 0;
	while (i < nPlayers - 1 && slots[i].first != index) i++;

	if (slots[i].first == index)
		return i;

	return -1;
}

bool ConfigurationMenu::changeHealth(int value)
{
	health += value;

	if (health < MIN_HEALTH) health = MIN_HEALTH;
	if (health > MAX_HEALTH) health = MAX_HEALTH;

	settingsPanel.getChild("SettingsBackground").getChild("Health").setText(std::to_string(health));

	return false;
}

bool ConfigurationMenu::changeTimeMode(int value)
{
	mode = value;
	settingsPanel.getChild("SettingsBackground").getChild("TimeMode").setText(timeModes[mode]);

	return false;
}

bool ConfigurationMenu::changeTime(int value)
{
	time += value;

	if (time < MIN_TIME) time = MIN_TIME;
	if (time > MAX_TIME) time = MAX_TIME;

	settingsPanel.getChild("SettingsBackground").getChild("Time").setText(std::to_string(time));

	return false;
}

bool ConfigurationMenu::changeLevel(int value)
{
	levelIndex += value;

	int index = levelIndex % levelNames.size();
	configurationLayout->getRoot().getChild("Level").setText(levelNames[index]);
	configurationLayout->getRoot().getChild("LevelImage").setProperty("Image", levelNames[index]);

	GameManager::GetInstance()->setLevel(index);
	GameManager::GetInstance()->setLevelName(levelNames[index]);

	return false;
}

bool ConfigurationMenu::changeSong(int value)
{
	songIndex += value;

	int index = songIndex % songNames.size();
	configurationLayout->getRoot().getChild("Song").setText(songNames[index]);

	GameManager::GetInstance()->setSong(index);
	GameManager::GetInstance()->setSongName(songNames[index]);

	return false;
}

bool ConfigurationMenu::startButtonClick()
{
	GameManager* gameManager = GameManager::GetInstance();

	gameManager->setPlayersAlive(nPlayers);
	gameManager->initPlayerRanking(nPlayers);

	std::vector<int> indexes;
	for (int i = 0; i < 4; i++)
		indexes.push_back(slots[i].first);

	gameManager->setPlayerIndexes(indexes);

	gameManager->setHealth(health);
	gameManager->setTimeMode(mode);

	if (mode)
		gameManager->setTime(-1);
	else
		gameManager->setTime(time);

	SceneManager::GetInstance()->changeScene("Game", true);
	return false;
}

bool ConfigurationMenu::settingsButtonClick()
{
	if (!settingsPanel.isVisible())
	{
		settingsPanel.setVisible(true);
		settingsPanel.setAlwaysOnTop(true);

		InterfaceSystem::GetInstance()->clearControllerMenuInput();
		InterfaceSystem::GetInstance()->initControllerMenuInput(&settingsPanel);
	}
	else
	{
		settingsPanel.setVisible(false);
		settingsPanel.setAlwaysOnTop(false);

		InterfaceSystem::GetInstance()->clearControllerMenuInput();
	}

	return false;
}

bool ConfigurationMenu::backButtonClick()
{
	SceneManager::GetInstance()->changeScene("MainMenu");
	return false;
}