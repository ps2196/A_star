#pragma once
/**
AUTHOR: Piotr S³ysz
VERSION: 1.0
DATE: May 2017

Implements algorithm for finding shortest path in given Graph

*/
#include <queue> // implements priority_queue
#include <set>
#include <functional>
#include <iostream> //  DEBUG
#include"Graph.h"
#include "Path.h"
#include "Heap.h"
#include "Heap.cpp"
#include "Visualization.h"

using namespace std;

/**

NodeCost is used in a priority_queue used in A* algorithm

*/
struct NodeCost {
	float cost; // cost + h(node)
	float rawCost; // cost of edges not including h(node)

	Node* node;

	NodeCost() : cost(-1), node(nullptr){}
	NodeCost(float cst, Node* n)
	{
		cost = cst;
		rawCost = 0;
		node = n;
	}

	bool operator==(const NodeCost& other) const
	{
		return (*node == *(other.node));
	}
	bool operator!=(const NodeCost& other) const
	{
		return !(*this == other);
	}
	// invert comparison operator to get minimizing heap
	bool operator<(const NodeCost& other) const
	{
		if (this->node->name == other.node->name) {
			return false;
		}
		// diffrent cities
		if (cost == other.cost) {
			return (node->name < other.node->name); // if costs are equla compare names lexicogrphically
		}
		else {
			return (cost < other.cost);
		}
	}
	bool operator<=(const NodeCost& other) const
	{
		return (cost < other.cost) || (*this == other);
	}
	bool operator>(const NodeCost& other) const
	{
		if (this->node->name == other.node->name)
			return false;
		// diffrent cities
		if (cost == other.cost)
			return (node->name > other.node->name); // if costs are equla compare names lexicogrphically
		return (cost > other.cost);
	}
	bool operator>=(const NodeCost& other) const
	{
		return (cost > other.cost) || (*this == other);
	}

	// Method required by heap
	float priority() const { return cost; }
	void setPriority(float np) { cost = np;}
	string key() const  { return node->name; }
};


/**
Implements path finding algorithm
*/
class PathFinder {
private:
	typedef unordered_map<Node*, float> NodeMap; // mapped value is smmalest known cost to Node
	typedef Heap<NodeCost> NodePriorityQ;
	typedef unordered_map<string, Path> PredecessorsMap;
	typedef vector<NodeCost> NodeVector;
	Graph* G;
	PredecessorsMap predecessors; // Predecessors map keyed on city name

	void setPredecessor(Node* keyCity, Node* predCity);
	Path recreatePath(const Node& source, const Node& target);
	float h(const Node& current, const Node& source); // heuristic funcion used by A*
public:
	PathFinder(Graph* graph);
	/**
	Finds shortest path from source to target
	*/
	~PathFinder(){
		predecessors.clear();
	}

	Path findShortestPath(Node& source, Node& target);
	/**
	Find shortest path using Dijkstra's algorithm.
	Parameters:
		source, target - source and target cities
	*/
	Path A_star(Node& source, Node& target);
	Path brute(Node& source, Node& target);

};
