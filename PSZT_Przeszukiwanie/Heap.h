#pragma once
/**
AUTHOR: Piotr S³ysz
VERSION: 1.0
DATE: May 2017

A heap structure which allows for priority update
*/
#ifndef HEAP_H
#define HEAP_H 
#include <set>
#include <unordered_map>


using namespace std;

template <class T>
class Heap
{
	unordered_map<string, float> priorityMap;
	set<T> heap;
public:
	typedef typename set<T>::iterator iterator;
	Heap() {}
	~Heap();
	void push(const T& elem);
	const T& top() const;
	void pop();
	bool empty() { return heap.empty(); }
	iterator begin() { return heap.begin(); }
	iterator end() { return heap.end(); }
	iterator find(const T& elem) { return heap.find(elem); }
	std::pair<bool, float> includes(const T& elem);
	void updatePriority(const T& oldVal, const T& newVal);
};

#endif // !HEAP_H
