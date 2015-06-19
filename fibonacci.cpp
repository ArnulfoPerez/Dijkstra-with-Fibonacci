#include "fibonacci.h"

Node :: Node () 
{
	_childCut = false;
	_rank = 0;
	_parent = NULL;
	_child = NULL;
	visited = false;
}   

Node :: Node ( Data & data )
{
	//Node ();
	_childCut = false;
	_rank = 0;
	_parent = NULL;
	_child = NULL;
	visited = false;
	_left = _right = this;
	_data = data;
}

//Returns vertex ID of the node
int Node :: GetNodeID ( ) 
{
	return ( _data.vertex_id );
}

//Returns current distance of the node
int Node :: GetNodeDist ( ) 
{
	return ( _data.distance );
}

//Overloaded functions for node object comparison
bool Node :: operator < ( const Node & rhs ) const 
{
	return ( _data.distance < rhs._data.distance );
}
bool Node :: operator > ( const Node & rhs ) const 
{
	return ( _data.distance > rhs._data.distance );
}
bool Node :: operator <= ( const Node & rhs ) const 
{
	return ( _data.distance <= rhs._data.distance );
}
bool Node :: operator >= ( const Node & rhs ) const 
{
	return ( _data.distance >= rhs._data.distance );
}

//Child cut related methods
//Marks childCut of the node to true
void Node :: MarkNode ( ) 
{
	_childCut = true;
}
//Returns child cut value of the node
bool Node :: IsMarked ( ) 
{
	return _childCut;
}

//Returns current rank of the node
int Node :: GetRank ( ) 
{
	return _rank;
}

//getter and setter methods for Node
Data * Node :: GetData ( ) 
{
	return (&_data);
}
void Node :: SetData ( Data & data ) 
{
	_data = data;
}

//Functions to remove a node from root list 

//Sets the node pointer passed in as argument to the left of current node
void Node :: SetLeft ( Node * ptrNode ) 
{
	_left = ptrNode;
}

//Sets the node pointer passed in as argument to the right of current node
void Node :: SetRight ( Node * ptrNode ) 
{
	_right = ptrNode;
}

//Checks the left pointer of the node and returns the value of if the node have
//sibling or not
bool Node :: HaveSibling ( ) 
{
	return ( _left != this ? true:false);
}

//Removes the given node from the list by
//setting _left node to point to _right node and vice versa
//setting own _left and _right pointers to null
//no operation done on the _parent and _child pointers expected to be 
//handled by calling routine
void Node :: RemoveSelfFromList ( ) 
{
	bool haveSibling = HaveSibling();
	//if node have sibling detach it from list
	if ( haveSibling ) {
		_right -> SetLeft  ( _left );
		_left  -> SetRight ( _right );
	}

	//Complete routine by setting _left and _right pointers to self
	_left = _right = this;
}

//Merging two nodes... probably same rank
//Checks if the two nodes in question have same rank or not
//Useful in case of one node becomes child of other
bool Node :: IsSameRank ( Node * ptrNode ) 
{
	return ( _rank == ptrNode -> _rank  );
}

//Set ptrNode as parent of the current node
void Node :: SetParent ( Node * ptrNode ) 
{
	_parent = ptrNode ;
}

//Inserts ptrNode in the right of current node in circular linked list
//Mostly called with headPtr so the node is inserted to the right of headPtr
void Node :: InsertIntoList ( Node * ptrNode ) 
{
	ptrNode -> SetRight ( this->_right );
	ptrNode -> SetLeft  ( this );
	this->_right -> SetLeft ( ptrNode );
	this->_right = ptrNode ;
}

//Makes ptrNode as child of current node by
//First remove ptrNode from the circular linked list it belongs to
//Insert ptrNode into the circular doubly linked list pointed by _child pointer
//Increase the rank of the current node by 1 as one more child is attached
void Node :: MakeMyChild ( Node * ptrNode ) 
{
	if (this == ptrNode)
		return; 
	//Fibonacci heap expects two nodes getting merged are of the 
	//same rank.... if rank is different user is alearted for any logical 
	//error that might have occured
	if ( ! IsSameRank (ptrNode) ) {
		cout << "Probable mistake in merging not of the same rank" << endl;
	}
	ptrNode -> RemoveSelfFromList ();
	if ( _child != NULL ) {
		_child  -> InsertIntoList ( ptrNode );
	} else {
		_child = ptrNode;
	}
	ptrNode -> SetParent ( this );
	_rank += 1; //Number of children increased by 1
}

