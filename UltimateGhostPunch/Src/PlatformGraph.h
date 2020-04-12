#pragma once

#ifndef PLATFORM_GRAPH_H
#define PLATFORM_GRAPH_H

#include <UserComponent.h>
#include <vector>
#include <map>

class PhysicsSystem;
class RaycastHit;
class PlatformNode;
class NavigationLink;

class PlatformGraph : public UserComponent
{
private:
	PhysicsSystem* physicsSystem;
	Vector3 levelStart, levelEnd, currentPos;

	std::vector<PlatformNode> platforms;

	std::map<float, int> lastPlatforms;

	int currentPlatformIndex;
	Vector3 fallOffset, playerCollisionSize;

	std::string saveFilename, loadFilename, fileRoute;

	void drawLinks();

public:
	PlatformGraph(GameObject* gameObject);
	virtual ~PlatformGraph();

	virtual void start();
	virtual void update(float deltaTime);

	virtual void handleData(ComponentData* data);

	void createNodes();
	void createLinks();

	void saveGraph();
	bool loadGraph();

	void addLinkToPlatform(int platform, const NavigationLink& navLink);

	int getIndex(const Vector3& pos);
};

#endif

