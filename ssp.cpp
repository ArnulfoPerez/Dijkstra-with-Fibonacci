#include <iostream>
#include <cstdio>
#include <vector>
#include <list>
#include "fibonacci.h"
#include "Graph.h"

using namespace std;
int main (int argc, char *argv[])
{
	char *line= NULL; 
	size_t size =100;
	int n, m;
	list <Node *> Path;
	Node * adjVertex = NULL;
	int weight= 0, newWeight = 0;
	unordered_map<int, Node *> mymap; //Maps node pointer list in consideration
	//List of adj vertext and its distance from the current vertex
	int x, y, dist;
	Data data;
	FHeap heap;
	Node * nodePtr;

	if ( argc != 4 ) {
		cout << "Usage: ssp file_name source_node dest_nod" << endl;
		return 1;
	}

	FILE * fp;
	int source, destination;

	source = atoi (argv[2]);
	destination = atoi (argv[3]);

	fp = fopen ( argv[1], "r");
	if ( fp == NULL) {
		printf ( " File %s doesn't exist!!!\n" , argv[1]);
		return 1;
	}

	fscanf (fp, "%d %d\n", &n, &m);
	vector < list<pair<int, int>> > adjList(n);

	//printf ("There are %d nodes and %d edges in the graph\nProceed to create the graph\n", n, m);
	data.distance = INFINITY;
	for ( int i = 0; i < n; i++ ) {
		data.vertex_id = i;
		nodePtr = new(std::nothrow) Node(data);
		if (nodePtr == NULL)
			cout << "Bad allocation\n";
		mymap[i] = nodePtr;
		//heap.Insert(nodePtr);
	}
	Graph graph(n);
	graph.StoreNodeMap ( mymap );

	while ( !feof(fp) ) {
		fscanf (fp, "%d %d %d\n", &x, &y, &dist);
		adjList[x].push_back(make_pair(y,dist));
		adjList[y].push_back(make_pair(x,dist));
	}

	Path = graph.Dijkstra(source, destination, adjList);
	auto xad = Path.rbegin();
	cout << (*xad)->GetNodeDist() << endl;
	for (auto &it : Path ) {
		cout << it->GetNodeID() << " ";
	}
	cout << endl;

	return 0;
}
