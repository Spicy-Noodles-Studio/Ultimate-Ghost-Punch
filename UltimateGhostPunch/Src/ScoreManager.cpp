
#include "ScoreManager.h"

#include <ComponentRegister.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <UILayout.h>
#include <UIElement.h>
#include <InputSystem.h>
#include <Camera.h>
#include <InterfaceSystem.h>
#include "GameManager.h"


REGISTER_FACTORY(ScoreManager);



bool ScoreManager::resetButtonClick()
{
	manager->setNumPlayers(manager->getPlayerIndexes().size());

	manager->setTime(manager->getInitialTime());
	manager->setLevel(manager->getLastLevel());
	manager->setSong(manager->getLastSong());

	// change scene
	SceneManager::GetInstance()->changeScene("mainScene");
	return false;
}

ScoreManager::ScoreManager(GameObject* gameObject) : UserComponent(gameObject), player1Text(NULL), player2Text(NULL), player3Text(NULL), player4Text(NULL),
													player1Panel(NULL), player2Panel(NULL), player3Panel(NULL), player4Panel(NULL)
{
	InterfaceSystem* interfaceSystem = InterfaceSystem::GetInstance();
	interfaceSystem->registerEvent("resetButtonClick", UIEvent("ButtonClicked", [this]() {return resetButtonClick(); }));
}

ScoreManager::~ScoreManager()
{
	InterfaceSystem* interfaceSystem = InterfaceSystem::GetInstance();
	interfaceSystem->unregisterEvent("resetButtonClick");
}

void ScoreManager::start()
{
	
	player1Text = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("P1");
	player1Text.setVisible(false);
	player2Text = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("P2");
	player2Text.setVisible(false);
	player3Text = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("P3");
	player3Text.setVisible(false);
	player4Text = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("P4");
	player4Text.setVisible(false);


	player1Panel = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("P1Background");
	player1Panel.setVisible(false);
	player2Panel = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("P2Background");
	player2Panel.setVisible(false);
	player3Panel = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("P3Background");
	player3Panel.setVisible(false);
	player4Panel = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("P4Background");
	player4Panel.setVisible(false);
	
	manager = GameManager::GetInstance();

	reposition(manager->getPlayerIndexes().size());
	initStatistics(manager->getPlayerIndexes().size());

}

void ScoreManager::initStatistics(int numOfPlayers)
{
	Score * score = manager->getScore();
	for (int i = 0; i < numOfPlayers; i++)
	{
		setTotalKills(i + 1);
		setIndexOfPlayersKilled(i + 1);
		setPercentOfHits(i + 1);
		setSuccessfullGrabs(i + 1);
		setTotalAttacks(i + 1);
		setAmountOfDamageDealt(i + 1);
		setLifesAsGhost(i + 1);
		setTimesHittedBySpikes(i + 1);
		setAmountOfFalls(i + 1);
		setTotalDamageSuffer(i + 1);
		setEnviromentDeaths(i + 1);
	}
}

void ScoreManager::reposition(int numOfPlayers)
{
	float size=(1-0.2)/numOfPlayers;
	float iTextPos = 0.15;
	float iPanelPos = 0.1;

	for (int i = 0; i < numOfPlayers; i++)
	{
		float textPos = iTextPos + size * i;
		float panelPos = iPanelPos + size * i;
		if (i == 0)
		{
			player1Text.setVisible(true);
			player1Text.setPosition(textPos, 0.1);
			player1Panel.setVisible(true);
			player1Panel.setPosition(panelPos, 0.2);
			player1Panel.setSize(size, 0.6);
		}
		if (i == 1)
		{
			player2Text.setVisible(true);
			player2Text.setPosition(textPos, 0.1);
			player2Panel.setVisible(true);
			player2Panel.setPosition(panelPos, 0.2);
			player2Panel.setSize(size, 0.6);
		}
		if (i == 2)
		{
			player3Text.setVisible(true);
			player3Text.setPosition(textPos, 0.1);
			player3Panel.setVisible(true);
			player3Panel.setPosition(panelPos, 0.2);
			player3Panel.setSize(size, 0.6);
		}
		if (i == 3)
		{
			player4Text.setVisible(true);
			player4Text.setPosition(textPos, 0.1);
			player4Panel.setVisible(true);
			player4Panel.setPosition(panelPos, 0.2);
			player4Panel.setSize(size, 0.6);
		}
	}
}

void ScoreManager::setTotalKills(int playerIndex)
{
	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "TotalKills";
	std::string name2 = "P"+ std::to_string(playerIndex);
	name2 += "ScrollablePane";
	
	if(playerIndex==1)
		player1Panel.getChild(name2).getChild(name).setText("Total kills: " + std::to_string(score->getNumberOfKills(playerIndex)));
	else if (playerIndex == 2)
		player2Panel.getChild(name2).getChild(name).setText("Total kills: " + std::to_string(score->getNumberOfKills(playerIndex)));
	else if (playerIndex == 3)
		player3Panel.getChild(name2).getChild(name).setText("Total kills: " + std::to_string(score->getNumberOfKills(playerIndex)));
	else 
		player4Panel.getChild(name2).getChild(name).setText("Total kills: " + std::to_string(score->getNumberOfKills(playerIndex)));
}

