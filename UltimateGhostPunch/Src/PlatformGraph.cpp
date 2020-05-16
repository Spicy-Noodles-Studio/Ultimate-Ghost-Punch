#include "PlatformGraph.h"
#include <ComponentRegister.h>
#include <PhysicsSystem.h>
#include <RaycastHit.h>
#include <GameObject.h>
#include <Transform.h>
#include <GaiaData.h>
#include <sstream>

#include "PlatformNode.h"
#include "GameManager.h"

REGISTER_FACTORY(PlatformGraph);

void PlatformGraph::drawLinks()
{
	for (PlatformNode node : platforms)
		for (NavigationLink n : node.getEdges())
		{
			std::vector<State> states = n.getStates();

			for (int i = 0; i < states.size() - 1; i++)
				physicsSystem->drawLine(states[i].getPos(), states[i + 1].getPos(), { 0,1,0 });

			if (states.size() > 0)
				physicsSystem->drawLine(states[states.size() - 1].getPos(), n.getEndPos(), { 0,0,1 });
		}
}

float PlatformGraph::getDistance(const Vector3& pos, const PlatformNode& node)
{
	Vector3 aux = node.getMiddle();

	float distance = sqrt(pow((aux.x - pos.x),2) + pow((aux.y - pos.y), 2) + pow((aux.z - pos.z), 2));


	return distance;
}

PlatformGraph::PlatformGraph(GameObject* gameObject) : UserComponent(gameObject), physicsSystem(nullptr), levelStart(Vector3()), levelEnd(Vector3()), currentPos(Vector3()), currentPlatformIndex(0),
													   fallOffset(Vector3(1.0f, 0.0f, 0.0f)), playerCollisionSize(Vector3(0.75f, 2.0f, 1.0f)), fileRoute("./Assets/Levels/"), saveFilename("PlatformsGraph.graph"),
													   loadFilename(saveFilename)
{

}

PlatformGraph::~PlatformGraph()
{

}

void PlatformGraph::start()
{
	physicsSystem = PhysicsSystem::GetInstance();

	setLoadFileName(GameManager::GetInstance()->getLevel().second + ".graph");
	setSaveFileName(GameManager::GetInstance()->getLevel().second + ".graph");

	//If the graph was not loaded we create an empty one
	if (!loadGraph())
	{
		currentPos = levelStart;
		createNodes();
	}
}

void PlatformGraph::update(float deltaTime)
{
#ifdef _DEBUG
	drawLinks();
#endif
}

void PlatformGraph::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "levelStart")
		{
			setVector3(levelStart);
		}
		else if (prop.first == "levelEnd")
		{
			setVector3(levelEnd);
		}
		else if (prop.first == "saveFilename")
		{
			setString(saveFilename);
		}
		else if (prop.first == "loadFilename")
		{
			setString(loadFilename);
		}
		else
			LOG("PLATFORM GRAPH: Invalid property with name %s", prop.first.c_str());
	}
}

void PlatformGraph::createNodes()
{
	std::vector<RaycastHit> hits;
	while (currentPos.x < levelEnd.x)
	{
		hits = physicsSystem->raycastAll({ currentPos.x,levelStart.y,0 }, { currentPos.x,levelEnd.y,0 });
		std::map<float, int> newPlatforms;

		for (RaycastHit hit : hits)
		{
			if (hit.transform->gameObject->getTag() == "suelo" && hit.normal == Vector3(0, 1, 0))
			{
				auto it = lastPlatforms.find(hit.point.y);

				//If we have seen this platform in the last iteration
				if (it != lastPlatforms.end())
				{
					newPlatforms[hit.point.y] = (*it).second;
					platforms[(*it).second].setEnd(hit.point);
				}
				//If its a new platform
				else
				{
					PlatformNode node = PlatformNode(hit.point, currentPlatformIndex);
					platforms.push_back(node);
					newPlatforms[hit.point.y] = currentPlatformIndex;
					currentPlatformIndex += 1;
				}
			}
		}

		lastPlatforms.clear();
		lastPlatforms = newPlatforms;
		currentPos.x += 0.25f;
	}
}

