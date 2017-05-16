#define GLEW_STATIC
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include "Shader.h"
#include "Camera.h"
#include "Object.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

//Instantiate with Release x86!

//Key detection functions
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//Variables
const GLuint WIDTH = 800;
const GLuint HEIGHT = 600;
GLfloat FOV = 45.0f;
float cameraSpeedV = 5.f;
bool keys[1024];
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = HEIGHT / 2.0f;
bool firstMouse = true;
GLint controls = 1;

//Camera variables to pass to view matrix
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//Objects vectors to define in constructor
vec3 scaleCube = vec3(0.3f, 0.3f, 0.3f);
vec3 rotationCube = vec3(0.f, 45.f, 0.f);
vec3 positionCube = vec3(1.5f, -1.0f, -1.5f);

vec3 scaleLamp = vec3(0.1f, 0.1f, 0.1f);
vec3 rotationLamp = vec3(0.f, 0.f, 0.f);
vec3 positionLamp = vec3(-1.0f, 1.5f, -1.0f);

//Camera speed control variables (deltaTime)
GLfloat deltaTime = 0.0f;	//Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	//Time of last frame

//Function to detect whenever a key is pressed and which one is it
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){

	//If that checks if a key is pressed or released
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;

	//Closing the window
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	//Lamp and cube controls
	if (key == GLFW_KEY_1 && action == GLFW_PRESS && controls == 2)
		controls = 1;

	if (key == GLFW_KEY_2 && action == GLFW_PRESS && controls == 1)
		controls = 2;
}

//Function that detects what is pressed and moves the camera alongside user movements
void do_movement() {

	GLfloat cameraSpeed = cameraSpeedV * deltaTime; //Camera speed calculus

	//Movement control
	if (keys[GLFW_KEY_W])camera.ProcessKeyboard(FORWARD, deltaTime); //Go forward

	if (keys[GLFW_KEY_S])camera.ProcessKeyboard(BACKWARD, deltaTime); //Go backward

	if (keys[GLFW_KEY_A])camera.ProcessKeyboard(LEFT, deltaTime); //Go left

	if (keys[GLFW_KEY_D])camera.ProcessKeyboard(RIGHT, deltaTime); //Go right

}

//Function that detects whenever a mouse button is pressed and the coordenates of the mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos){

	//Correct initialization of the mouse position for the first time
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	//Offset calculations
	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset); //Pass the offset calculations to the camera movement function
}

//Function that detects the mouse scroll to zoom in and out
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) { camera.ProcessMouseScroll(yoffset); }

int main(){

	//GLFW Initialization
	glfwInit();

	//Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//Create a GLFWwindow object
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "A_ParallaxMapping_MarcMolina_AniolFolch", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	//Set callback functions and input mode
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Glew Initialization and activation of experimental glew
	glewExperimental = GL_TRUE;
	glewInit();

	//Viewport
	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);  //Z-buffer enabler

	//Shader declaration
	Shader lightingShader = Shader("./src/lightVertexShader.vertexshader", "./src/lightFragmentShader.fragmentshader");
	Shader lampShader = Shader("./src/lampVertexShader.vertexshader", "./src/lampFragmentShader.fragmentshader");
	Object Cube = Object(scaleCube, rotationCube, positionCube, Object::cube);
	Object Lamp = Object(scaleLamp, rotationLamp, positionLamp, Object::cube);

	//Shader vertex
	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
	 	 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f
	};

	//VAO cube and VBO declaration and creation
	GLuint VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	//Position attributes for the cube
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//Creation of VAO of the light
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Just need to bind it

	//Vertex attributes for the lamp
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window)){

		//Deltatime calculation for each frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Events checker
		glfwPollEvents();
		do_movement();
		if (controls == 1) { Cube.do_movement(window); }
		else { Lamp.do_movement(window); }

		//Background color and z-buffer corrector
		glClearColor(0.f, 0.f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Getting uniform variables from the shaders and declaring cube attributes
		lightingShader.USE(); //Usage of the shader

		//Light and camera position initialization
		vec3 lightPos = Lamp.GetPosition();
		vec3 camPosition = camera.Position;

		//Variables and uniform from shaders
		GLint objectColorLoc = glGetUniformLocation(lightingShader.Program, "objectColor");
		GLint lightColorLoc = glGetUniformLocation(lightingShader.Program, "lightColor");
		GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "lightPos");
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(objectColorLoc, 1.0f, 0.8f, 0.6f);
		glUniform3f(lightColorLoc, 0.8f, 0.2f, 0.2f);
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc, camPosition.x, camPosition.y, camPosition.z);

		//Camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		//Getting uniform from shaders for the camera
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		//Passing the matrix to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//Drawing the cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(Cube.GetModelMatrix()));
		Cube.Draw();

		//Drawing the lamp
		lampShader.USE();

		//Variables and uniform from shaders
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(Lamp.GetModelMatrix()));

		Lamp.Draw();

		//Movement checkers
		if (controls == 1) { Cube.check_movement(deltaTime); }
		else { Lamp.check_movement(deltaTime); }

		//Swap the screen buffers
		glfwSwapBuffers(window);
	}

	//Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}
	

