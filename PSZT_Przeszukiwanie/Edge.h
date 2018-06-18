#pragma once
/**
AUTHOR: Piotr S³ysz
VERSION: 1.0
DATE: May 2017

Edge in the graph, constains pointers to nodes at both ends
*/
#include <utility>
#include <unordered_map>
#include "Node.h"

class Node;
using namespace std;
class Edge 
{
public:
	float fee; //cost of traversing this edge
	Node *node1;
	Node *node2;

	Edge(Node* n1, Node* n2, float f);
	
	bool operator==(const Edge& edge) const;

	/**
		Given a Node returns pointer to Node on the other end of this Edge
	*/
	Node* otherNode(Node* node) const;

	float cost();
};