void PlatformGraph::createLinks()
{
	RaycastHit hit;
	for (PlatformNode node : platforms)
	{
		Vector3 raycastLeft = node.getBegining() - fallOffset, raycastRight = node.getEnd() + fallOffset;

		if (!physicsSystem->raycast(node.getBegining() + Vector3(0, 0.5f, 0), Vector3::NEGATIVE_RIGHT, playerCollisionSize.x, hit))
			if (physicsSystem->raycast(raycastLeft, { raycastLeft.x, levelEnd.y,0 }, hit));

		if (!physicsSystem->raycast(node.getEnd() + Vector3(0, 0.5f, 0), Vector3::RIGHT, playerCollisionSize.x, hit))
			if (physicsSystem->raycast(raycastRight, { raycastRight.x, levelEnd.y,0 }, hit));
	}
}

void PlatformGraph::saveGraph()
{
	GaiaData data;
	data.addElement<std::string>("numberOfPlatforms", std::to_string(platforms.size()));

	std::vector<GaiaData>platformsData;
	for (PlatformNode node : platforms)
		platformsData.push_back(node.savePlatform());

	data.addElement("platforms", platformsData);
	data.save(fileRoute + saveFilename);
}

bool PlatformGraph::loadGraph()
{
	GaiaData data;
	if (!data.load(fileRoute + loadFilename))
		return false;

	GaiaData nPlat = data.find("numberOfPlatforms");
	std::stringstream ss(nPlat.getValue());

	int numberPlatforms; ss >> numberPlatforms;
	if (numberPlatforms < 0)
		return false;

	platforms = std::vector<PlatformNode>(numberPlatforms);

	GaiaData platformsData = data.find("platforms");
	for (auto it = platformsData.begin(); it != platformsData.end(); it++)
	{
		PlatformNode node;
		node.loadPlatform((*it));
		platforms[node.getIndex()] = node;
	}

	return true;
}

void PlatformGraph::clearAllConnections()
{
	for (int i = 0; i < platforms.size(); i++)
		clearConnections(i);
}

void PlatformGraph::clearConnections(int platform)
{
	if (platform < platforms.size() && platform >= 0)
		platforms[platform].removeAllEdges();
}

void PlatformGraph::addLinkToPlatform(int platform, const NavigationLink& navLink)
{
	if (platform < platforms.size() && platform >= 0)
		platforms[platform].addEdge(navLink);
}

void PlatformGraph::removeLastLink(int platform)
{
	if (platform < platforms.size() && platform >= 0)
		platforms[platform].removeLastEdge();
}

void PlatformGraph::setSaveFileName(std::string name)
{
	saveFilename = name;
}

void PlatformGraph::setLoadFileName(std::string name)
{
	 loadFilename = name;
}

int PlatformGraph::getIndex(const Vector3& pos)
{
	int index = -1;
	float minYDiff = INFINITY, yDiff = 0.0f;

	for (int i = 0; i < platforms.size(); i++)
	{
		PlatformNode node = platforms[i];
		if (node.getBegining().x > pos.x + playerCollisionSize.x || node.getEnd().x < pos.x - playerCollisionSize.x ||	// Off limits
			node.getEnd().y > pos.y + playerCollisionSize.y || abs(node.getEnd().y - pos.y) > playerCollisionSize.y)	// Height difference in range
			continue;

		yDiff = pos.y - node.getEnd().y;
		if (yDiff < minYDiff)
		{
			minYDiff = yDiff;
			index = i;
		}
	}
	return index;
}

int PlatformGraph::getFurthestIndex(const Vector3& pos)
{
	int index = -1;
	
	float max = -1;

	for (int i = 0; i < platforms.size(); i++)
	{
		PlatformNode node = platforms[i];

		float aux = getDistance(pos,node);

		if (aux > max) {
			max = aux;
			index = i;
		}
	}
	return index;
}

std::vector<PlatformNode>& PlatformGraph::getPlatforms()
{
	return platforms;
}