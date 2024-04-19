#pragma once



#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"shaderClass.h"
//#include"Sphere.h"
#include<vector>
#include"Sphere.h"
#include "BHTree.h"

class NBodyController {
public:
	NBodyController() {};
	~NBodyController() {// Deallocate memory for nodes
		for (auto node : nodes) {
			delete node;
		}
		nodes.clear();
	};
	void SpawnRandom(int numSpheres);
	void SpawnSingleRandom();
	void Spawn(glm::vec3 position);
	void runGravity(Node * head, Node * current, double deltaTime);
	glm::vec3 calculateGravity(Node* n, Node* other);
	float calculateDistance(Node* n, Node* other);
	void addSphere(Sphere* s); // galaxy.push_back(s); (old code)
	Node* getHead() { return(galaxy.GetHead()); };
	void rebuildTree();
private:
	std::vector<Node*> nodes; // this is what I am gonna substitute with a BH-Tree
	BHTree galaxy;
	void Move(double deltaTime, glm::vec3 force, Node* head);
	float tooFar = 0.0f; // lower this for faster but more innacurate simulation
	const float GRAVITY = .0000000000667;
};



void NBodyController::rebuildTree() {
	// Clean all the node connections
	//cleanTree(galaxy.GetHead()); 
	galaxy.clear();
	// Rebuild the tree
	for (auto i : nodes) {
		galaxy.addNode(i);
	}
}


void NBodyController::addSphere(Sphere* s) {
	Node* n = new Node(s); 
	galaxy.addNode(n);
	nodes.push_back(n);
}

