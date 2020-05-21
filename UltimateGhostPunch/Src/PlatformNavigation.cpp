#include "PlatformNavigation.h"

#include <GameObject.h>
#include <RigidBody.h>
#include <queue>

#include "AIStateMachine.h"
#include "Movement.h"

PlatformNavigation::PlatformNavigation(StateMachine* stateMachine) : StateAction(stateMachine), platformGraph(nullptr), character(nullptr), movingThroughLink(false),
linkInUse(NavigationLink()), time(0.0f), lastState(-1)
{

}

PlatformNavigation::~PlatformNavigation()
{

}

void PlatformNavigation::setPlatformGraph(PlatformGraph* platformGraph)
{
	checkNullAndBreak(platformGraph);
	this->platformGraph = platformGraph;
}

void PlatformNavigation::setCharacter(GameObject* character)
{
	checkNullAndBreak(character);

	std::vector<GameObject*> gameObjects = character->findChildrenWithTag("groundSensor");
	if (!gameObjects.size()) return;
	this->character = gameObjects[0];

	// Inicial target should be current platform
	if (this->character != nullptr && this->character->transform != nullptr)
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
	checkNullAndBreak(target);
	checkNullAndBreak(target->transform);

	targetObject = target;
	setTarget(target->transform->getPosition());
}

void PlatformNavigation::setTarget(const PlatformNode& node)
{
	target = node;
}

bool PlatformNavigation::hasArrived() const
{
	if (character == nullptr || character->transform == nullptr || platformGraph == nullptr) return false;

	return target.getIndex() == platformGraph->getIndex(character->transform->getWorldPosition());
}

void PlatformNavigation::setFleeing(bool fleeing)
{
	this->fleeing = fleeing;
}

bool PlatformNavigation::isFleeing() const
{
	return fleeing;
}

std::vector<PlatformNavigation::PathNode> PlatformNavigation::getShortestPath()
{
	std::vector<PathNode> path;
	if (platformGraph == nullptr || character == nullptr || character->transform == nullptr)
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
	if (character == nullptr || character->transform == nullptr) return;

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
		GameObject* parent = character->getParent();
		if (parent != nullptr && parent ->transform != nullptr) {
			parent->transform->setRotation({ 0, 90.0 * linkInUse.getDirection(),0 });
			RigidBody* rb = parent->getComponent<RigidBody>();
			if (rb != nullptr) {
				rb->clearForces();
				rb->setLinearVelocity(linkInUse.getStartVelocity());
				rb->addForce(linkInUse.getStartForce());
			}
		}
		return;
	}

	ActionInput action = (startPos.x < characterPos.x) ? ActionInput::MOVE_LEFT : ActionInput::MOVE_RIGHT;
	if(stateMachine != nullptr)stateMachine->addActionInput(action);
}

void PlatformNavigation::moveToPlatform()
{
	if (character == nullptr) return;
	// If arrived to end of link, reset
	if (linkInUse.getDuration() < time || (character->transform != nullptr && (character->transform->getWorldPosition() - linkInUse.getEndPos()).magnitude() < 0.1)) {
		movingThroughLink = false;
		time = 0.0f;
		lastState = 0;
		if (platformGraph != nullptr && stateMachine != nullptr && platformGraph->getIndex(linkInUse.getEndPos()) == target.getIndex())
			((AIStateMachine*)stateMachine)->startPlatformMovement();
		return;
	}

	RigidBody* rb = character->getComponent<RigidBody>();
	Vector3 vel = rb != nullptr ? rb->getLinearVelocity() : Vector3::ZERO;
	std::vector<State> states = linkInUse.getStates();
	if (states.size() > 0) {
		bool processed = false;
		while (lastState >= 0 && lastState < states.size() && time >= states[lastState].getTime()) {
			// Inject input
			for (Action action : states[lastState].getActions())
				if (stateMachine != nullptr)
					stateMachine->addActionInput((ActionInput)action);
			lastState++;
			processed = true;
		}
		// Compensate time diference by injecting last state input again (unless was the last one)
		if (!processed && lastState - 1 >= 0 && lastState < states.size()) {
			for (Action action : states[lastState - 1].getActions())
				if (stateMachine != nullptr)
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
		if (stateMachine == nullptr) return;
		else if (fleeing)
		{
			fleeing = false;
			((AIStateMachine*)stateMachine)->changeTarget();
			((AIStateMachine*)stateMachine)->startPlatformNavigation();
		}
		else {
			((AIStateMachine*)stateMachine)->startPlatformMovement();
		}
	}
}
