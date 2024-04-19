#ifndef SPHERE_H
#define SPHERE_H
#include <vector>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include "Model.h"
#include "shaderClass.h"

class Sphere
{
public:
	Sphere();
	Sphere(glm::vec3 setPosition); 
	void Draw(Shader &myShader); // function to draw
	void Scale(int scaler); // function to scale
	void Move(glm::vec3 movement);
	void setMass(float g) { localMass = g; };
	glm::mat4 getModel() { return matrix; };
	glm::mat4  velocity = glm::mat4(1.0f); 
	float localMass = 300.0f; // the amount of mass this sphere has

private:
	Model particleModel;
	glm::mat4 matrix;
	glm::vec3 position;

};

Sphere::Sphere() {
	particleModel = Model ("Sphere.stl"); // load in model using model class
	position = glm::vec3(0.0f, 0.0f, 0.0f); // default position
	matrix = glm::mat4(1.0f); // identify matrix
}
Sphere::Sphere(glm::vec3 setPosition) {
	particleModel = Model("Sphere.stl"); // load in model using model class
	position = setPosition;
	matrix = glm::mat4(1.0f); // identify matrix
	matrix = glm::translate(matrix, position);
}
void Sphere::Draw(Shader &myShader) {
	myShader.setMat4("model", matrix);
	particleModel.Draw(myShader);
}
void Sphere::Scale(int scaler) {
	matrix = glm::scale(matrix, glm::vec3(scaler, scaler, scaler));
}
void Sphere::Move(glm::vec3 movement) {
	matrix = glm::translate(matrix, movement);
}
#endif // SPHERE_H