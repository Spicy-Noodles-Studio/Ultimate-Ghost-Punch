#include "LoadingScreen.h"

#include <ComponentRegister.h>
#include <InterfaceSystem.h>
#include <SceneManager.h>
#include <WindowManager.h>
#include <GameObject.h>
#include <RigidBody.h>
#include <Camera.h>
#include <UILayout.h>
#include <UIElement.h>
#include <time.h>

REGISTER_FACTORY(LoadingScreen);

LoadingScreen::LoadingScreen(GameObject* gameObject) : UserComponent(gameObject)
{
}

LoadingScreen::~LoadingScreen()
{
}

std::string LoadingScreen::getRandomTip()
{
	srand(time(NULL));
	if (tipsVector.empty())
	{
		return "No tips for you";
	}
	int i = rand() % (tipsVector.size() - 1);

	return tipsVector[i];
}

void LoadingScreen::start()
{
	sceneToLoad = "NO SCENE";
	loadDelay = 1.0f;
	currentDelay = 0;

	UIElement root = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot();
	UIElement tipsText = root.getChild("Tips");
	tipsText.setText(getRandomTip());
	tipsText.setProperty("HorzFormatting", "WordWrapCentred");

}

void LoadingScreen::update(float deltaTime)
{
	if(sceneToLoad == "NO SCENE") currentDelay += deltaTime;

	if (currentDelay > loadDelay) {
		sceneToLoad = SceneManager::GetInstance()->getSceneToLoad();
		if (sceneToLoad != "NO SCENE") SceneManager::GetInstance()->changeScene(sceneToLoad);
	}
}

void LoadingScreen::handleData(ComponentData* data)
{
	int i = 1;
	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		std::string propName = "tip" + std::to_string(i);
		if (prop.first == propName)
		{
			tipsVector.push_back(prop.second);
		}
		i++;
	}
}
