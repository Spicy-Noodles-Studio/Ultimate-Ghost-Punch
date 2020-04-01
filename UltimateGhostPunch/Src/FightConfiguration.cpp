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
	GameManager::GetInstance()->setLevel(levelNames[level]);
	GameManager::GetInstance()->setSong(songNames[song]);
	GameManager::GetInstance()->setNPlayers(nPlayers);
	GameManager::GetInstance()->setHealth(health);

	std::vector<int> indexes;
	for (int i = 0; i < nPlayers; i++)
		indexes.push_back(slots[i].first);
	GameManager::GetInstance()->setPlayerIndexes(indexes);

	if (time != MAX_TIME) GameManager::GetInstance()->setTime(time);
	else GameManager::GetInstance()->setTime(-1);

	// change scene
	SceneManager::GetInstance()->changeScene("mainScene");

	return false;
}

bool FightConfiguration::changeHealth(int value)
{
	health += value;

	if (health < MIN_HEALTH) health = MIN_HEALTH;
	if (health > MAX_HEALTH) health = MAX_HEALTH;

	findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("Health").setText(std::to_string(health));

	return false;
}

bool FightConfiguration::changeTime(int value)
{
	time += value;

	if (time < MIN_TIME) time = MIN_TIME;
	if (time > MAX_TIME) time = MAX_TIME;

	if (time == MAX_TIME) findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("Time").setText("INFINITE");
	else findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("Time").setText(std::to_string(time));

	return false;
}

bool FightConfiguration::changeSong(int value)
{
	song += value;

	if (song < 0) song = 0;
	if (song > songNames.size() - 1) song = songNames.size() - 1;

	findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("Song").setText(songNames[song]);

	return false;
}

bool FightConfiguration::changeLevel(int value)
{
	level += value;

	if (level < 0) level = 0;
	if (level > levelNames.size() - 1) level = levelNames.size() - 1;

	findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("Level").setText(levelNames[level]);

	return false;
}

// -----

FightConfiguration::FightConfiguration(GameObject* gameObject) :
	UserComponent(gameObject), fightButton(NULL)
{
	InterfaceSystem::GetInstance()->registerEvent("-healthButtonClick", UIEvent("ButtonClicked", [this]() {return changeHealth(-1); }));
	InterfaceSystem::GetInstance()->registerEvent("+healthButtonClick", UIEvent("ButtonClicked", [this]() {return changeHealth(+1); }));

	InterfaceSystem::GetInstance()->registerEvent("-timeButtonClick", UIEvent("ButtonClicked", [this]() {return changeTime(-20); }));
	InterfaceSystem::GetInstance()->registerEvent("+timeButtonClick", UIEvent("ButtonClicked", [this]() {return changeTime(+20); }));

	InterfaceSystem::GetInstance()->registerEvent("-songButtonClick", UIEvent("ButtonClicked", [this]() {return changeSong(-1); }));
	InterfaceSystem::GetInstance()->registerEvent("+songButtonClick", UIEvent("ButtonClicked", [this]() {return changeSong(+1); }));

	InterfaceSystem::GetInstance()->registerEvent("-levelButtonClick", UIEvent("ButtonClicked", [this]() {return changeLevel(-1); }));
	InterfaceSystem::GetInstance()->registerEvent("+levelButtonClick", UIEvent("ButtonClicked", [this]() {return changeLevel(+1); }));

	InterfaceSystem::GetInstance()->registerEvent("fightButtonClick", UIEvent("ButtonClicked", [this]() {return fightButtonClick(); }));
}

FightConfiguration::~FightConfiguration()
{
	InterfaceSystem::GetInstance()->unregisterEvent("-healthButtonClick");
	InterfaceSystem::GetInstance()->unregisterEvent("+healthButtonClick");

	InterfaceSystem::GetInstance()->unregisterEvent("-timeButtonClick");
	InterfaceSystem::GetInstance()->unregisterEvent("+timeButtonClick");

	InterfaceSystem::GetInstance()->unregisterEvent("-songButtonClick");
	InterfaceSystem::GetInstance()->unregisterEvent("+songButtonClick");

	InterfaceSystem::GetInstance()->unregisterEvent("-levelButtonClick");
	InterfaceSystem::GetInstance()->unregisterEvent("+levelButtonClick");

	InterfaceSystem::GetInstance()->unregisterEvent("fightButtonClick");
}

void FightConfiguration::start()
{
	inputSystem = InputSystem::GetInstance();

	fightButton = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("FightButton");

	level = 0;
	song = 0;
	time = 60;
	health = 4;
	nPlayers = 0;

	slots = std::vector<std::pair<int, UIElement>>(4, { -1, NULL });
	for (int i = 0; i < 4; i++)
	{
		slots[i] = { -1 , findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("Slot" + std::to_string(i + 1))
			.getChild("Connected" + std::to_string(i + 1)) };
	}

}

void FightConfiguration::update(float deltaTime)
{
	checkController();
	checkKeyboard();
}

void FightConfiguration::checkController()
{
	for (int i = 0; i < 4; i++)
	{
		int slotIndex = isIndexConnected(i);

		if (nPlayers < 4 && slotIndex == -1 && inputSystem->getButtonPress(i, "A"))
			fillSlot(nPlayers, i);
		else if (slotIndex != -1 && (inputSystem->getButtonPress(i, "B") || !inputSystem->isControllerConnected(i)))
		{
			clearSlot(slotIndex);
			reorderSlots(slotIndex);
		}
	}
}

void FightConfiguration::checkKeyboard()
{
	int slotIndex = isIndexConnected(5);

	if (nPlayers < 4 && slotIndex == -1 && inputSystem->getKeyPress("SPACE"))
		fillSlot(nPlayers, 5);
	else if (slotIndex != -1 && inputSystem->getKeyPress("ESCAPE"))
	{
		clearSlot(slotIndex);
		reorderSlots(slotIndex);
	}
}

void FightConfiguration::fillSlot(int slotIndex, int deviceIndex)
{
	slots[slotIndex].first = deviceIndex;

	slots[slotIndex].second.setVisible(true);
	slots[slotIndex].second.getChild("PlayerText").setText("P" + std::to_string(slotIndex + 1));
	slots[slotIndex].second.getChild("IndexText").setText("Index: " + std::to_string(slots[slotIndex].first));

	if (deviceIndex == 5) slots[slotIndex].second.getChild("TypeText").setText("Keyboard");
	else slots[slotIndex].second.getChild("TypeText").setText("Controller");

	nPlayers++;

	if (!fightButton.isVisible() && nPlayers >= MIN_PLAYERS) fightButton.setVisible(true);
}

void FightConfiguration::clearSlot(int index)
{
	slots[index].first = -1;
	slots[index].second.setVisible(false);

	nPlayers--;

	if (fightButton.isVisible() && nPlayers < MIN_PLAYERS) fightButton.setVisible(false);
}

int FightConfiguration::isIndexConnected(int index)
{
	int i = 0;
	while (i < nPlayers && slots[i].first != index)
		i++;

	if (slots[i].first == index)
		return i;

	return -1;
}

void FightConfiguration::reorderSlots(int index)
{
	for (int i = index; i < nPlayers; i++)
	{
		fillSlot(i, slots[i + 1].first);
		clearSlot(i + 1);
	}
}