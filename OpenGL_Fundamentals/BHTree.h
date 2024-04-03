#pragma once
#include "Sphere.h"
#include "Node.h"
class BHTree {
	public:
		BHTree();
		Node* GetHead() { return head; }; //woah
	protected:
		Node* head;

}

