#pragma once
#include "PathFinder.h"

PathFinder::PathFinder(Graph* graph)
{
	G = graph;
}

/**
Heuristic function used by A*.
Returns staight line distance between current and source.
*/
float PathFinder::h(const Node& current, const Node& source)
{
	return current.distance(source);
}

/**
Finds shortest path from source to target
*/
Path PathFinder::findShortestPath(Node& source, Node& target)
{
	return A_star(source, target);
}
/**
Find shortest path using A* algorithm.
Parameters:
source, target - source and target cities
*/
Path PathFinder::A_star(Node& source, Node& target)
{
	NodeMap visitedNodes; //  unordered_map<City*, int> mapped value is smallest known cost from source to key
	NodePriorityQ openNodes;

	NodeCost src = NodeCost(0, &source);

	openNodes.push(src);
	NodeCost currentNC = src;
	while ((currentNC.node != &target) &&
		(!openNodes.empty()))
	{
		currentNC = openNodes.top();
		openNodes.pop(); //  pop only removes top element without returning it
		Node* node = currentNC.node; //  currently processed node
		float cost = currentNC.rawCost; // known cost to current node
	
		if (currentNC.node == &target)
		{
			break; // we are done after this iteration, dont bother with checking paths from target forward
		}

		typedef Node::EdgeVect::iterator EdgeVecIter;
		for (EdgeVecIter i = node->edges.begin(); i != node->edges.end(); ++i)
		{
			Edge edge = *i;
			Node* otherNode = edge.otherNode(node);;
			if (otherNode == nullptr)
				continue;

			if (visitedNodes.count(otherNode) != 1) 
			{// other city was not visited yet
				float otherCost = cost + edge.cost() + h(*node, target);				
				NodeCost otherNC = NodeCost(otherCost, otherNode);
				otherNC.rawCost = cost + edge.cost();
				
				std::pair<bool, float> includesRes = openNodes.includes(otherNC);//includes returns parir (included?, priority/cost)
				if ( includesRes.first && (includesRes.second > otherNC.cost ))
				{// other is present in openCities but with higher cost - update entry in openCities heap
					NodeCost oldNC(includesRes.second, otherNC.node);
					openNodes.updatePriority(oldNC, otherNC);
					setPredecessor(otherNode, node);
				}
				else if (!includesRes.first){// other city not in Q insert it with current cost
					openNodes.push(otherNC);
					setPredecessor(otherNode, node);
				}
			}
		}
		visitedNodes[node] = cost; // mark city as visited
	}
	if (currentNC.node == &target)
	{// Path to target found, return it
		Path& resultPath = recreatePath(source, target);
		resultPath.total_cost = currentNC.rawCost;
		return resultPath;
	}
	//path not found within cost limit, or doesnt exist at all
	return Path(-1); // return a dummy path
}

Path PathFinder::recreatePath(const Node& source, const Node& target)
{
	Path resultPath;
	Node *currentNode = (Node*) &target;
	resultPath.path.push_back(currentNode);
	while (currentNode != nullptr && currentNode != &source)
	{
		Path pred = predecessors[currentNode->name];
		resultPath.reversedMerge(pred);
		currentNode = predecessors[currentNode->name].pathSource();
	}
	return resultPath;
}

/**
Adds entry to predecessors map 
*/
void PathFinder::setPredecessor(Node* keyNode, Node* predNode)
{
	if(predecessors.count(keyNode->name) != 0)
		predecessors[keyNode->name].path.clear();

	predecessors[keyNode->name].path.push_back(predNode);
}