void ScoreManager::setIndexOfPlayersKilled(int playerIndex)
{
	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "Kills";
	std::string name2 = "P" + std::to_string(playerIndex);
	name2 += "ScrollablePane";
	std::string text = "Player kills:";

	for (int i = 0; i < score->getIndexOfPlayersKilled(playerIndex).size(); i++)
	{
		text = text + " ";
		text = text + "P";
		text += std::to_string(score->getIndexOfPlayersKilled(playerIndex).at(i));
	}
	if (playerIndex == 1)
		player1Panel.getChild(name2).getChild(name).setText(text);
	else if (playerIndex == 2)
		player2Panel.getChild(name2).getChild(name).setText(text);
	else if (playerIndex == 3)
		player3Panel.getChild(name2).getChild(name).setText(text);
	else
		player4Panel.getChild(name2).getChild(name).setText(text);
}

void ScoreManager::setPercentOfHits(int playerIndex)
{
	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "PercentOfHits";
	std::string name2 = "P" + std::to_string(playerIndex);
	name2 += "ScrollablePane";

	std::string text = "Succesfull attacks: ";
	if (playerIndex == 1)
	{ 
		text += std::to_string(score->getPercentOfHits(playerIndex));
		text += "%";
		player1Panel.getChild(name2).getChild(name).setText(text);
	}
	else if (playerIndex == 2)
	{
		text += score->getPercentOfHits(playerIndex);
		text += "%";
		player2Panel.getChild(name2).getChild(name).setText(text);
	}
	else if (playerIndex == 3)
	{
		text += score->getPercentOfHits(playerIndex);
		text += "%";
		player3Panel.getChild(name2).getChild(name).setText(text);
	}
	else
	{
		text += score->getPercentOfHits(playerIndex);
		text += "%";
		player4Panel.getChild(name2).getChild(name).setText(text);
	}
}

void ScoreManager::setSuccessfullGrabs(int playerIndex)
{
	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "SuccesfullGrabs";
	std::string name2 = "P" + std::to_string(playerIndex);
	name2 += "ScrollablePane";



	if (playerIndex == 1)
		player1Panel.getChild(name2).getChild(name).setText("Successful grabs made: " + std::to_string(score->getSuccessfullGrabs(playerIndex)));
	else if (playerIndex == 2)
		player2Panel.getChild(name2).getChild(name).setText("Successful grabs made: " + std::to_string(score->getSuccessfullGrabs(playerIndex)));
	else if (playerIndex == 3)
		player3Panel.getChild(name2).getChild(name).setText("Successful grabs made: " + std::to_string(score->getSuccessfullGrabs(playerIndex)));
	else
		player4Panel.getChild(name2).getChild(name).setText("Successful grabs made: " + std::to_string(score->getSuccessfullGrabs(playerIndex)));
}

void ScoreManager::setTotalAttacks(int playerIndex)
{
	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "Attacks";
	std::string name2 = "P" + std::to_string(playerIndex);
	name2 += "ScrollablePane";

	if (playerIndex == 1)
		player1Panel.getChild(name2).getChild(name).setText("Attacks made: " + std::to_string(score->getNumberOfGroundAttacks(playerIndex)));
	else if (playerIndex == 2)
			player2Panel.getChild(name2).getChild(name).setText("Attacks made: " + std::to_string(score->getNumberOfGroundAttacks(playerIndex)));
	else if (playerIndex == 3)
			player3Panel.getChild(name2).getChild(name).setText("Attacks made: " + std::to_string(score->getNumberOfGroundAttacks(playerIndex)));
	else
			player4Panel.getChild(name2).getChild(name).setText("Attacks made: " + std::to_string(score->getNumberOfGroundAttacks(playerIndex)));
}

void ScoreManager::setAmountOfDamageDealt(int playerIndex)
{
	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "DamageDealt";
	std::string name2 = "P" + std::to_string(playerIndex);
	name2 += "ScrollablePane";

	if (playerIndex == 1)
		player1Panel.getChild(name2).getChild(name).setText("Damage dealt: " + std::to_string(score->getAmountOfDamageDealt(playerIndex)));
	else if (playerIndex == 2)
			player2Panel.getChild(name2).getChild(name).setText("Damage dealt: " + std::to_string(score->getAmountOfDamageDealt(playerIndex)));
	else if (playerIndex == 3)
			player3Panel.getChild(name2).getChild(name).setText("Damage dealt: " + std::to_string(score->getAmountOfDamageDealt(playerIndex)));
	else
			player4Panel.getChild(name2).getChild(name).setText("Damage dealt: " + std::to_string(score->getAmountOfDamageDealt(playerIndex)));
}

