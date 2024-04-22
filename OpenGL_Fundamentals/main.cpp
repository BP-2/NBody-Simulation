#include<filesystem>
namespace fs = std::filesystem;

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

//#include"Texture.h"
#include"shaderClass.h"
//#include"VAO.h"
//#include"VBO.h"
//#include"EBO.h"
#include"Camera.h"
#include<irrKlang.h>
#include"Line.h"
#include<assimp/Importer.hpp>
#include"Sphere.h"
#include"Model.h"
#include"NBodayController.h"

const unsigned int width = 800;
const unsigned int height = 800;
const int NUM_WORLDS = 50;
const int RESET_TREE = 500; // How many frames between tree rebuild

// register frame resize callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// function for processing input
void processInput(GLFWwindow* window);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void soundSet(irrklang::ISound* sound, float min, float max);
void makePlanet(glm::mat4 &model, unsigned int texture, irrklang::ISound* sound, glm::vec3 sunPosition, float rotationAmount, float dopplerVelocity, Shader myShader, glm::vec3 invisibleColor, bool pass);
void makeModel(glm::mat4& model, unsigned int texture, irrklang::ISound* sound, glm::vec3 sunPosition, float rotationAmount, float dopplerVelocity, Shader myShader, glm::vec3 invisibleColor, bool pass, Model ourModel);

void generateTexture(unsigned int texture, int width, int height, int nrChannels, const char path[]);
void drawCursor(Shader& shader);


// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 15.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 75.0f; // updated from 45
bool selected = false;


