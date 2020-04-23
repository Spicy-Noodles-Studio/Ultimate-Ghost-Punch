#include "PlatformNavigation.h"

#include <GameObject.h>
#include <RigidBody.h>
#include <queue>

#include "AIStateMachine.h"
#include "Movement.h"

PlatformNavigation::PlatformNavigation(StateMachine* stateMachine) : StateAction(stateMachine), platformGraph(nullptr), character(nullptr), movingThroughLink(false), linkInUse(NavigationLink()), time(0.0f), lastState(-1)
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
	this->character = character->findChildrenWithTag("groundSensor")[0];

	// Inicial target should be current platform
	setTarget(this->character->transform->getPosition());
}

void PlatformNavigation::setTarget(const Vector3& position)
{
	if (platformGraph == nullptr) return;

	int index = platformGraph->getIndex(position);
	const std::vector<PlatformNode>& graph = platformGraph->getPlatforms();

	if (index < 0 || index >= graph.size()) return;

	target = platformGraph->getPlatforms()[index];
}

void PlatformNavigation::setTarget(GameObject* target)
{
	setTarget(target->transform->getPosition());
}

bool PlatformNavigation::hasArrived() const
{
	if (character == nullptr || platformGraph == nullptr) return false;

	return target.getIndex() == platformGraph->getIndex(character->transform->getWorldPosition());
}

std::vector<PlatformNavigation::PathNode> PlatformNavigation::getShortestPath()
{
	std::vector<PathNode> path;
	if (platformGraph == nullptr || character == nullptr)
		return path; // Empty

	std::vector<PlatformNode>& graph = platformGraph->getPlatforms();
	int startIndex = platformGraph->getIndex(character->transform->getWorldPosition());

	if (startIndex >= graph.size() || startIndex < 0)
		return path;

	// Cost - Node index (order)
	std::priority_queue<ii, std::vector<ii>, std::greater<ii>> pq;
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
			float beginCost = std::abs(edge.getIniPos().x - character->transform->getWorldPosition().x); // Coste de recorrer la plataforma
			// If find a better cost
			if (cost[index] + toCost + beginCost < cost[toIndex]) {
				cost[toIndex] = cost[index] + toCost + beginCost;
				pq.push({ cost[toIndex], toIndex });
				route[toIndex] = { index, i };
			}
		}
	}

	// Build path
	int index = target.getIndex();
	while (index >= 0 && index < route.size() && startIndex != index) {
		if (route[index].first >= 0 && route[index].first < graph.size())
			path.push_back({ graph[route[index].first], route[index].second });

		std::reverse(path.begin(), path.end());
		index = route[index].first;
	}
	return path;
}

void PlatformNavigation::moveToStartingPoint(const PathNode& node)
{
	if (character == nullptr) return;

	Vector3 startPos = node.platform.getEdge(node.index).getIniPos(), characterPos = character->transform->getWorldPosition();

	// Si la diferencia en altura el muy grande, node no es valido
	if (std::abs(characterPos.y - startPos.y) > 1.0f) return;

	float diff = std::abs(characterPos.x - startPos.x);
	// If arrived to start point
	if (diff < 0.1f) {
		movingThroughLink = true;
		lastState = 0;
		linkInUse = node.platform.getEdge(node.index);
		time = 0.0f;
		

		//Set rigidbody to link´s inicial state
		if (character->getParent()) {
			character->getParent()->transform->setRotation({ 0, 90.0 * linkInUse.getDirection(),0 });
			RigidBody* rb = character->getParent()->getComponent<RigidBody>();
			if (rb != nullptr) {
				rb->setLinearVelocity(linkInUse.getStartVelocity());
				rb->clearForces();
				rb->addForce(linkInUse.getStartForce());
			}
		}		
		return;
	}

	ActionInput action = (startPos.x < characterPos.x) ? ActionInput::MOVE_LEFT : ActionInput::MOVE_RIGHT;
	stateMachine->addActionInput(action);
}

void PlatformNavigation::moveToPlatform()
{
	// If arrived to end of link, reset
	if (linkInUse.getDuration() < time || (character->transform->getWorldPosition() - linkInUse.getEndPos()).magnitude() < 0.1) {
		movingThroughLink = false;
		time = 0.0f;
		lastState = 0;
		if (platformGraph->getIndex(linkInUse.getEndPos()) == target.getIndex()) 
			((AIStateMachine*)stateMachine)->startPlatformMovement();
		return;
	}

	Vector3 vel = character->getComponent<RigidBody>()->getLinearVelocity();
	std::vector<State> states = linkInUse.getStates();
	if (states.size() > 0) {
		bool processed = false;
		while (lastState >= 0 && lastState < states.size() && time >= states[lastState].getTime()) {
			// Inject input
			for (Action action : states[lastState].getActions())
				stateMachine->addActionInput((ActionInput)action);
			lastState++;
			processed = true;
		}
		// Compensate time diference by injecting last state input again (unless was the last one)
		if (!processed && lastState - 1 >= 0 && lastState - 1 < states.size() - 1) {
			for (Action action : states[lastState - 1].getActions())
				stateMachine->addActionInput((ActionInput)action);
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
		std::vector<PathNode> path = getShortestPath();
		// Get next node
		if (path.size() > 0) 
			// Go to next platform
			moveToStartingPoint(path[0]);

		else 
			((AIStateMachine*)stateMachine)->startPlatformMovement();
	}
}
