#include "PlatformNode.h"

PlatformNode::PlatformNode() : iniPos(Vector3()), endPos(Vector3())
{
	edges = std::vector<NavigationLink>();
}

PlatformNode::PlatformNode(const Vector3& iniPos) : iniPos(iniPos), endPos(iniPos)
{
	edges = std::vector<NavigationLink>();
}

PlatformNode::PlatformNode(const Vector3& iniPos, const Vector3& endPos) : iniPos(iniPos), endPos(endPos)
{
	edges = std::vector<NavigationLink>();
}

PlatformNode::~PlatformNode()
{
	edges.clear();
}

void PlatformNode::setEnd(const Vector3& endPos)
{
	this->endPos = endPos;
}

void PlatformNode::setBegining(const Vector3& iniPos)
{
	this->iniPos = iniPos;
}

Vector3 PlatformNode::getBegining() const
{
	return iniPos;
}

Vector3 PlatformNode::getEnd() const
{
	return endPos;
}

void PlatformNode::addEdge(const NavigationLink& link)
{
	edges.push_back(link);
}

std::vector<NavigationLink> PlatformNode::getEdges()
{
	return edges;
}


//STATE
State::State() : pos(Vector3()), velocity(Vector3()), force(Vector3()), action(Action::None)
{

}

State::State(const Vector3& pos, const Vector3& velocity, const Vector3& force, Action action) : pos(pos), velocity(velocity), force(force), action(action)
{
}

//NAVIGATION LINK
NavigationLink::NavigationLink(const std::vector<State>& states, int connection) : linkStates(states), connection(connection)
{
}

int NavigationLink::getConnection() const
{
	return connection;
}
