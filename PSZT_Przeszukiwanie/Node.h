#pragma once
/**
AUTHOR: Piotr S³ysz
VERSION: 1.0
DATE: May 2017

Grahps node, node is identified by name, it contains point and list of edges incident with this.
*/

#include <string>
#include <vector>
#include <cmath>
#include "Edge.h"

class Edge;
struct Point {
	float x;
	float y;
	
	Point() { x = INFINITY; y = INFINITY; }
	Point(float x_coord, float y_coord)
	{
		x = x_coord;
		y = y_coord;
	}

	Point(const Point& other)
	{//copy constructor
		x = other.x;
		y = other.y;
	}
	/**
	Returns straight line distance between this and other
	*/
	float distance(const Point& other) const
	{
		float dx = other.x - x;
		float dy = other.y - y;
		float d = sqrt( (dx*dx) + (dy*dy) );
		return d;
	}
};

class Node
{
public:
	typedef std::vector<Edge> EdgeVect;
	std::string name;
	Point point;
	EdgeVect edges; // Edges incident with this node
	
	Node();
	Node(std::string n, Point& p);
	Node(std::string n, float x, float y);
	~Node();
	bool operator==(const Node& other) const;
	float distance(const Node& other) const { return point.distance(other.point); }
};
