#include "Node.h"


Node::Node(std::string n, Point& p) : name(n), point(p) {};
Node::Node(std::string n, float x, float y) : name(n), point(x, y) {}
Node::Node() :Node("", Point(INFINITY, INFINITY)) {}

Node::~Node() {
	edges.clear();
}

/**
Nodes are compared based on a name
*/
bool Node::operator==(const Node& other) const
{
	return (name == other.name);
}

