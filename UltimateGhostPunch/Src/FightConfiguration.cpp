#include "FightConfiguration.h"

#include "InputSystem.h"
#include "GameObject.h"
#include "UILayout.h"
#include "UIElement.h"

#include <SceneManager.h>
#include <InterfaceSystem.h>
#include "GameManager.h"

#include "ComponentRegister.h"

REGISTER_FACTORY(FightConfiguration);

// EVENTS----

bool FightConfiguration::fightButtonClick()
{
	// set data
	GameManager* gameManager = GameManager::GetInstance();
	gameManager->setLevel(levelNames[levelIndex]);
	gameManager->setSong(songNames[songIndex]);
	gameManager->setNumPlayers(numPlayers);
	gameManager->setHealth(health);

	std::vector<int> indexes;
	for (int i = 0; i < numPlayers; i++)
		indexes.push_back(slots[i].first);
	gameManager->setPlayerIndexes(indexes);

	if (time != MAX_TIME) gameManager->setTime(time);
	else gameManager->setTime(-1);

	// change scene
	SceneManager::GetInstance()->changeScene("mainScene");

	return false;
}

bool FightConfiguration::changeHealth(int value)
{
	health += value;

	if (health < MIN_HEALTH) health = MIN_HEALTH;
	if (health > MAX_HEALTH) health = MAX_HEALTH;

	configLayout->getRoot().getChild("Health").setText(std::to_string(health));

	return false;
}

bool FightConfiguration::changeTime(int value)
{
	time += value;

	if (time < MIN_TIME) time = MIN_TIME;
	if (time > MAX_TIME) time = MAX_TIME;

	if (time == MAX_TIME) configLayout->getRoot().getChild("Time").setText("INFINITE");
	else configLayout->getRoot().getChild("Time").setText(std::to_string(time));

	return false;
}

bool FightConfiguration::changeSong(int value)
{
	songIndex += value;

	if (songIndex < 0) songIndex = 0;
	if (songIndex > songNames.size() - 1) songIndex = songNames.size() - 1;

	configLayout->getRoot().getChild("Song").setText(songNames[songIndex]);

	return false;
}

bool FightConfiguration::changeLevel(int value)
{
	levelIndex += value;

	if (levelIndex < 0) levelIndex = 0;
	if (levelIndex > levelNames.size() - 1) levelIndex = levelNames.size() - 1;

	configLayout->getRoot().getChild("Level").setText(levelNames[levelIndex]);

	return false;
}

// -----

FightConfiguration::FightConfiguration(GameObject* gameObject) :	UserComponent(gameObject), inputSystem(nullptr), configLayout(nullptr), fightButton(NULL),
																	numPlayers(0), health(0), time(0), levelIndex(0), songIndex(0)
{
	InterfaceSystem* interfaceSystem = InterfaceSystem::GetInstance();
	interfaceSystem->registerEvent("-healthButtonClick", UIEvent("ButtonClicked", [this]() {return changeHealth(-1); }));
	interfaceSystem->registerEvent("+healthButtonClick", UIEvent("ButtonClicked", [this]() {return changeHealth(+1); }));

	interfaceSystem->registerEvent("-timeButtonClick", UIEvent("ButtonClicked", [this]() {return changeTime(-20); }));
	interfaceSystem->registerEvent("+timeButtonClick", UIEvent("ButtonClicked", [this]() {return changeTime(+20); }));

	interfaceSystem->registerEvent("-songButtonClick", UIEvent("ButtonClicked", [this]() {return changeSong(-1); }));
	interfaceSystem->registerEvent("+songButtonClick", UIEvent("ButtonClicked", [this]() {return changeSong(+1); }));

	interfaceSystem->registerEvent("-levelButtonClick", UIEvent("ButtonClicked", [this]() {return changeLevel(-1); }));
	interfaceSystem->registerEvent("+levelButtonClick", UIEvent("ButtonClicked", [this]() {return changeLevel(+1); }));

	interfaceSystem->registerEvent("fightButtonClick", UIEvent("ButtonClicked", [this]() {return fightButtonClick(); }));
}