void ScoreManager::setLifesAsGhost(int playerIndex)
{
	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "LivesStolen";
	std::string name2 = "P" + std::to_string(playerIndex);
	name2 += "ScrollablePane";

	if (playerIndex == 1)
		player1Panel.getChild(name2).getChild(name).setText("Lives stolen: " + std::to_string(score->getLifesAsGhost(playerIndex)));
	else if (playerIndex == 2)
			player2Panel.getChild(name2).getChild(name).setText("Lives stolen: " + std::to_string(score->getLifesAsGhost(playerIndex)));
	else if (playerIndex == 3)
			player3Panel.getChild(name2).getChild(name).setText("Lives stolen: " + std::to_string(score->getLifesAsGhost(playerIndex)));
	else
			player4Panel.getChild(name2).getChild(name).setText("Lives stolen: " + std::to_string(score->getLifesAsGhost(playerIndex)));
}

void ScoreManager::setTimesHittedBySpikes(int playerIndex)
{
	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "DamageTakenByEnviroment";
	std::string name2 = "P" + std::to_string(playerIndex);
	name2 += "ScrollablePane";

	if (playerIndex == 1)
		player1Panel.getChild(name2).getChild(name).setText("Enviroment hazards hit: " + std::to_string(score->getTimesHittedBySpikes(playerIndex)));
	else if (playerIndex == 2)
			player2Panel.getChild(name2).getChild(name).setText("Enviroment hazards hit: " + std::to_string(score->getTimesHittedBySpikes(playerIndex)));
	else if (playerIndex == 3)
			player3Panel.getChild(name2).getChild(name).setText("Enviroment hazards hit: " + std::to_string(score->getTimesHittedBySpikes(playerIndex)));
	else
			player4Panel.getChild(name2).getChild(name).setText("Enviroment hazards hit: " + std::to_string(score->getTimesHittedBySpikes(playerIndex)));
}

void ScoreManager::setAmountOfFalls(int playerIndex)
{
	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "TimesFallen";
	std::string name2 = "P" + std::to_string(playerIndex);
	name2 += "ScrollablePane";
	if (playerIndex == 1)
		player1Panel.getChild(name2).getChild(name).setText("Times has fallen: " + std::to_string(score->getAmountOfFalls(playerIndex)));
	else if (playerIndex == 2)
			player2Panel.getChild(name2).getChild(name).setText("Times has fallen: " + std::to_string(score->getAmountOfFalls(playerIndex)));
	else if (playerIndex == 3)
			player3Panel.getChild(name2).getChild(name).setText("Times has fallen: " + std::to_string(score->getAmountOfFalls(playerIndex)));
	else
			player4Panel.getChild(name2).getChild(name).setText("Times has fallen: " + std::to_string(score->getAmountOfFalls(playerIndex)));
}

void ScoreManager::setTotalDamageSuffer(int playerIndex)
{
	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "TotalDamageTaken";
	std::string name2 = "P" + std::to_string(playerIndex);
	name2 += "ScrollablePane";
	if (playerIndex == 1)
		player1Panel.getChild(name2).getChild(name).setText("Damage received: " + std::to_string(score->getTotalDamageSuffer(playerIndex)));
	else if (playerIndex == 2)
			player2Panel.getChild(name2).getChild(name).setText("Damage received: " + std::to_string(score->getTotalDamageSuffer(playerIndex)));
	else if (playerIndex == 3)
			player3Panel.getChild(name2).getChild(name).setText("Damage received: " + std::to_string(score->getTotalDamageSuffer(playerIndex)));
	else
			player4Panel.getChild(name2).getChild(name).setText("Damage received: " + std::to_string(score->getTotalDamageSuffer(playerIndex)));
}

void ScoreManager::setEnviromentDeaths(int playerIndex)
{

	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "DeathsByEnviroment";
	std::string name2 = "P" + std::to_string(playerIndex);
	name2 += "ScrollablePane";
	if (playerIndex == 1)
		player1Panel.getChild(name2).getChild(name).setText("Enviroment hazards deaths: " + std::to_string(score->getEnviromentDeaths(playerIndex)));
	else if (playerIndex == 2)
			player2Panel.getChild(name2).getChild(name).setText("Enviroment hazards deaths: " + std::to_string(score->getEnviromentDeaths(playerIndex)));
	else if (playerIndex == 3)
			player3Panel.getChild(name2).getChild(name).setText("Enviroment hazards deaths: " + std::to_string(score->getEnviromentDeaths(playerIndex)));
	else
			player4Panel.getChild(name2).getChild(name).setText("Enviroment hazards deaths: " + std::to_string(score->getEnviromentDeaths(playerIndex)));
}
