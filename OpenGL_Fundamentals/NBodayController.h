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

class NBodyController {
public:
	NBodyController() {};
	void SpawnRandom(int numSpheres);
	void SpawnSingleRandom();
	void Spawn(glm::vec3 position);
	void runGravity(double deltaTime);
	void addSphere(Sphere* s) { galaxy.push_back(s); };
private:
	std::vector<Sphere*> galaxy; // this is what I am gonna substitute with a BH-Tree

	void Move(double deltaTime);
};

void NBodyController::runGravity(double deltaTime) {
	// Calculate amount of gravity to add
	//for (int i = 0; i < galaxy.size(); i++) {
		// testing for two spheres
		// Switch this with formula F = g*m1*m2 / r^2
		// For now a crude implementation
		// basically I am just gonna find the differences between the two positions and add a velocity in those directions
	//}
	glm::mat4 diff = galaxy[0]->getModel() - galaxy[1]->getModel();
	diff = glm::scale(diff, glm::vec3(.000000001f, .000000001f, .000000001f));
	galaxy[1]->velocity += diff * static_cast<float>(deltaTime); 
	diff = galaxy[1]->getModel() - galaxy[0]->getModel();
	diff = glm::scale(diff, glm::vec3(.000000001f, .000000001f, .000000001f));
	galaxy[0]->velocity += diff * static_cast<float>(deltaTime);
	Move(deltaTime);
	
}
void NBodyController::Move(double deltaTime) {
	//for (int i = 0; i < galaxy.size(); i++) {
	//}
	galaxy[1]->Move(galaxy[1]->velocity[3] * static_cast<float>(deltaTime));
	galaxy[0]->Move(galaxy[0]->velocity[3] * static_cast<float>(deltaTime));
}