#include "Graph.h"

Graph :: Graph(int n) {
//	adjList = new vector < list<pair<int, int>> > (n);
}
void Graph :: InsertEdge ( int x, int y, int dist ) {
	//*adjList[x].push_back(make_pair(y,dist));
	//*adjList[y].push_back(make_pair(x,dist));
}
void Graph :: StoreNodeMap ( unordered_map<int, Node *> &map ) {
	mymap = map;
	staticmap = map;
}
void Graph :: CreateGraph () {
	for ( auto it : staticmap ) {
		it.second->IncreaseKey(INFINITY);
		heap.Insert(it.second);
	}
}
list<Node *> Graph :: Dijkstra ( int src, int dest, vector < list<pair<int, int>> > & adjList ) {
	Node * adjVertex;
	bool isCompletePath = false;
	int weight= 0, newWeight = 0;
	Node * nodePtr;
	list<Node *> completePath;
	Path.clear();
	mymap = staticmap;
	CreateGraph();
	auto myit = mymap.find(src);
	if ( myit == mymap.end() ) {
		cout << "source doesn't exist in list" ;
		exit(1);
	}
	nodePtr = myit->second;
	heap.DecreaseKey(nodePtr, (Node *)NULL, 0);
	for ( auto & it : adjList[nodePtr->GetNodeID()] ) {
		myit = mymap.find(it.first);
		weight = it.second;
		if (myit == mymap.end() ) {
			cout << " Logical error occurred!!! Plz check given graph!!\n" << it.first << " vertext doesn't exist in map\n";
			exit(1);
		}
		adjVertex = myit->second;
		newWeight = nodePtr->GetNodeDist () + weight;
		heap.DecreaseKey ( adjVertex, nodePtr, newWeight );
	}
	myit = mymap.find(dest);
	if ( myit == mymap.end() ) {
		isCompletePath = true;
		//cout << "destination doesn't exist in list" ;
		//exit(1);
	}
	mymap.erase(src);
	while ( !heap.IsEmpty() ) {
		nodePtr = heap.DeleteMin();
		completePath.push_back(nodePtr);
		if (nodePtr->GetNodeID() == dest){
			//cout << nodePtr->GetNodeDist() << endl;
			Node * dummy;
			while (!heap.IsEmpty() )
				dummy = heap.DeleteMin();
			break;
		}
		if (nodePtr->GetNodeID() == src)
			continue;
		for ( auto & it : adjList[nodePtr->GetNodeID()] ) {
			myit = mymap.find(it.first);
			weight = it.second;
			if (myit == mymap.end() ) {
				continue;
			}
			adjVertex = myit->second;
			newWeight = nodePtr->GetNodeDist () + weight;
			heap.DecreaseKey ( adjVertex, nodePtr, newWeight );
		}
		mymap.erase(nodePtr->GetNodeID());
	}
	if ( isCompletePath ) 
		return(completePath);

	while ( nodePtr != NULL ) {
        	Path.push_front(nodePtr);
		nodePtr=nodePtr->GetExtNode();
	}
	return(Path);
}		
