#pragma once
/**
AUTHOR: Piotr S³ysz
VERSION: 1.0
DATE: May 2017

Path object stores ordered list of Nodes and total cost of traversing the path.
*/

#ifndef PATH_H
#define PATH_H

#include <list> 
#include "Node.h"

using namespace std;

class Node;

struct Path
{
public:
	typedef std::list<Node*>NodeList;
	NodeList path;
	float total_cost;
	
	Path();
	Path(float cost);
	Path(const NodeList& p, float cost);
	~Path() { path.clear(); }
	Node* pathSource();
	Node* pathTarget();
	void merge(Path& other);
	void reversedMerge(Path& other);
	bool isValid();
	void reverse();
	NodeList::iterator begin();
	NodeList::iterator end();
};

#endif // !PATH_H