//Check if heap property will be preserved on decrease key if yes decrease key and return true else return false
//on violation follow steps
//Remove itself and put in root list marking childCut to false
//Go to parent for cascade cut check
Node * Node :: DecreaseKey ( Node * headPtr, Node * extendedFrom, int distance ) 
{
	bool isDecreasedValueMin = false;
	if ( _data.distance == INFINITY )
		_data.distance = distance;
	else if ( _data.distance < distance )
		return(headPtr);

	_data.distance = distance;
	_extendedFrom = extendedFrom;
	if ( *this < *headPtr ) {
		isDecreasedValueMin = true;
	}
	if ( _parent != NULL && _data.distance < _parent->_data.distance ) {
		if (_parent->_child == this)
			_parent->_child = _parent->_child->GetNextNode();
		_parent -> CascadeCut ( headPtr );
		RemoveSelfFromList ();
		headPtr->InsertIntoList ( this );
		_parent = NULL;
		_childCut = false;
	}
	if ( isDecreasedValueMin ) 
		headPtr = this;
	if (headPtr == NULL){
		exit(1);
	}
	return headPtr;
}

//Checks if _childCut is true
//	No: Set value of _childCut to true and return
//	Yes: RemoveNode from list, insert it into root list and call CascadeCut on parent
void Node :: CascadeCut ( Node * headPtr )
{
	if ( _parent != NULL && _childCut == true ) {
		_parent -> CascadeCut ( headPtr );
	} else if ( _parent == NULL ) {
		_childCut = false;
		_rank -= 1;
		return ;
	} else if ( _childCut == false ) {
		_childCut = true;
		_rank -= 1;
		return;
	}

	if (_parent->_child == this)
		_parent->_child = _parent->_child->GetNextNode();
	RemoveSelfFromList ();
	MergeTwoLists ( headPtr, this );
	_rank -= 1;
	_childCut = false;
	_parent = NULL;
}

Node * Node :: GetNextNode () 
{
	return ( _left == this ? NULL: _left );
}

//This function clears parent pointers of the whole list
//Used for the case where child list is merged in root list
//in DeleteMin operation
void Node :: ClearParentsFromList (Node * node) 
{
	while (node->_parent != NULL) {
		node->_parent = NULL;
		node->_childCut = false;
		node = node->_left;
	}
}

//Removes the node pointed by headPtr and return to the calling routine
//In addition following steps are follwed
//Circular linked list pointed by _child pointer is merged to root list
//New min is located 
//Melding operation is performed on the root list
Node * Node :: DeleteMin () 
{
	Node * childPtr, * leftPtr, * listStart;
	childPtr = _child;
	leftPtr = this -> GetNextNode();
	RemoveSelfFromList ();
	_child = NULL;
	if ( childPtr != NULL && leftPtr != NULL ) {
		ClearParentsFromList(childPtr);
		MergeTwoLists(childPtr, leftPtr);
		listStart = childPtr;
	} else if ( childPtr != NULL ) {
		ClearParentsFromList(childPtr);
		listStart = childPtr;
	} else if (leftPtr != NULL ) {
		listStart = leftPtr;
	} else {
		fflush(stdout);
		return (NULL); //Queue is empty now.
	}
	listStart = GetMinPointerOfList ( listStart );
	listStart = MeldOperation (listStart );
	return(listStart);
}

//Merge given leftList and rightList 
void MergeTwoLists ( Node * leftList, Node * rightList ) 
{
	Node * leftStart, *leftEnd, *rightStart, *rightEnd;
	int lcount, rcount, mcount;
	//lcount = NodesInList ( leftList );
	//rcount = NodesInList ( rightList );
	leftStart = leftList;
	leftEnd = leftList -> _left;
	rightStart = rightList;
	rightEnd = rightList -> _left;
	leftStart -> _left = rightEnd;
	rightEnd -> _right = leftStart;
	leftEnd -> _right = rightStart;
	rightStart -> _left = leftEnd;
	//mcount = NodesInList ( leftList );
	//if ( mcount != ( lcount + rcount ) ) {
		//cout << "Problem in MergeTwoLists\n";
	//}
}

//Debugging function used to count the nodes in the list
int NodesInList ( Node * node ) 
{
	int i = 1;
	if (node == NULL )
		return 0;
	Node * ptrToStop;
	ptrToStop = node->_left;
	while ( node != ptrToStop ) {
		++i;
		node = node->GetNext();
	}
	return i;
}

//Checks if the node have right sibling 
// YES: returns right sibling pointer
// NO: returns NULL
Node * Node::GetNext() {
	return(_right == this? NULL:_right);
}

