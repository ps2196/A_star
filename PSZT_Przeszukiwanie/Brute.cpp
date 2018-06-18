#include "Path.h"
#include "PathFinder.h"


Path PathFinder::brute(Node& source, Node& target)
{
	NodeMap visitedNodes; //  unordered_map<City*, int> mapped value is smallest known cost from source to key
	NodeVector openNodes;


	int iterations = 0;
	float minDistance = FLT_MAX;
	NodeCost src = NodeCost(0, &source);

	openNodes.push_back(src);
	NodeCost currentNC = src;
	while (!openNodes.empty())
	{
		iterations++;
		currentNC = openNodes.front();
		if (currentNC.node == &target)
		{
			openNodes.erase(openNodes.begin());
			visitedNodes[currentNC.node] = minDistance;
			continue;
		}
		openNodes.erase(openNodes.begin()); //  pop only removes top element without returning it
		Node* node = currentNC.node; //  currently processed node
		float cost = currentNC.rawCost; // known cost to current node
		

		typedef Node::EdgeVect::iterator EdgeVecIter;
		for (EdgeVecIter i = node->edges.begin(); i != node->edges.end(); ++i)
		{
			cout << "------------ NEXT EDGE ------------" << endl;
			Edge edge = *i;
			Node* otherNode = edge.otherNode(node);;
			if (otherNode == nullptr)
				continue;
			/*if (otherNode == &target && ((cost + edge.cost()) < minDistance))
			{
				minDistance = cost + edge.cost();
				cout << "Mindistance from:" << otherNode->name << "equals: " << minDistance << endl;
			}*/

			if (visitedNodes.count(otherNode) != 1)
			{// other city was not visited yet
				float otherCost = cost + edge.cost();
				NodeCost otherNC = NodeCost(otherCost, otherNode);
				otherNC.rawCost = cost + edge.cost();
				cout << cost + edge.cost() << endl;
				if (otherNode == &target && ((cost + edge.cost()) < minDistance))
				{
					minDistance = cost + edge.cost();
					cout << "Mindistance from:" << otherNode->name << "equals: " << minDistance << endl;
				}
				std::pair<bool, float> includesRes(false, otherNC.cost);
				if (std::find(openNodes.begin(), openNodes.end(), otherNC) != openNodes.end())
				{
					cout << "OtherNC.cost = " << otherNC.cost << endl;
					includesRes.first = true;	
				}
				if (includesRes.first && (includesRes.second > otherNC.cost))
				{// other is present in openCities but with higher cost - update entry in openCities
					cout << cost + edge.cost() << endl;
					NodeCost oldNC(includesRes.second, otherNC.node);
					setPredecessor(otherNode, node);
				}
				else if (!includesRes.first) {// other city not in Q insert it with current cost
					openNodes.push_back(otherNC);
					setPredecessor(otherNode, node);
				}

			}
		}
		visitedNodes[node] = cost; // mark city as visited

	}

	if (minDistance != FLT_MAX)
	{
		Path& resultPath = recreatePath(source, target);
		resultPath.total_cost = minDistance;
		cout << "Iterations: " << iterations << endl;
		return resultPath;
	}
	//path not found within cost limit, or doesnt exist at all
	return Path(-1); // return a dummy path
}