int main() {
	irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();

	if (!engine)
		return 0; // err check for irrklang



	engine->setListenerPosition(irrklang::vec3df(0, 0, 3.0f), irrklang::vec3df(0,0,1));

	// Loading in model data
	// Model ourModel("UFO_Empty.glb");


	glfwInit();
	// Basically, here we are setting the version of glfw to use (3.3 because learnOpenGL.com uses OpenGL 3.3) more info at https://www.glfw.org/docs/latest/window.html#window_hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Here we sett it to core profile, this is a small subset of OpenGL features.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Creating a window
	GLFWwindow* window = glfwCreateWindow(width, height, "Badass window", NULL, NULL);

	// Checking if window failed to create
	if (window == NULL)
	{
		std::cout << "The window creator failed man" << std::endl;
		glfwTerminate();
		return - 1;
	}

	// Make context (window)
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	// Checking glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Set viewport so OpenGL knows
	glViewport(0, 0, width, height);

	
	// Buffer handling
	Shader myShader("default.vert", "default.frag");
	// ** Buffers
	// NOTE* Usually when you have multiuple objects you wnat to draw, you first generate all VAOs and required VBO and attribute pointers, and store those for later user.
	// NOTE* When you want to draw, you will then take the corresponding VAO, bind it, then draw the object and unbind



	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f, // COORDINATES, TEXTURE COORDINATES, NORMALS
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,  0.0f,
	};



	// indices man
	unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
	};

	// Vertex Array Object, whenever we want to draw, we will bind VAO with correct settings before drawing
	unsigned int VAO;

	// Vertex buffer object
	unsigned int VBO;

	// Element buffer object (used for indexes so we do not reuse information)
	unsigned int EBO;
	// Generate VAO buffer
	glGenVertexArrays(1, &VAO);

	// We generate the buffer, we can bind to multiple buffer objects at same time as long as they have different types.
	glGenBuffers(1, &VBO);

	// generate Element buffer object
	glGenBuffers(1, &EBO);

	// 1. Bind vertex array object, this stores our vertex attribute configuration and which VBO to use
	glBindVertexArray(VAO);

	// 2. copy our vertices array in a buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Binding VBO to GL_ARRAY_BUFFER
	// This transfers user-defined data into the currently bound buffer. (The VBO is bound to GL_ARRAY_BUFFER, we have size argument, the argument, and how we want graphics card to manage the given data)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// 3. specify elements
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// IM GONNA EXPLAIN THIS IN DETAIL

	// First parameter specifies which vertex attribute to configure (attribute 0 in our layout of vertex shader)
	// Second specifies the size of the vertex attribute (vec3 so ut us composed of 3 values)
	// Third is data type
	// Fourth specifies if we want the data to be normalized. If we are inputing integer data, we would want this to be true.
	// Fifth parameter is the stride and details the space between consecutive attributes. We say 3 * sizeof(float) because we have 3 floats in each vec3
	// Sixth parameter is the offset of where this element started in the array. Since this is first element, this is just zero.
	// 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5*sizeof(float)));
	glEnableVertexAttribArray(3);


	// Unsure if any of this is necessary
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// You can unbind after this so other VAO's will not modify this.
	glBindVertexArray(0);



	// 3. specify elements
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Unsure if any of this is necessary
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// You can unbind after this so other VAO's will not modify this.
	glBindVertexArray(0);



	myShader.use();
	//myShader.setInt("texture1", 0);
	//myShader.setInt("texture2", 1);
	

	glm::mat4 modelSun = glm::mat4(1.0f); // sun

	glm::vec3 sunPos = glm::vec3(.0f, .0f, -20.0f);


	modelSun = glm::translate(modelSun, sunPos);
	// relative position of each of the planets
	


	myShader.setVec3("lightColor", glm::vec3(1.0f, 0.9f, 0.8f));
	
	// I gotta initialize these and genuinely no idea why
	glm::vec3 start = glm::vec3(10, 10, 0);
	glm::vec3 end = glm::vec3(20, 20, 0);

	start.x = .0f;
	start.y = -.02f;
	end.x = .0f;
	end.y = .02f;

	//0.0f, -0.02f, // Bottom point
	//	0.0f, 0.02f, // Top point
	//	// Horizontal line
	//	-0.02f, 0.0f, // Left point
	//	0.02f, 0.0f  // Right point

	Line lineVert(start, end);

	start.x = -.02f;
	start.y = .0f;
	end.x = .02f;
	end.y = .0f;
	Line lineHorz(start, end);
	Model particleModel("Sphere.obj");

	Sphere* myOrb = new Sphere();
	Sphere* myOtherOrb = new Sphere();
	Sphere* orb3 = new Sphere();
	Sphere* orb4 = new Sphere();
	Sphere* orb5 = new Sphere();
	Sphere* orb6 = new Sphere();
	Sphere* orb7 = new Sphere();




	// Appear to all be moving towards the final node added in the oct in their suboct
	myOrb->Move(glm::vec3(0.0f, 0.0f, -10.0f));
	myOtherOrb->Move(glm::vec3(0.0f, 10.0f, -10.0f));
	orb3->Move(glm::vec3(0.0f, 20.0f, -10.0f));
	orb4->Move(glm::vec3(5.0f, 8.0f, -10.0f));
	orb5->Move(glm::vec3(5.0f, 12.0f, -10.0f));
	orb6->Move(glm::vec3(5.0f, 12.0f, -18.0f));
	orb7->Move(glm::vec3(3.0f, 17.0f, 0.0f));




	NBodyController controller;
	controller.addSphere(myOrb);
	controller.addSphere(myOtherOrb);
	controller.addSphere(orb3);
	controller.addSphere(orb4);
	controller.addSphere(orb5);
	controller.addSphere(orb6);
	controller.addSphere(orb7);
	controller.SpawnRandom(NUM_WORLDS);
	int loopCounter = 0; // Keeping a counter to rebuild the tree every RESET_TREE amount of frames

	myShader.setVec3("lightColor", glm::vec3(.8f, .8f, .8f));
	myShader.setVec3("lightPos", glm::vec3(0, 0, -10));

	// render loop
	while (!glfwWindowShouldClose(window)) {
		if (loopCounter >= RESET_TREE) { // rebuil the tree
			loopCounter = 0;
			// rebuild tree
			controller.rebuildTree();
			std::cout << "TREE REBUILT >:D" << std::endl;
		}
		// Timing stuff
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Process our input (movement)
		processInput(window);
		// Setting color of screen
		glClearColor(.1f, .1f, .1f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		// Then we are clearing it with that set color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::vec3 invisColor = glm::vec3(0.0f, .0f, .0f); // for invisible render pass (not needed in this sim)

		if (selected) {
			// Read the color of the pixel at the center of the screen
			glReadBuffer(GL_COLOR_ATTACHMENT0);
			unsigned char pixel[3];
			glReadPixels(width / 2, height / 2, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
		}
		else {
			
			lineVert.draw();
			lineHorz.draw();
		}
		// rendering commands
		myShader.use();

		glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, -10.0f), cameraUp);
		myShader.setMat4("view", view);
		glm::mat4 projection = glm::mat4(1.0f); // initialize as identity to start
		projection = glm::perspective(glm::radians(fov), (float)width / (float)height, .1f, 100.0f);
		//unsigned int modelLoc = glGetUniformLocation(myShader.ID, "model");
		unsigned int viewLoc = glGetUniformLocation(myShader.ID, "view");
		//unsigned int transformLoc = glGetUniformLocation(myShader.ID, "transform");
		// 1. Uniform location
		// 2. Number of matrices to send
		// 3. Transpose the matrix?
		// 4. the actual matrix data
		//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		myShader.setMat4("projection", projection);
		//glBindVertexArray(VAO);

		// Takes the head and then the current, we start at head so we move from there
		controller.runGravity(controller.getHead(), controller.getHead(), deltaTime); 
		controller.draw(myShader);
		/*
		myOrb->Draw(myShader);
		myOtherOrb->Draw(myShader);
		orb3->Draw(myShader);
		orb4->Draw(myShader);
		orb5->Draw(myShader);
		orb6->Draw(myShader);
		orb7->Draw(myShader);
		*/
		// std::cout << "POSITION OF ORB 1:" << myOrb->getModel()[3].x << " " << myOrb->getModel()[3].y << " " << myOrb->getModel()[3].z << endl;
		// std::cout << "POSITION OF ORB 2:" << myOtherOrb->getModel()[3].x << " " << myOtherOrb->getModel()[3].y << " " << myOtherOrb->getModel()[3].z << endl;

		loopCounter++;
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	engine->drop();
	
	glfwTerminate(); // Close up shop
	return 0;
}
	

