#include "Path.h"
using namespace std;

Path::Path()
{
	path = list<Node*>();
	total_cost = 0.0;
}

Path::Path(float cost)
{
	total_cost = cost;
}

Path::Path(const NodeList& p, float cost)
{
	path = NodeList(p);
	total_cost = cost;
}
bool Path::isValid()
{
	return (total_cost >= 0);
}
Node* Path::pathSource()
{
	if (path.empty())
		return nullptr;
	return *(path.begin());
}

Node* Path::pathTarget()
{
	if (path.empty())
		return nullptr;

	NodeList::iterator end = path.end();
	end--;
	return *end;
}

Path::NodeList::iterator Path::begin()
{
	return path.begin();
}

Path::NodeList::iterator Path::end()
{
	return path.end();
}

/**
Merge this with other. Other path is added to the end of this total cost is updated.
*/
void Path::merge( Path& other)
{
	for (NodeList::iterator i = other.path.begin(); i != other.path.end(); ++i)
	{
		this->path.push_back(*i);
	}
	this->total_cost += other.total_cost;
}

/**
Merge this with reversed other
*/
void Path::reversedMerge(Path& other)
{
	if (other.path.empty())
		return;
	//for (CityList::iterator i = other.path.begin(); i != other.path.end(); ++i)
	NodeList::iterator i = other.path.end();
	i--;
	for (;i != other.path.begin(); --i)
	{
		this->path.push_front(*i);
	}
	this->path.push_front(*i);
	this->total_cost += other.total_cost;
}

void Path::reverse()
{
	path.reverse();
}