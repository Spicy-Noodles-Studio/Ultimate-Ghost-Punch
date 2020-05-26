#include "StatsMenu.h"

#include <ComponentRegister.h>
#include <InputSystem.h>
#include <InterfaceSystem.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <UILayout.h>
#include <UIElement.h>
#include <Camera.h>

#include "Score.h"
#include "GameManager.h"

REGISTER_FACTORY(StatsMenu);

StatsMenu::StatsMenu(GameObject* gameObject) : Menu(gameObject), texts(), panels(), score(nullptr)
{

}

StatsMenu::~StatsMenu()
{
	score = nullptr;

	texts.clear();
	panels.clear();
}

void StatsMenu::start()
{
	Menu::start();

	UILayout* layout = nullptr;
	UIElement root = NULL;

	if (notNull(mainCamera))
	{
		layout = mainCamera->getComponent<UILayout>();

		if (notNull(layout))
			root = layout->getRoot();
	}

	checkNullAndBreak(gameManager);

	score = gameManager->getScore();

	positions = score->getPlayerID();

	for (int i = 0; i < positions.size(); i++)
	{
		std::string name = "P" + std::to_string(positions[i]);
		texts.push_back(root.getChild(name));
		root.getChild(name).setText("Player " + std::to_string(positions[i]) + ": " + std::to_string(gameManager->getPlayerRanking(positions[i])) + "º");

		name = name + "Background";
		panels.push_back(root.getChild(name));
	}

	if (gameManager->getWinner() != -1)
		root.getChild("Result").setText("WINNER: PLAYER " + std::to_string(positions[gameManager->getWinner() - 1]));
	else
		root.getChild("Result").setText("TIE");

	score = gameManager->getScore();
	checkNull(score);

	reposition(gameManager->getInitialPlayers());
	initStatistics(gameManager->getInitialPlayers());
}

void StatsMenu::update(float deltaTime)
{
	if (notNull(inputSystem) && notNull(sceneManager) && (inputSystem->getKeyPress("ESCAPE") || checkControllersInput()))
		sceneManager->changeScene("ConfigurationMenu");
}

bool StatsMenu::checkControllersInput()
{
	bool result = false;

	int i = 0;
	while (i < 4 && !result)
	{
		if (notNull(inputSystem) && (inputSystem->getButtonPress(i, "B") || inputSystem->getButtonPress(i, "A")))
			result = true;

		i++;
	}

	return result;
}

void StatsMenu::reposition(int numOfPlayers)
{
	float size = (1 - 0.2) / numOfPlayers;
	float iTextPos = 0.15;
	float iPanelPos = 0.1;

	for (int i = 0; i < numOfPlayers; i++)
	{
		float textPos = iTextPos + size * i;
		float panelPos = iPanelPos + size * i;

		texts.at(i).setVisible(true);
		texts.at(i).setPosition(textPos, 0.2);

		panels.at(i).setVisible(true);
		panels.at(i).setPosition(panelPos, 0.3);
		panels.at(i).setSize(size, 0.6);
	}
}

void StatsMenu::initStatistics(int numOfPlayers)
{
	for (int i = 0; i < numOfPlayers; i++)
	{
		setPlayersKilled(i);

		setDamageDealt(i);
		setDamageReceived(i);
		setAccuracy(i);

		setTimesFallen(i);

		setHitsByEnviroment(i);
		setDeathsByEnviroment(i);
	}
}

void StatsMenu::setPlayersKilled(int playerIndex)
{
	std::string name = "P" + std::to_string(positions[playerIndex]);
	name = name + "Kills";

	std::string text = "Players killed:";

	if (notNull(score))
		for (int i = 0; i < score->getIndexOfPlayersKilled(playerIndex).size(); i++)
			text += " P" + std::to_string(positions[score->getIndexOfPlayersKilled(playerIndex).at(i)]);

	panels.at(playerIndex).getChild(name).setText(text);
}

void StatsMenu::setDamageDealt(int playerIndex)
{
	std::string name = "P" + std::to_string(positions[playerIndex]);
	name = name + "DamageDealt";

	if (notNull(score))
		panels.at(playerIndex).getChild(name).setText("Damage dealt: " + std::to_string(score->getAmountOfDamageDealt(playerIndex)));
}

void StatsMenu::setDamageReceived(int playerIndex)
{
	std::string name = "P" + std::to_string(positions[playerIndex]);
	name = name + "DamageReceived";

	if (notNull(score))
		panels.at(playerIndex).getChild(name).setText("Damage received: " + std::to_string(score->getTotalDamageReceived(playerIndex)));
}

void StatsMenu::setAccuracy(int playerIndex)
{
	std::string name = "P" + std::to_string(positions[playerIndex]);
	name = name + "AccuracyPercentage";

	if (notNull(score))
		panels.at(playerIndex).getChild(name).setText("Accuracy: " + std::to_string(score->getPercentageOfHits(playerIndex)) + "%");
}

void StatsMenu::setTimesFallen(int playerIndex)
{
	std::string name = "P" + std::to_string(positions[playerIndex]);
	name = name + "TimesFallen";

	if (notNull(score))
		panels.at(playerIndex).getChild(name).setText("Times fallen: " + std::to_string(score->getAmountOfFalls(playerIndex)));
}

void StatsMenu::setHitsByEnviroment(int playerIndex)
{
	std::string name = "P" + std::to_string(positions[playerIndex]);
	name = name + "HitsByEnviroment";

	if (notNull(score))
		panels.at(playerIndex).getChild(name).setText("Hits by spikes: " + std::to_string(score->getTimesHittedBySpikes(playerIndex)));
}

void StatsMenu::setDeathsByEnviroment(int playerIndex)
{
	std::string name = "P" + std::to_string(positions[playerIndex]);
	name = name + "DeathsByEnviroment";

	if (notNull(score))
		panels.at(playerIndex).getChild(name).setText("Deaths by\nspikes and falls: " + std::to_string(score->getEnviromentDeaths(playerIndex)));
}