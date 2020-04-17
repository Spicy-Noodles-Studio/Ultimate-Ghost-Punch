#include "PlatformNavigation.h"
#include <GameObject.h>
#include <queue>


PlatformNavigation::PlatformNavigation(StateMachine* stateMachine) : StateAction(stateMachine), platformGraph(nullptr), character(nullptr)
{

}

PlatformNavigation::~PlatformNavigation()
{

}

void PlatformNavigation::setPlatformGraph(PlatformGraph* platformGraph)
{
	this->platformGraph = platformGraph;
}

void PlatformNavigation::setCharacter(GameObject* character)
{
	this->character = character;
}

std::vector<PlatformNode> PlatformNavigation::getShortestPath()
{
	std::vector<PlatformNode> path;
	if(platformGraph == nullptr || character == nullptr)
		return path; // Empty

	std::vector<PlatformNode>& graph = platformGraph->getPlatforms();
	int startIndex = platformGraph->getIndex(character->transform->getPosition());

	if (startIndex >= graph.size() || startIndex < 0)
		return path;

	// Cost - Node index (order)
	std::priority_queue<std::pair<int, int>, 
						std::vector<std::pair<int, int>>, 
						std::greater<std::pair<int, int>>> pq;
	pq.push({ 0, startIndex });

	// Vector of distances
	std::vector<float> cost(0.0f, graph.size());
	cost[startIndex] = 0.0f;

	// Route
	std::vector<int> route(-1, graph.size());

	while (!pq.empty()) {
		std::pair<int, int> front = pq.top(); pq.pop();
		int prevCost = front.first;
		int index = front.second;

		if (prevCost > cost[index]) continue;
		// For each connection
		for (auto edge : graph[index].getEdges()) {
			int toIndex = edge.getConnection();
			int toCost = edge.getDuration();
			// If find a better cost
			// TODO: de momento solo tiene en cuenta el coste por arista sin tener el cuenta el coste de recorrer la plataforma
			if (cost[index] + toCost < cost[toIndex]) {
				cost[toIndex] = cost[index] + toCost;
				pq.push({ cost[toIndex], toIndex });
				route[index] = toIndex;
			}
		}
	}

	// Build path
	int index = route[startIndex]; // One step ahead (current node does not count)
	while (target.getIndex() != route[index]) {
		path.push_back(graph[index]);
	}
	// Add target
	path.push_back(target);
}

void PlatformNavigation::update(float deltaTime)
{
	/* CALCULATIONS TO FOLLOW NODES CORRECTLY */
	std::vector<PlatformNode> path = getShortestPath();

	// Get next platform
	PlatformNode next = path[0];

	// Go to next platform
	
}
