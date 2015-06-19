#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <iostream>
#include <cstdio>
#include <cstring>
#include "fibonacci.h"
#include <list>
#include <vector>
#include <unordered_map>

using namespace std;

class Graph {
	unordered_map<int, Node *> mymap;
	unordered_map<int, Node *> staticmap;
	FHeap heap;
	//vector < list<pair<int, int>> > * adjList;
	list <Node *> Path;
public:
	Graph(int n);
	void InsertEdge ( int x, int y, int dist );
	void StoreNodeMap ( unordered_map<int, Node *> &map ); 
	void CreateGraph ();
	list<Node *> Dijkstra ( int src, int dest, vector < list<pair<int, int>> > & adjList );
};
#endif
