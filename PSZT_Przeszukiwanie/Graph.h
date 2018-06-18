#pragma once
/**
AUTHOR: Piotr S³ysz
VERSION: 1.0
DATE: May 2017

Node-Edge network representation.
*/
#include <unordered_set>
#include <unordered_map>
#include <queue> // implements priority_queue
//#include<iostream>
#include <fstream>
#include <string>
#include "Node.h"
#include "Edge.h"
//#include "EdgeHasher.h"
#include "StringSplitter.h"



using namespace std;

class Graph
{
public:
	typedef unordered_map<string, Node> NodeMap;
	//typedef unordered_set<Edge, EdgeHash> EdgeSet;

	NodeMap nodes;// hash map keyed on Node name
	//EdgeSet Edges;

	~Graph()
	{
		nodes.clear();
		//edges.clear();
	}
	Graph() {}

	Graph(NodeMap v)
	{
		nodes = v;
	}
	/**
		Constructor method reads Graph definition from file.
		
		Parameters:
			FNAME - path to  file with graph definition

		Expected file format:
			|nodes| lines with nodes definitions, format:
				NODE_NAME;NODE_X;NODE_Y
			|edges| lines with edges definition, format: 
				NODE1_NAME;Node2_NAME;FEE;

	*/
	Graph(string fname)
	{

		ifstream instream(fname);
		try
		{
			string line;
			int line_counter= 0;
			//Cities
			while (getline(instream, line))
			{
				line_counter++;
				if (line.empty()) { break; } // move to Edges
		
				vector<string> name_xy = splitWithDelimiter(line, ';');
				if (name_xy.size() != 3)
				{
					string msg = "Incorret file format, line: " + to_string(line_counter);
					throw exception(msg.c_str());
				}
				Node n(name_xy[0], stof(name_xy[1]),stof(name_xy[2]));
				nodes.insert({ name_xy[0], n });
			}
			//Edges
			while (getline(instream, line))
			{
				line_counter++;
				vector<string> params = splitWithDelimiter(line, ';');
				if (params.size() != 3)
				{
					string msg = "Incorret file format, line: " + to_string(line_counter);
					throw exception(msg.c_str());
				}
				Node& node1 = nodes[params[0]];
				Node& node2 = nodes[params[1]];
				Edge edge(&node1, &node2, stof(params[2]));
				
				//Edges.insert(edge);
				node1.edges.push_back(edge);
				node2.edges.push_back(edge);
			}

			instream.close(); // close input file
		}
		catch (exception ex)
		{
			instream.close();
			throw ex;
		}
	}

};