#include "PlatformGraph.h"

#include <ComponentRegister.h>
#include <PhysicsSystem.h>
#include <RaycastHit.h>
#include <Transform.h>
#include <GameObject.h>
#include <sstream>

#include "PlatformNode.h"

REGISTER_FACTORY(PlatformGraph);

void PlatformGraph::drawLinks()
{
	for (PlatformNode node : platforms) {
		for (NavigationLink n : node.getEdges()) {
			auto v = n.getStates();
			for(int i=0; i< v.size()-1;i++)
			physicsSystem->drawLine(v[i].getPos(), v[i+1].getPos(), { 0,1,0 });
		}
	}
}

PlatformGraph::PlatformGraph(GameObject* gameObject) : UserComponent(gameObject), physicsSystem(nullptr), levelStart(Vector3()), levelEnd(Vector3()), currentPos(Vector3()), currentPlatformIndex(0),
													   fallOffset(Vector3(1.0f, 0.0f, 0.0f)), playerCollisionSize(Vector3(1.0f, 2.0f, 1.0f))
{
}

PlatformGraph::~PlatformGraph()
{
}

void PlatformGraph::start()
{
	physicsSystem = PhysicsSystem::GetInstance();

	currentPos = levelStart;

	createNodes();
}

void PlatformGraph::update(float deltaTime)
{
	drawLinks();
}

void PlatformGraph::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "levelStart")
		{
			if (!(ss >> levelStart.x >> levelStart.y >> levelStart.z))
				LOG("PLATFORM GRAPH: Invalid value for property %s", prop.first.c_str());
		}
		if (prop.first == "levelEnd") {
			if (!(ss >> levelEnd.x >> levelEnd.y >> levelEnd.z))
				LOG("PLATFORM GRAPH: Invalid value for property %s", prop.first.c_str());
		}
		else
			LOG("PLATFORM GRAPH: Invalid property with name %s", prop.first.c_str());
	}
}

void PlatformGraph::createNodes()
{
	std::vector<RaycastHit> hits;
	while(currentPos.x < levelEnd.x) {
		hits = physicsSystem->raycastAll({ currentPos.x,levelStart.y,0 }, { currentPos.x,levelEnd.y,0 });
		std::map<float, int> newPlatforms;
		for (RaycastHit hit : hits) {
			if (hit.transform->gameObject->getTag() == "suelo" && hit.normal == Vector3(0, 1, 0)) {
				auto it = lastPlatforms.find(hit.point.y);
				//If we have seen this platform in the last iteration
				if (it != lastPlatforms.end()) {
					newPlatforms[hit.point.y] = (*it).second;
					platforms[(*it).second].setEnd(hit.point);
				}
				//If its a new platform
				else {
					PlatformNode node = PlatformNode(hit.point);
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
	for (PlatformNode node : platforms) {
		Vector3 raycastLeft = node.getBegining() - fallOffset, raycastRight = node.getEnd() + fallOffset;

		if (!physicsSystem->raycast(node.getBegining() + Vector3(0, 0.5f, 0), Vector3::NEGATIVE_RIGHT, playerCollisionSize.x, hit)) {
			if (physicsSystem->raycast(raycastLeft, { raycastLeft.x, levelEnd.y,0 }, hit)) {
				;
			}
		}
		if (!physicsSystem->raycast(node.getEnd() + Vector3(0, 0.5f, 0), Vector3::RIGHT, playerCollisionSize.x, hit)) {
			if (physicsSystem->raycast(raycastRight, { raycastRight.x, levelEnd.y,0 }, hit)) {
				;
			}
		}
	}
}

void PlatformGraph::addLinkToPlatform(int platform, const NavigationLink& navLink)
{
	if (platform < platforms.size())
		platforms[platform].addEdge(navLink);
}

int PlatformGraph::getIndex(const Vector3& pos)
{
	int index = -1;
	float minYDiff = INFINITY, yDiff = 0.0f;
	for (int i = 0; i < platforms.size(); i++) {
		PlatformNode node = platforms[i];
		if (node.getBegining().x > pos.x || node.getEnd().x < pos.x || node.getEnd().y > pos.y)
			continue;
		yDiff = pos.y - node.getEnd().y;
		if (yDiff < minYDiff) {
			minYDiff = yDiff;
			index = i;
		}
	}
	return index;
}