//This traverses the given list and checks for the same ranked nodes and melds them
Node * MeldOperation ( Node * headPtr ) 
{
	Node * iterator, * big, * small, *ptrToStop;
	unordered_map <int, Node *> rankMatrix;
	iterator = headPtr;
	//Assign pointer to stop left of headPtr and walk along the right path
	//Once the right path meets left sibling all nodes have taken care of
	ptrToStop = iterator->_left;
	while ( iterator != ptrToStop ) {
		auto fnd = rankMatrix.find(iterator->_rank);
		if ( fnd == rankMatrix.end() ) {
			rankMatrix[iterator->_rank] = iterator;
			iterator = iterator->GetNext();
		} else {
			//assume that already visited element is big, if not we will switch
			big = fnd->second;
			small = iterator;
			//Always keep smaller node on the left so that traversing loop end condition becomes easy and efficient
			if ( *big < *small ) {
				SwapTwoNodesInList(small, big);
				small = fnd->second;
				big = iterator;
				//Sanity Check
				if ( *big < *small )
					printf("something went horribly wrong!!\n");
			}
			iterator = small;
			rankMatrix.erase(iterator->_rank);
			small -> MakeMyChild ( big );
		}
	}

	auto fnd = rankMatrix.find(iterator->_rank);
	//This is a cleanUp operation where we check if the node last two same ranked node if remaining are merged as well
	while ( fnd != rankMatrix.end() ) {
		big = fnd->second;
		small = iterator;
		if ( *big < *small ) {
			SwapTwoNodesInList(small, big);
			small = fnd->second;
			big = iterator;
			//Sanity Check
			if ( *big < *small )
				printf("something went horribly wrong!!\n");
		}
		iterator = small;
		rankMatrix.erase(iterator->_rank);
		small -> MakeMyChild ( big );
		fnd = rankMatrix.find(small->_rank);
	}
	//BugFound: Return the minimum node from the root list
	return( GetMinPointerOfList ( iterator ));
}
FHeap :: FHeap () 
{
	_head = NULL;
}

void FHeap :: Insert ( Data & data ) 
{
	Node * node = new Node ( data );
	if ( _head == NULL ) {
		_head = node;
	} else {
		_head -> InsertIntoList ( node );
		if ( *node < *_head ) {
			_head = node;
		}
	}
}

void FHeap :: Insert ( Node * node ) 
{
	if ( _head == NULL ) {
		_head = node;
	} else {
		_head -> InsertIntoList ( node );
		if ( *node < *_head ) {
			_head = node;
		}
	}
}

Node * FHeap :: DeleteMin ( ) 
{
	Node * toReturn;
	toReturn = _head;
	_head = _head -> DeleteMin ();
	//return (toReturn -> GetData() );
	return (toReturn);
}

bool FHeap :: IsEmpty () 
{
	return(_head==NULL? true: false);
}

void FHeap :: DecreaseKey ( Node * nodePtr, Node * srcNode, int newd ) {
	_head = nodePtr -> DecreaseKey ( _head, srcNode, newd);
}

void FHeap :: CreateHeap ( Data & data ) 
{ 
    data.distance = 0;
    Node * head = new (std::nothrow) Node (); 
    head -> SetData (data);
}   

//Friend function to swap positions of two nodes in list helps to 
// keep track of loop ending positions by always keeping smaller element to left
void SwapTwoNodesInList ( Node * lhs, Node * rhs ) 
{
	Node * lhsL, *lhsR, *rhsL, *rhsR;
	lhsL = lhs->_left;
	rhsL = rhs->_left;
	lhsR = lhs->_right;
	rhsR = rhs->_right;

	if ( lhsR == rhs && rhsL == lhs ) {
		// LHSL = LHS = RHS = RHSR
		lhsL->_right = rhs;
		lhs->_left = rhs;
		lhs->_right = rhsR;
		rhs->_left = lhsL;
		rhs->_right = lhs;
		rhsR->_left = lhs;
	} else if ( lhsL == rhs && rhsR == lhs ) {
		//RHSL = RHS = LHS = LHSR
		rhsL->_right = lhs;
		rhs->_left = lhs;
		rhs->_right = lhsR;
		lhs->_left = rhsL;
		lhs->_right = rhs;
		lhsR->_left = rhs;
	} else {
		//RHSL = RHS = RHSR ==== ... ==== LHSL = LHS = LHSR
		//LHSL = LHS = LHSR ==== ... ==== RHSL = RHS = RHSR
		//Rearrange lhs's left and right pointer to point to rhsL and rhsR
		lhs->_left = rhsL;
		lhs->_right = rhsR;

		//Rearrange rhs's left and right pointer to point to lhsL and lhsR
		rhs->_left = lhsL;
		rhs->_right = lhsR;

		//Change lhsL and lhsR to point to rhs node
		lhsL->_right = rhs;
		lhsR->_left = rhs;

		//Change rhsL and rhsR to point to lhs node
		rhsL->_right = lhs;
		rhsR->_left = lhs;
	}
}

//Traverses doubly circular linked list and gets the minimum element in the list
//Complexity O(n)
Node* GetMinPointerOfList ( Node * headPtr )
{
	Node * curMin, *iterator;
	curMin = headPtr;
	iterator = headPtr->_left;

	while ( iterator != headPtr ) {
		if (*curMin > *iterator ) {
			curMin = iterator;
		}
		iterator = iterator->_left;
	}
	return(curMin);
}
