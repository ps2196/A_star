#include "Heap.h"

template<class T>
Heap<T>::~Heap()
{
	priorityMap.clear();
	heap.clear();
}

/**
Inserts an elem on heap
*/
template<class T>
void Heap<T>::push(const T& elem)
{
	priorityMap[elem.key()] = elem.priority();
	heap.insert(elem);
}


/**
Returns element on top of heap without removing it
*/
template<class T>
const T& Heap<T>::top() const
{
	return *(heap.begin());
}

/**
Removes element on top of the stack aka minimal/maximal element
*/
template<class T>
void  Heap<T>::pop()
{
	priorityMap.erase(top().key());
	heap.erase(heap.begin());
}


/**
Returns a pair<bool,int>. First element is true if elem is in the heap, second is elements priority.
If element is not in the heap (false, MAX_INT) is returned.
This is const time but does not return the position of elem.
*/
template<class T>
std::pair<bool, float> Heap<T>::includes(const T& elem)
{
	std::pair<bool, float> res;
	auto it = priorityMap.find(elem.key());
	if (it != priorityMap.end())
	{
		res.first = true;
		res.second = it->second;
	}
	else
	{//element not in the heap
		res.first = false;
		res.second = INFINITY;
	}
	return res;
}

/**
Replaces old value on heap with new value
*/
template<class T>
void Heap<T>::updatePriority(const T& oldVal, const T& newVal)
{
	auto it = heap.begin();	
	while( it!=heap.end() && *it != oldVal)  //  CityCost::== compares objects based on cities, ignoring cost
	{
		it++;
	}

	if (it != heap.end())
	{// it points to oldValue in the heap
		heap.erase(it);
		priorityMap.erase(oldVal.key());
		push(newVal);
	}
}