#pragma once
#include "Sphere.h"

class Node {
public:
	Node();
	void InsertNode(Node* n);
	glm::vec3 getPosition();
private:
	Sphere* orb;
	Node* oct1 = nullptr; // -x, -y, -z
	Node* oct2 = nullptr; // -x, -y, z
	Node* oct3 = nullptr; // -x y, -z
	Node* oct4 = nullptr; // -x, y, z
	Node* oct5 = nullptr; // x , -y, -z
	Node* oct6 = nullptr; // x, -y, z
	Node* oct7 = nullptr; // x, y, -z
	Node* oct8 = nullptr; // x, y, z
};

// This just makes an octree right now which appears to be the root of the BH-tree.
// Need to see how weighting and distances play into calculations. But for now, BH-tree.
void Node::InsertNode(Node* n) {
	// Using the position, we will find where the node lands in our tree
	glm::vec3 ownPosition = getPosition();
	glm::vec3 newPosition = n->getPosition();
	if (ownPosition.x < newPosition.x) {
		if (ownPosition.y < newPosition.y) {
			if (ownPosition.z < newPosition.z) {
				if (oct1 != nullptr) { // -x, -y, -z
					oct1 = n;
				}
				else {
					oct1->InsertNode(n);
				}
			}
			else {
				if (oct2 != nullptr) { // -x, -y, z
					oct2 = n;
				}
				else {
					oct2->InsertNode(n);
				}
			}
		}
		else {
			if (ownPosition.z < newPosition.z) {
				if (oct3 != nullptr) { // -x, y, -z
					oct3 = n;
				}
				else {
					oct3->InsertNode(n);
				}
			}
			else {
				if (oct4 != nullptr) { // -x, y, z
					oct4 = n;
				}
				else {
					oct4->InsertNode(n);
				}
			}

		}
	}
	else {
		if (ownPosition.y < newPosition.y) {
			if (ownPosition.z < newPosition.z) {
				if (oct5 != nullptr) { // x, -y, -z
					oct5 = n;
				}
				else {
					oct5->InsertNode(n);
				}
			}
			else {
				if (oct6 != nullptr) { // x, -y, z
					oct6 = n;
				}
				else {
					oct6->InsertNode(n);
				}
			}
		}
		else {
			if (ownPosition.z < newPosition.z) {
				if (oct7 != nullptr) { // x, y, -z
					oct7 = n;
				}
				else {
					oct7->InsertNode(n);
				}
			}
			else {
				if (oct8 != nullptr) { // x, y, z
					oct8 = n;
				}
				else {
					oct8->InsertNode(n);
				}
			}

		}
	}
}
glm::vec3 Node::getPosition() {
	return(glm::vec3(orb->getModel()[3]));
}