glm::vec3 NBodyController::calculateGravity(Node* n, Node* other) {
	float dx = other->getPosition().x - n->getPosition().x;
	float dy = other->getPosition().y - n->getPosition().y;
	float dz = other->getPosition().z - n->getPosition().z;
	float sqrd_dist = pow(dx, 2) + pow(dy, 2) + pow(dz, 2);
	if (sqrd_dist == 0) {
		return(glm::vec3(0, 0, 0));
	}
	float force = GRAVITY * n->avgMass * other->avgMass / sqrd_dist;
	float force_x = force * dx / (pow(sqrd_dist, 1 / 2));
	float force_y = force * dy / (pow(sqrd_dist, 1 / 2));
	float force_z = force * dz / (pow(sqrd_dist, 1 / 2));
	return(glm::vec3(force_x, force_y, force_z));
}
float NBodyController::calculateDistance(Node* n, Node* other) {
	float dx = other->getPosition().x - n->getPosition().x;
	float dy = other->getPosition().y - n->getPosition().y;
	float dz = other->getPosition().z - n->getPosition().z;
	float sqrd_dist = pow(dx, 2) + pow(dy, 2) + pow(dz, 2);
	if (sqrd_dist == 0) {
		return(0);
	}
	float dist = pow(sqrd_dist, 1 / 2);
	return dist;
}
void NBodyController::runGravity(Node * head, Node * current, double deltaTime) {
	// Calculate amount of gravity to add
	// OLD CODE
	//for (int i = 0; i < galaxy.size(); i++) {
		// testing for two spheres
		// Switch this with formula F = g*m1*m2 / r^2
		// For now a crude implementation
		// basically I am just gonna find the differences between the two positions and add a velocity in those directions
	//}
	/*glm::mat4 diff = galaxy[0]->getModel() - galaxy[1]->getModel();
	diff = glm::scale(diff, glm::vec3(.000000001f, .000000001f, .000000001f));
	galaxy[1]->velocity += diff * static_cast<float>(deltaTime); 
	diff = galaxy[1]->getModel() - galaxy[0]->getModel();
	diff = glm::scale(diff, glm::vec3(.000000001f, .000000001f, .000000001f));
	galaxy[0]->velocity += diff * static_cast<float>(deltaTime);
	Move(deltaTime); */

	// NEW CODE
	// For this, we run the BH algorithm
	// We will need to calculate the gravity running on each node
	// There is a distance that is acceptable for each node...
	// I am using the tooFar variable I set arbitrarily. I guess we will see !


	if (head == nullptr || current==nullptr) {
		return;
	}
	// Only using the highest level nodes for gravity calculations (for now)
	Node* first = head->getFirst();
	Node* second = head->getSecond();
	Node* third = head->getThird();
	Node* fourth = head->getFourth();
	Node* fifth = head->getFifth();
	Node* sixth = head->getSixth();
	Node* seventh = head->getSeventh();
	Node* eigth = head->getEigth();	
	glm::vec3 totalForce = glm::vec3(0, 0, 0);
	if (first != nullptr) {
		float d = calculateDistance(current, first);
		//if (d > tooFar) { ADD BACK LATER
			totalForce += calculateGravity(current, first);
		//} // might need to go down tree here because currently we are staying at highest level and not going down, we only go down to do rest at highest level
		
		
		// Need to add other clause
	}
	if (second != nullptr) {
		float d = calculateDistance(current, second);
		//if (d > tooFar) {
			totalForce += calculateGravity(current, second);
		//}

	}
	if (third != nullptr) {
		float d = calculateDistance(current, third);
		//if (d > tooFar) {
			totalForce += calculateGravity(current, third);
		//}
	}
	if (fourth != nullptr) {
		float d = calculateDistance(current, fourth);
		//if (d > tooFar) {
			totalForce += calculateGravity(current, fourth);
		//}	
	}
	if (fifth != nullptr) {
		float d = calculateDistance(current, fifth);
		//if (d > tooFar) {
			totalForce += calculateGravity(current, fifth);
		//}
		

	}
	if (sixth != nullptr) {
		float d = calculateDistance(current, sixth);
		//if (d > tooFar) {
			totalForce += calculateGravity(current, sixth);
		//}
		

	}
	if (seventh != nullptr) {
		float d = calculateDistance(current, seventh);
		//if (d > tooFar) {
			totalForce += calculateGravity(current, seventh);
		//}
		

	}
	if (eigth != nullptr) {
		float d = calculateDistance(current, eigth);
		//if (d > tooFar) {
			totalForce += calculateGravity(current, eigth);
		//}
	}
	// Traverse the rest of tree and apply proper forces
	runGravity(head, current->getFirst(), deltaTime); // this originally recursively went down from head, it needs to instead go to the next current but also pass the head !!!!!ESRSRESRE
	runGravity(head, current->getSecond(), deltaTime);
	runGravity(head, current->getThird(), deltaTime);
	runGravity(head, current->getFourth(), deltaTime);
	runGravity(head, current->getFifth(), deltaTime);
	runGravity(head, current->getSixth(), deltaTime);
	runGravity(head, current->getSeventh(), deltaTime);
	runGravity(head, current->getEigth(), deltaTime);
	//Move(deltaTime, totalForce, head);
	//std::cout << "Total force: " << totalForce.x << " " << totalForce.y << " "<< totalForce.z << std::endl;
	Move(deltaTime, totalForce, current);

}
void NBodyController::Move(double deltaTime, glm::vec3 force, Node* head) {
	//for (int i = 0; i < galaxy.size(); i++) {
	//}
	// OLD CODE
	/*galaxy[1]->Move(galaxy[1]->velocity[3] * static_cast<float>(deltaTime));
	galaxy[0]->Move(galaxy[0]->velocity[3] * static_cast<float>(deltaTime)); */

	// NEW CODE
	// This works in my brain but not likely in practice
	// std::cout << force.x << force.y << force.z << std::endl;
	/// head->setPosition(head->getPosition() + force * static_cast<float>(deltaTime)); this dont work because we are moving it not setting it
	head->velocity += force * static_cast<float>(deltaTime);
	head->translatePosition(head->velocity); 

}

void NBodyController::SpawnRandom(int numSpheres) {
	return;
}
void NBodyController::SpawnSingleRandom() {
	return;
}
void NBodyController::Spawn(glm::vec3 position) {
	return;
}