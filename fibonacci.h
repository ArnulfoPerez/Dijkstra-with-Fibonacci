#ifndef __FIBO_H__
#define __FIBO_H__
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <unordered_map>

#define MAX_RANK 10000
#define INFINITY 99999999

using namespace std;

struct Contain {
	int vertex_id;
    int distance;
	char ip_add[16];
};

typedef struct Contain Data;

class Node {
    Data _data;            //Data of the node
    bool _childCut;        //Has lost child or not
    int _rank;             //Number of child nodes 
	bool visited;
	//int _nodesUnder;
    Node * _parent;        //Parent pointer
    Node * _left;          //Left sibling 
    Node * _right;         //right sibling
    Node * _child;         //Child list Pointer
	Node * _extendedFrom;
public:
    Node (); 
    Node (Data & data); 

    bool operator < ( const Node & rhs ) const;
    bool operator > ( const Node & rhs ) const;
    bool operator <=( const Node & rhs ) const;
    bool operator >=( const Node & rhs ) const;
	bool IsSameRank ( Node * ptrNode );
	bool HaveSibling ();
    bool IsMarked ( ) ; 

	int GetRank ();
	int GetNodeID();
	int GetNodeDist();
	Node * GetExtNode () {
		return(_extendedFrom);
	}

	void IncreaseKey ( int dist ) {
		_data.distance = dist;
	}
    void MarkNode ( ) ; 
	void SetData (Data & data);
	void SetParent ( Node * ptrNode );
	void SetLeft   ( Node * ptrNode );
	void SetRight  ( Node * ptrNode );
	void RemoveSelfFromList ( );
	void InsertIntoList ( Node * ptrNode );
	void MakeMyChild    ( Node * ptrNode );
	void CascadeCut     ( Node * headPtr );
	void ClearParentsFromList (Node * node);
	//void Delete ();

	Data * GetData ();
	Node * DeleteMin ();
	Node * GetNextNode ( );
	Node * GetNext ();
	Node * DecreaseKey    ( Node * headPtr, Node * extendedFrom, int distance );
	friend void MergeTwoLists ( Node * leftList, Node * rightList ) ;
	friend int NodesInList ( Node * node);
	friend Node * MeldOperation ( Node * headPtr );
	friend Node* GetMinPointerOfList ( Node * headPtr );
	friend void SwapTwoNodesInList ( Node * lhs, Node * rhs );
};

class FHeap {
    Node * _head;

public:
    FHeap();
	void CreateHeap ( Data & data );
	void DecreaseKey( Node * node, Node * srcNode, int distance );
	void Insert ( Data & data ) ;
	void Insert ( Node * node ) ;
	bool IsEmpty();
	Node * DeleteMin ( );
};
#endif