// for resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
// for processing input
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	float cameraSpeed = static_cast<float>(20.5 * deltaTime); // CAMERA SPEED
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		irrklang::ISoundEngine* mini_engine = irrklang::createIrrKlangDevice();
		irrklang::ISound* gunSound = mini_engine->play3D("laser-gun-81720.mp3", irrklang::vec3df(0, 0, -20.0f), false, false, true);
		gunSound->drop();
		//mini_engine->drop();
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		selected = !(selected);

}
// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}
//void soundSet(irrklang::ISound sound, float min, float max);

// Safe set the sound dimensions and error check
void soundSet(irrklang::ISound* sound, float min, float max) {
	if (sound) {
		sound->setMinDistance(min);
		sound->setMaxDistance(max);
	}
}

void makePlanet(glm::mat4& model, unsigned int texture, irrklang::ISound* sound, glm::vec3 sunPosition, float rotationAmount, float dopplerVelocity, Shader myShader, glm::vec3 invisibleColor, bool pass) {
	if (texture != NULL) {
		glBindTexture(GL_TEXTURE_2D, texture); // Planet texture
	}
	model = glm::translate(-sunPosition) * model; // applying translating to model two  (because right side)
	model = glm::rotate(glm::mat4(1.0f), rotationAmount, glm::vec3(0.0f, 1.0f, 0.0f)) * model;
	model = glm::translate(sunPosition) * model; // applying translating to model two  (because right side)
	glm::vec3 modelPosition = glm::vec3(model[3]); // Extract translation component
	
	myShader.setMat4("model", model);
	myShader.setVec3("viewPos", cameraPos);
	myShader.setVec3("material.ambient", glm::vec3(.8f, .5f, .31f));
	myShader.setVec3("material.diffuse", glm::vec3(.8f, .5f, .31f));
	myShader.setVec3("material.specular", glm::vec3(.5f, .5f, .5f));
	myShader.setVec3("invisColor", invisibleColor);

	// COLORFUL GALAXY
	/*myShader.setVec3("material.ambient", glm::vec3(sin(glfwGetTime() * 2.0f) , sin(glfwGetTime() * 0.7f), sin(glfwGetTime() * 1.3f)));
	myShader.setVec3("material.diffuse", glm::vec3(sin(glfwGetTime() * 2.0f) , sin(glfwGetTime() * 0.7f), sin(glfwGetTime() * 1.3f)));
	myShader.setVec3("material.specular", glm::vec3(.5f, .5f, .5f));*/

	myShader.setFloat("material.shininess", 32.0f);
	myShader.setBool("isLight", false);
	
	myShader.setBool("pass", pass);
	
	
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void makeModel(glm::mat4& model, unsigned int texture, irrklang::ISound* sound, glm::vec3 sunPosition, float rotationAmount, float dopplerVelocity, Shader myShader, glm::vec3 invisibleColor, bool pass, Model ourModel) {
	glBindTexture(GL_TEXTURE_2D, texture); // Planet texture

	model = glm::translate(-sunPosition) * model; // applying translating to model two  (because right side)
	model = glm::rotate(glm::mat4(1.0f), rotationAmount, glm::vec3(0.0f, 1.0f, 0.0f)) * model;
	model = glm::translate(sunPosition) * model; // applying translating to model two  (because right side)
	glm::vec3 modelPosition = glm::vec3(model[3]); // Extract translation component
	sound->setPosition(irrklang::vec3df(modelPosition.x, modelPosition.y, modelPosition.z)); // updating the sound position
	sound->setVelocity(irrklang::vec3df(0, dopplerVelocity, 0));
	myShader.setMat4("model", model);
	myShader.setVec3("viewPos", cameraPos);
	myShader.setVec3("material.ambient", glm::vec3(.8f, .5f, .31f));
	myShader.setVec3("material.diffuse", glm::vec3(.8f, .5f, .31f));
	myShader.setVec3("material.specular", glm::vec3(.5f, .5f, .5f));
	myShader.setVec3("invisColor", invisibleColor);

	// COLORFUL GALAXY
	/*myShader.setVec3("material.ambient", glm::vec3(sin(glfwGetTime() * 2.0f) , sin(glfwGetTime() * 0.7f), sin(glfwGetTime() * 1.3f)));
	myShader.setVec3("material.diffuse", glm::vec3(sin(glfwGetTime() * 2.0f) , sin(glfwGetTime() * 0.7f), sin(glfwGetTime() * 1.3f)));
	myShader.setVec3("material.specular", glm::vec3(.5f, .5f, .5f));*/

	myShader.setFloat("material.shininess", 32.0f);
	myShader.setBool("isLight", false);
	if (pass) {
		myShader.setBool("pass", pass);
	}

	ourModel.Draw(myShader);
}


void generateTexture(unsigned int texture, int width, int height, int nrChannels, const char path[]) {
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char*  data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data) { // err check
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D); // automaticlly generates all the required mipmaps for the currently bound texture
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void drawCursor(Shader& shader) {
	// Set the color of the cursor lines using a uniform in the shader
	shader.use();
	shader.setVec3("cursorColor", glm::vec3(1.0f, 1.0f, 1.0f)); // Set cursor color to white
	shader.setBool("isCursor", true);
	// Vertices for the cursor lines
	float cursorVertices[] = {
		// Vertical line
		0.0f, -0.02f, // Bottom point
		0.0f,  0.02f, // Top point
		// Horizontal line
	   -0.02f,  0.0f, // Left point
		0.02f,  0.0f  // Right point
	};

	// Create and bind a VAO
	GLuint cursorVAO;
	glGenVertexArrays(1, &cursorVAO);
	glBindVertexArray(cursorVAO);

	// Create a VBO for cursor vertices
	GLuint cursorVBO;
	glGenBuffers(1, &cursorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cursorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cursorVertices), cursorVertices, GL_STATIC_DRAW);

	// Set the vertex attribute pointers
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Draw the cursor lines
	glDrawArrays(GL_LINES, 0, 4);

	shader.setBool("isCursor", false);

	// Unbind VAO and VBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Don't forget to delete VAO and VBO if they're no longer needed
	glDeleteVertexArrays(1, &cursorVAO);
	glDeleteBuffers(1, &cursorVBO);
}