#include "Edge.h"

Edge::Edge(Node* n1, Node* n2, float f)
{
	node1 = n1;
	node2 = n2;
	fee = f;
}
bool Edge::operator==(const Edge& Edge) const
{
	return (((node1 == Edge.node1) && (node2 == Edge.node2)) ||
		((node1 == Edge.node2) && (node2 == Edge.node1)));
}

Node* Edge::otherNode(Node* node) const
{
	if (node == nullptr)
		return nullptr;
	if (*node == *node1)
		return node2;
	else if (*node == *node2)
		return node1;
	else
		return nullptr;
}

float Edge::cost()
{
	return fee;
}