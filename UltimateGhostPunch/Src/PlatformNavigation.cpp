#include "PlatformNavigation.h"

#include <GameObject.h>
#include <queue>

#include "AIStateMachine.h"

PlatformNavigation::PlatformNavigation(StateMachine* stateMachine) : StateAction(stateMachine), platformGraph(nullptr), character(nullptr), movingThroughLink(false), linkInUse(NavigationLink()), time(0.0f), lastState(-1)
{

}

PlatformNavigation::~PlatformNavigation()
{

}

void PlatformNavigation::setPlatformGraph(PlatformGraph* platformGraph)
{
	this->platformGraph = platformGraph;	
	target = platformGraph->getPlatforms()[platformGraph->getPlatforms().size() - 2];
}

void PlatformNavigation::setCharacter(GameObject* character)
{
	this->character = character->findChildrenWithTag("groundSensor")[0];
}

std::vector<PlatformNavigation::pathNode> PlatformNavigation::getShortestPath()
{
	std::vector<pathNode> path;
	if (platformGraph == nullptr || character == nullptr)
		return path; // Empty

	std::vector<PlatformNode>& graph = platformGraph->getPlatforms();
	int startIndex = platformGraph->getIndex(character->transform->getWorldPosition());

	if (startIndex >= graph.size() || startIndex < 0)
		return path;

	// Cost - Node index (order)
	std::priority_queue<std::pair<int, int>,
		std::vector<std::pair<int, int>>,
		std::greater<std::pair<int, int>>> pq;
	pq.push({ 0, startIndex });

	// Vector of distances
	std::vector<float> cost(graph.size(), INFINITY);
	cost[startIndex] = 0.0f;

	// Route
	std::vector<ii> route(graph.size(), { -1, -1 });//Index of the platform and of the link to that platform

	while (!pq.empty()) {
		std::pair<int, int> front = pq.top(); pq.pop();
		int prevCost = front.first;
		int index = front.second;

		if (prevCost > cost[index]) continue;
		// For each connection
		auto edges = graph[index].getEdges();
		for (int i = 0; i < edges.size(); i++) {
			NavigationLink edge = edges[i];
			int toIndex = edge.getConnection();
			float toCost = edge.getDuration();
			float beginCost = std::abs(edge.getIniPos().x - character->transform->getWorldPosition().x);//Coste de reccorrer la plataforma
			// If find a better cost
			if (cost[index] + toCost + beginCost < cost[toIndex]) {
				cost[toIndex] = cost[index] + toCost + beginCost;
				pq.push({ cost[toIndex], toIndex });
				route[toIndex] = { index, i };
			}
		}
	}

	// Build path
	int index =target.getIndex();
	while (index >= 0 && index<route.size() && startIndex != index) {
		path.insert(path.begin(), { graph[route[index].first], route[index].second });
		index = route[index].first;
	}
	return path;
}

void PlatformNavigation::moveToStartingPoint(const pathNode& node)
{
	if (character == nullptr) return;

	Vector3 startPos = node.platform.getEdge(node.index).getIniPos(), characterPos = character->transform->getWorldPosition();

	if (std::abs(characterPos.x - startPos.x) < 0.5) {
		movingThroughLink = true;
		lastState = 0;
		linkInUse = node.platform.getEdge(node.index);
		time = 0.0f;
		return;
	}

	ActionInput action = (startPos.x < characterPos.x) ? ActionInput::MOVE_LEFT : ActionInput::MOVE_RIGHT;
	stateMachine->addActionInput(action);
}

void PlatformNavigation::moveToPlatform()
{
	if (linkInUse.getDuration() < time || (character->transform->getWorldPosition() - linkInUse.getEndPos()).magnitude() < 0.5) {
		movingThroughLink = false;
		time = 0.0f;
		lastState = 0;
	}

	std::vector<State> states = linkInUse.getStates();
	if (states.size() > 0) {
		while (lastState >= 0 && lastState < states.size() && time > states[lastState].getTime()) {
			for (Action action : states[lastState].getActions())
				stateMachine->addActionInput((ActionInput)action);
			lastState++;
		}
	}
}

void PlatformNavigation::update(float deltaTime)
{
	if (movingThroughLink) {
		moveToPlatform();
		time += deltaTime;
	}
	else {
		/* CALCULATIONS TO FOLLOW NODES CORRECTLY */
		std::vector<pathNode> path = getShortestPath();

		// Get next node
		if (path.size() > 0) 
			// Go to next platform
			moveToStartingPoint(path[0]);
	}

}
