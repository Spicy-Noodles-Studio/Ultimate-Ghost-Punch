#include "FightConfiguration.h"

#include "InputSystem.h"
#include "GameObject.h"
#include "UILayout.h"
#include "UIElement.h"

#include <SceneManager.h>
#include <InterfaceSystem.h>
#include "GameManager.h"

// EVENTS----

bool FightConfiguration::fightButtonClick()
{
	// set data
	GameManager::GetInstance()->setLevel(level);
	GameManager::GetInstance()->setSong(song);
	GameManager::GetInstance()->setNPlayers(nPlayers);

	// change scene
	SceneManager::GetInstance()->changeScene("mainScene");

	return false;
}

// -----

FightConfiguration::FightConfiguration(GameObject* gameObject) : UserComponent(gameObject)
{
	InterfaceSystem::GetInstance()->registerEvent("fightButtonClick", UIEvent("ButtonClicked", [this]() {return fightButtonClick(); }));
}

FightConfiguration::~FightConfiguration()
{

}

void FightConfiguration::start()
{
	inputSystem = InputSystem::GetInstance();

	nPlayers = 0;

	slots = std::vector<std::pair<int, UIElement>>(4, { -1,NULL });
	for (int i = 0; i < 4; i++)
	{
		slots[i] = { -1 , findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("Slot" + std::to_string(i + 1))
			.getChild("Connected" + std::to_string(i + 1)) };
	}

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
}

void FightConfiguration::clearSlot(int index)
{
	slots[index].first = -1;
	slots[index].second.setVisible(false);

	nPlayers--;
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

void FightConfiguration::update(float deltaTime)
{
	checkController();
	checkKeyboard();
}
