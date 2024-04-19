#pragma once
#include "Node.h"

class BHTree {
public:
	BHTree() { head = nullptr; };
	Node* GetHead() { return head; }; //woah
	void clear();
	void addNode(Node* n);

protected:
    void clearNode(Node* node);
	Node* head;
};

void BHTree::clear() {
    clearNode(head); 
    head = nullptr;  // Set the head to nullptr THIS IS WHAT I FORGOT TO DO FOR A WHILE
}

void BHTree::clearNode(Node* node) {
    if (node == nullptr) {
        return;
    }

    // clear child nodes
    clearNode(node->getFirst());
    clearNode(node->getSecond());
    clearNode(node->getThird());
    clearNode(node->getFourth());
    clearNode(node->getFifth());
    clearNode(node->getSixth());
    clearNode(node->getSeventh());
    clearNode(node->getEigth());

    // Clear links to child nodes
    node->removeFirst();
    node->removeSecond();
    node->removeThird();
    node->removeFourth();
    node->removeFifth();
    node->removeSixth();
    node->removeSeventh();
    node->removeEigth();
}

void BHTree::addNode(Node* n) {
	if (head == nullptr) {
		head = n;
		return;
	}
	head->InsertNode(n);
	return;
}
