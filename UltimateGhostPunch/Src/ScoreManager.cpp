
#include "ScoreManager.h"
#include <ComponentRegister.h>
#include <InputSystem.h>
#include <InterfaceSystem.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <UILayout.h>
#include <UIElement.h>
#include <Camera.h>
#include <SoundEmitter.h>

#include "Score.h"
#include "GameManager.h"

REGISTER_FACTORY(ScoreManager);

bool ScoreManager::resetButtonClick()
{
	manager->setNumPlayers(manager->getPlayerIndexes().size());

	manager->setTime(manager->getInitialTime());
	manager->setLevel(manager->getLastLevel());
	manager->setSong(manager->getLastSong());
	buttonClick("button4");

	// change scene
	SceneManager::GetInstance()->changeScene("mainScene");
	return false;
}

bool ScoreManager::backButtonClick()
{
	GameManager::GetInstance()->pauseGame(false);
	SceneManager::GetInstance()->changeScene("mainMenu");
	buttonClick("button4");
	return false;
}

void ScoreManager::buttonClick(const std::string& sound)
{
	if (soundEmitter != nullptr) soundEmitter->playSound(sound);
}

ScoreManager::ScoreManager(GameObject* gameObject) : UserComponent(gameObject)
{
	InterfaceSystem* interfaceSystem = InterfaceSystem::GetInstance();
	interfaceSystem->registerEvent("resetButtonClick", UIEvent("ButtonClicked", [this]() {return resetButtonClick(); }));
	interfaceSystem->registerEvent("backButtonClick", UIEvent("ButtonClicked", [this]() {return backButtonClick(); }));
}

ScoreManager::~ScoreManager()
{
	InterfaceSystem* interfaceSystem = InterfaceSystem::GetInstance();
	interfaceSystem->unregisterEvent("resetButtonClick");
	interfaceSystem->unregisterEvent("backButtonClick");
}

void ScoreManager::start()
{
	GameObject* mainCamera = findGameObjectWithName("MainCamera");
	if (mainCamera == nullptr)  return;

	UIElement root = mainCamera->getComponent<UILayout>()->getRoot();
	soundEmitter = mainCamera->getComponent<SoundEmitter>();
	for (int i = 0; i < 4; i++)
	{
		std::string name = "P" + std::to_string(i + 1);
		texts.push_back(root.getChild(name));
		name = name + "Background";
		panels.push_back(root.getChild(name));
	}
	manager = GameManager::GetInstance();

	reposition(manager->getPlayerIndexes().size());
	initStatistics(manager->getPlayerIndexes().size());
}

void ScoreManager::initStatistics(int numOfPlayers)
{
	for (int i = 0; i < numOfPlayers; i++)
	{
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
	float size = (1 - 0.2) / numOfPlayers;
	float iTextPos = 0.15;
	float iPanelPos = 0.1;

	for (int i = 0; i < numOfPlayers; i++)
	{
		float textPos = iTextPos + size * i;
		float panelPos = iPanelPos + size * i;

		texts.at(i).setVisible(true);
		texts.at(i).setPosition(textPos, 0.1);
		panels.at(i).setVisible(true);
		panels.at(i).setPosition(panelPos, 0.2);
		panels.at(i).setSize(size, 0.6);
	}
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
	panels.at(playerIndex - 1).getChild(name2).getChild(name).setText(text);
}

void ScoreManager::setPercentOfHits(int playerIndex)
{
	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "PercentOfHits";

	std::string name2 = "P" + std::to_string(playerIndex);
	name2 += "ScrollablePane";

	std::string text = "Succesfull attacks: ";

	text += std::to_string(score->getPercentOfHits(playerIndex));
	text += "%";
	panels.at(playerIndex - 1).getChild(name2).getChild(name).setText(text);
}

void ScoreManager::setSuccessfullGrabs(int playerIndex)
{
	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "SuccesfullGrabs";

	std::string name2 = "P" + std::to_string(playerIndex);
	name2 += "ScrollablePane";

	panels.at(playerIndex - 1).getChild(name2).getChild(name).setText("Successful grabs: " + std::to_string(score->getSuccessfullGrabs(playerIndex)));

}

void ScoreManager::setTotalAttacks(int playerIndex)
{
	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "Attacks";

	std::string name2 = "P" + std::to_string(playerIndex);
	name2 += "ScrollablePane";

	panels.at(playerIndex - 1).getChild(name2).getChild(name).setText("Attacks made: " + std::to_string(score->getNumberOfGroundAttacks(playerIndex)));
}

void ScoreManager::setAmountOfDamageDealt(int playerIndex)
{
	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "DamageDealt";

	std::string name2 = "P" + std::to_string(playerIndex);
	name2 += "ScrollablePane";

	panels.at(playerIndex - 1).getChild(name2).getChild(name).setText("Damage dealt: " + std::to_string(score->getAmountOfDamageDealt(playerIndex)));
}

void ScoreManager::setLifesAsGhost(int playerIndex)
{
	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "LivesStolen";

	std::string name2 = "P" + std::to_string(playerIndex);
	name2 += "ScrollablePane";

	panels.at(playerIndex - 1).getChild(name2).getChild(name).setText("Lives stolen: " + std::to_string(score->getLifesAsGhost(playerIndex)));
}

void ScoreManager::setTimesHittedBySpikes(int playerIndex)
{
	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "DamageTakenByEnviroment";

	std::string name2 = "P" + std::to_string(playerIndex);
	name2 += "ScrollablePane";

	panels.at(playerIndex - 1).getChild(name2).getChild(name).setText("Enviromental hit: " + std::to_string(score->getTimesHittedBySpikes(playerIndex)));
}

void ScoreManager::setAmountOfFalls(int playerIndex)
{
	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "TimesFallen";

	std::string name2 = "P" + std::to_string(playerIndex);
	name2 += "ScrollablePane";

	panels.at(playerIndex - 1).getChild(name2).getChild(name).setText("Times fallen: " + std::to_string(score->getAmountOfFalls(playerIndex)));
}

void ScoreManager::setTotalDamageSuffer(int playerIndex)
{
	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "TotalDamageTaken";

	std::string name2 = "P" + std::to_string(playerIndex);
	name2 += "ScrollablePane";

	panels.at(playerIndex - 1).getChild(name2).getChild(name).setText("Damage received: " + std::to_string(score->getTotalDamageSuffer(playerIndex)));
}

void ScoreManager::setEnviromentDeaths(int playerIndex)
{
	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "DeathsByEnviroment";

	std::string name2 = "P" + std::to_string(playerIndex);
	name2 += "ScrollablePane";

	panels.at(playerIndex - 1).getChild(name2).getChild(name).setText("Enviromental deaths: " + std::to_string(score->getEnviromentDeaths(playerIndex)));
}