FightConfiguration::~FightConfiguration()
{
	InterfaceSystem* interfaceSystem = InterfaceSystem::GetInstance();
	interfaceSystem->unregisterEvent("-healthButtonClick");
	interfaceSystem->unregisterEvent("+healthButtonClick");

	interfaceSystem->unregisterEvent("-timeButtonClick");
	interfaceSystem->unregisterEvent("+timeButtonClick");

	interfaceSystem->unregisterEvent("-songButtonClick");
	interfaceSystem->unregisterEvent("+songButtonClick");

	interfaceSystem->unregisterEvent("-levelButtonClick");
	interfaceSystem->unregisterEvent("+levelButtonClick");

	interfaceSystem->unregisterEvent("fightButtonClick");
}

void FightConfiguration::start()
{
	inputSystem = InputSystem::GetInstance();

	GameObject* mainCamera = findGameObjectWithName("MainCamera");
	if (mainCamera != nullptr)
		configLayout = mainCamera->getComponent<UILayout>();
	if(configLayout != nullptr)
		fightButton = configLayout->getRoot().getChild("FightButton");

	levelIndex = 0;
	songIndex = 0;
	time = 60;
	health = 4;
	numPlayers = 0;

	slots = std::vector<std::pair<int, UIElement>>(4, { -1, NULL });
	for (int i = 0; i < 4; i++)	{
		if (configLayout != nullptr)
			slots[i] = { -1 , configLayout->getRoot().getChild("Slot" + std::to_string(i + 1)).getChild("Connected" + std::to_string(i + 1)) };
	}

}

void FightConfiguration::update(float deltaTime)
{
	checkInput();
}

void FightConfiguration::checkInput()
{
	for (int i = 0; i < 5; i++)	{
		int slotIndex = isIndexConnected(i);
		// Controller || Keyboard
		bool enterButton =		(i < 4 && inputSystem->getButtonPress(i, "A"))	// Controller
							||	(i == 4 && inputSystem->getKeyPress("Space"));	// Keyboard
		bool exitButton =		(i < 4 && (inputSystem->getButtonPress(i, "B") || !inputSystem->isControllerConnected(i)))	// Controller
							||	(i == 4 && inputSystem->getKeyPress("ESCAPE"));												// Keyboard

		if (numPlayers < 4 && slotIndex == -1 && enterButton)
			fillSlot(numPlayers, i);
		else if (slotIndex != -1 && exitButton)	{
			clearSlot(slotIndex);
			reorderSlots(slotIndex);
		}
	}
}

void FightConfiguration::fillSlot(int slotIndex, int deviceIndex)
{
	slots[slotIndex].first = deviceIndex;

	slots[slotIndex].second.setVisible(true);
	slots[slotIndex].second.getChild("PlayerText").setText("P" + std::to_string(slotIndex + 1));
	slots[slotIndex].second.getChild("IndexText").setText("Index: " + std::to_string(slots[slotIndex].first));

	if (deviceIndex == 4) slots[slotIndex].second.getChild("TypeText").setText("Keyboard");
	else slots[slotIndex].second.getChild("TypeText").setText("Controller");

	numPlayers++;

	if (!fightButton.isVisible() && numPlayers >= MIN_PLAYERS) fightButton.setVisible(true);
}

void FightConfiguration::clearSlot(int index)
{
	slots[index].first = -1;
	slots[index].second.setVisible(false);

	numPlayers--;

	if (fightButton.isVisible() && numPlayers < MIN_PLAYERS) fightButton.setVisible(false);
}

int FightConfiguration::isIndexConnected(int index)
{
	int i = 0;
	while (i < numPlayers && slots[i].first != index)
		i++;

	if (slots[i].first == index)
		return i;

	return -1;
}

void FightConfiguration::reorderSlots(int index)
{
	for (int i = index; i < numPlayers; i++)
	{
		fillSlot(i, slots[i + 1].first);
		clearSlot(i + 1);
	}
}