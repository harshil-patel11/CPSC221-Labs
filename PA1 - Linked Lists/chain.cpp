#include "chain.h"
#include <cmath>
#include <iostream>
#include <limits>
#include "block.h"

using namespace std;


// PA1 functions

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain() {
  clear();
}

/**
 * Inserts a new node after the node pointed to by p in the
 * chain (so p->next is the new node) and returns a pointer to
 * the newly created node.
 * If p is NULL, inserts a new head node to the chain.
 * This function **SHOULD** create a new Node and increase length_.
 *
 * @param p = The new node should be pointed to by p->next.
 *            If p is NULL, the new node becomes the head of the chain.
 * @param ndata = The data to be inserted.
 */
Chain::Node * Chain::insertAfter(Node * p, const Block &ndata) {
  Node* toInsert = new Node(ndata);
  if (p == NULL) {
    head_ = toInsert;
    toInsert->next = NULL;
    toInsert->prev = NULL;
    length_++;
    return toInsert;
  }

  Node *q = p->next;

  toInsert->next = p->next;
  toInsert->prev = p;
  if (q != NULL && q->prev != NULL) {
    q->prev = toInsert;
  }
  p->next = toInsert;
  
  length_++;
  return toInsert;  
}

/**
 * Swaps the position in the chain of the two nodes pointed to
 * by p and q.
 * If p or q is NULL or p==q, do nothing.
 * Change the chain's head pointer if necessary.
 */
void Chain::swap(Node *p, Node *q) {
  if (p == NULL || q == NULL || p==q) {
    return;
  }

  Node *prevP = p->prev;
  Node *nextP = p->next;
  Node *prevQ = q->prev;
  Node *nextQ = q->next; 

  if (head_ == p) {
    head_ = q;
  } else if (head_ == q) {
    head_ = p;
  }

  if (prevP != NULL) prevP->next = q;
  if (prevQ != NULL) prevQ->next = p;
  if (nextP != NULL) nextP->prev = q;
  if (nextQ != NULL) nextQ->prev = p;

  Node* tempNextP = p->next;
  p->next = q->next;
  q->next = tempNextP;
  Node *tempPrevP = p->prev;
  p->prev = q->prev;
  q->prev = tempPrevP;

  // p->next = nextQ;
  // p->prev = prevQ;
  // q->next = nextP;
  // q->prev = prevP;

  // if (prevP != NULL) prevP->next = q;
  // if (nextP != NULL) nextP->prev = q;
  // if (prevQ != NULL) prevQ->next = p;
  // if (nextQ != NULL) nextQ->prev = p;


  // Node *tempQ = q;
  // q = p;
  // p = tempQ; 

}

/**
 * Destroys all dynamically allocated memory associated with the
 * current Chain class.
 */
void Chain::clear() {
  
  while(head_ != NULL) {
    Node *tempHead = head_;
    head_ = head_->next;

    //delete &tempHead->data;
    delete tempHead;
    tempHead = NULL;
    length_--;
  }

}

/**
 * Makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */
void Chain::copy(Chain const &other) {
  if (other.head_ == NULL) {
    head_ = NULL;
    return;
  }

  length_ = other.length_;
  
  Node *curr = other.head_;
  Node *head = new Node(other.head_->data);
  head_ = head;

  Node *p = head_;
  curr = curr->next;

  while (curr != NULL) {
    Node *n = new Node(curr->data);
    p->next = n;
    n->prev = p;
    n->next = NULL;

    p = p->next;
    curr = curr->next;
  }

}

/* Modifies the current chain: 
 * 1) Find the node with the first (leftmost) block in the unscrambled
 *    image and move it to the head of the chain.
 *	This block is the one whose closest match (to the left) is the
 *	largest.  That is, the distance (using distanceTo) to this block 
 *	is big for all other blocks.
 *	For each block B, find the distanceTo B from every other block
 *	and take the minimum of these distances as B's "value".
 *	Choose the block B with the maximum value over all blocks and
 *	swap its node to the head of the chain.
 *
 * 2) Starting with the (just found) first block B, find the node with
 *    the block that is the closest match to follow B (to the right)
 *    among the remaining blocks, move (swap) it to follow B's node,
 *    then repeat to unscramble the chain/image.
 */
void Chain::unscramble() {
  vector<double> distArr;
  vector<Node*> nodeArr;  
  Node *B = head_;

  while(B != NULL) {
    double minDist = std::numeric_limits<double>::max();
    Node *A = head_;
    
    while(A != NULL) {
      if (A != B) {
        double currDist = A->data.distanceTo(B->data);
        if (currDist < minDist) {
          minDist = currDist;
        }
      }
      A = A->next;
    }
    distArr.push_back(minDist);
    nodeArr.push_back(B);
    B = B->next;
  }

  double maxDist = std::numeric_limits<double>::min();
  unsigned long maxDex = 0;
  for (unsigned long i = 0; i < distArr.size(); i++) {
    if (distArr[i] > maxDist) {
      maxDist = distArr[i];
      maxDex = i;
    }
  }

  swap(head_, nodeArr[maxDex]);

  Node *h = head_;
  while (h->next != NULL) {
    double minDist = std::numeric_limits<double>::max();
    Node *curr = h->next;
    Node *minNode = NULL;
    
    while(curr != NULL) {
      double currDist = h->data.distanceTo(curr->data);
      if (currDist < minDist) {
          minDist = currDist;
          minNode = curr;
      }
      curr = curr->next;
    }
    if (h->next != minNode && minNode != NULL) {
      swap(h->next, minNode);
    }
    h = h->next;
  }
}
