#pragma once
#include "Sphere.h"
#include <random> // for random velocity seeding

class Node {
public:
	Node() {};
	Node(Sphere* orby);
	void InsertNode(Node* n);
	void translatePosition(glm::vec3 translate);
	glm::vec3 getPosition();
	glm::vec3 velocity = glm::vec3(0,0,0);
	float getLocalMass() { return(orb->localMass); };
	float avgMass = 200.0f; // ideally this might also include a direction if a subsection is heavily weighted but lets try this
	// glm::vec3 getAvgPosition(); Might not actually need this because this point is acting as the central position
	Node* getFirst() { return oct1; };
	Node* getSecond() { return oct2; };
	Node* getThird() { return oct3; };
	Node* getFourth() { return oct4; };
	Node* getFifth() { return oct5; };
	Node* getSixth() { return oct6; };
	Node* getSeventh() { return oct7; };
	Node* getEigth() { return oct8; };

	void removeFirst() { oct1 = nullptr; };
	void removeSecond() { oct2 = nullptr; };
	void removeThird() { oct3 = nullptr; };
	void removeFourth() { oct4 = nullptr; };
	void removeFifth() { oct5 = nullptr; };
	void removeSixth() { oct6 = nullptr; };
	void removeSeventh() { oct7 = nullptr; };
	void removeEigth() { oct8 = nullptr; };


private:
	//glm::vec3 avgPosition = orb->getModel()[3];
	Sphere* orb = nullptr;
	Node* oct1 = nullptr; // -x, -y, -z
	Node* oct2 = nullptr; // -x, -y, z
	Node* oct3 = nullptr; // -x y, -z
	Node* oct4 = nullptr; // -x, y, z
	Node* oct5 = nullptr; // x , -y, -z
	Node* oct6 = nullptr; // x, -y, z
	Node* oct7 = nullptr; // x, y, -z
	Node* oct8 = nullptr; // x, y, z
};

Node::Node(Sphere* orby) {
	orb = orby;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> distribution(-.00001f, .00001f);
	avgMass = orb->localMass;
	// Set random velocity
	velocity.x = distribution(gen);
	velocity.y = distribution(gen);
	velocity.z = distribution(gen);
}

void Node::translatePosition(glm::vec3 translate) { orb->Move(translate); };
// This just makes an octree right now which appears to be the root of the BH-tree.
// Need to see how weighting and distances play into calculations. But for now, BH-tree.
void Node::InsertNode(Node* n) {
	// Using the position, we will find where the node lands in our tree
	glm::vec3 ownPosition = getPosition();
	glm::vec3 newPosition = n->getPosition();
	avgMass += n->getLocalMass();
	if (ownPosition.x < newPosition.x) {
		if (ownPosition.y < newPosition.y) {
			if (ownPosition.z < newPosition.z) {
				if (oct1 == nullptr) { // -x, -y, -z
					oct1 = n;
					//std::cout << "OCT1" << endl;					std::cout << "SUBOCT" << endl;

				}
				else {
					oct1->InsertNode(n);
				}
			}
			else {
				if (oct2 == nullptr) { // -x, -y, z
					oct2 = n;
					//std::cout << "OCT2" << endl;
				}
				else {
					oct2->InsertNode(n);					//std::cout << "SUBOCT" << endl;

				}
			}
		}
		else {
			if (ownPosition.z < newPosition.z) {
				if (oct3 == nullptr) { // -x, y, -z
					oct3 = n;
					//std::cout << "OCT3" << endl;
				}
				else {
					oct3->InsertNode(n);					//std::cout << "SUBOCT" << endl;

				}
			}
			else {
				if (oct4 == nullptr) { // -x, y, z
					oct4 = n;
					//std::cout << "OCT4" << endl;
				}
				else {
					oct4->InsertNode(n);					//std::cout << "SUBOCT" << endl;

				}
			}

		}
	}
	else {
		if (ownPosition.y < newPosition.y) {
			if (ownPosition.z < newPosition.z) {
				if (oct5 == nullptr) { // x, -y, -z
					//std::cout << "OCT5" << endl;
					oct5 = n;
				}
				else {
					oct5->InsertNode(n);					//std::cout << "SUBOCT" << endl;

				}
			}
			else {
				if (oct6 == nullptr) { // x, -y, z
					//std::cout << "OCT6" << endl;
					oct6 = n;
				}
				else {
					oct6->InsertNode(n);					//std::cout << "SUBOCT" << endl;

				}
			}
		}
		else {
			if (ownPosition.z < newPosition.z) {
				if (oct7 == nullptr) { // x, y, -z
					//std::cout << "OCT7" << endl;
					oct7 = n;
				}
				else {
					oct7->InsertNode(n);					//std::cout << "SUBOCT" << endl;

				}
			}
			else {
				if (oct8 == nullptr) { // x, y, z
					//std::cout << "OCT8" << endl;
					oct8 = n;
				}
				else {
					oct8->InsertNode(n);
					//std::cout << "SUBOCT" << endl;
				}
			}

		}
	}
}
glm::vec3 Node::getPosition() {
	return(glm::vec3(orb->getModel()[3]));
}