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

/*
vec3 scaleLamp = vec3(0.1f, 0.1f, 0.1f);
vec3 rotationLamp = vec3(0.f, 0.f, 0.f);
vec3 positionLamp = vec3(-1.0f, 1.5f, -1.0f);*/

/*vec3 scaleDirectional = vec3(0.1f, 0.1f, 0.1f);
vec3 rotationDirectional = vec3(0.f, 0.f, 0.f);
vec3 positionDirectional = vec3(-1.0f, 1.5f, -1.0f);*/

vec3 scalePoint = vec3(0.1f, 0.1f, 0.1f);
vec3 rotationPoint= vec3(0.f, 0.f, 0.f);
vec3 positionPoint= vec3(-1.0f, 1.5f, -1.0f);

vec3 scalePoint2 = vec3(0.1f, 0.1f, 0.1f);
vec3 rotationPoint2 = vec3(0.f, 0.f, 0.f);
vec3 positionPoint2 = vec3(-1.0f, -1.5f, -1.0f);

vec3 scaleSpot= vec3(0.1f, 0.1f, 0.1f);
vec3 rotationSpot = vec3(0.f, 0.f, 0.f);
vec3 positionSpot = vec3(3.0f, 1.5f, -1.0f);

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
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) { camera.ProcessMouseScroll(yoffset/8); }

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
	Shader lightingShader = Shader("./src/multipleLightVertexShader.vertexshader", "./src/multipleLightFragmentShader.fragmentshader");
	Shader PointShader = Shader("./src/lampVertexShader.vertexshader", "./src/lampFragmentShader.fragmentshader");
	//Shader PointShader2 = Shader("./src/lampVertexShader.vertexshader", "./src/lampFragmentShader.fragmentshader");
	Shader SpotShader = Shader("./src/lampVertexShader.vertexshader", "./src/lampFragmentShader.fragmentshader");
	Object Cube = Object(scaleCube, rotationCube, positionCube, Object::cube);
	//Object Lamp = Object(scaleLamp, rotationLamp, positionLamp, Object::cube);
	//Object DirectionalLight = Object(scaleDirectional, rotationDirectional, positionDirectional, Object::cube); no representem amb cub a les directional lights
	Object PointLight = Object(scalePoint, rotationPoint, positionPoint, Object::cube);
	//Object PointLight2 = Object(scalePoint2, rotationPoint2, positionPoint2, Object::cube);
	//Object SpotLight = Object(scaleSpot, rotationSpot, positionSpot, Object::cube);

	while (!glfwWindowShouldClose(window)){

		//Deltatime calculation for each frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Events checker
		glfwPollEvents();
		do_movement();
		if (controls == 1) { Cube.do_movement(window); }
		else { 
			//Lamp.do_movement(window); 
			PointLight.do_movement(window);
		}

		//Background color and z-buffer corrector
		glClearColor(0.f, 0.f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Getting uniform variables from the shaders and declaring cube attributes
		lightingShader.USE(); //Usage of the shader

		//Light and camera position initialization
		
		//vec3 lightPos = Lamp.GetPosition();

		vec3 camPosition = camera.Position;

		//Variables and uniform from shaders
		GLint objectColorLoc = glGetUniformLocation(lightingShader.Program, "objectColor");
		GLint lightColorLoc = glGetUniformLocation(lightingShader.Program, "lightColor");
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(objectColorLoc, 1.0f, 0.8f, 0.6f);
		glUniform3f(lightColorLoc, 0.8f, 0.2f, 0.2f);
		glUniform3f(viewPosLoc, camPosition.x, camPosition.y, camPosition.z);

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		
		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);

		// Point light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), PointLight.GetPosition().x, PointLight.GetPosition().y, PointLight.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032);

		/* Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), PointLight2.GetPosition().x, PointLight2.GetPosition().y, PointLight2.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032);*/
		
		
		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camPosition.x, camPosition.y, camPosition.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.Front.x, camera.Front.y, camera.Front.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

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

		//Drawing the point light 1
		PointShader.USE();

		//Variables and uniform from shaders
		modelLoc = glGetUniformLocation(PointShader.Program, "model");
		viewLoc = glGetUniformLocation(PointShader.Program, "view");
		projLoc = glGetUniformLocation(PointShader.Program, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(Lamp.GetModelMatrix()));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(PointLight.GetModelMatrix()));

		//Lamp.Draw();
		//DirectionalLight.Draw();
		PointLight.Draw();

		//Drawing the point light 2
		/*PointShader2.USE();

		//Variables and uniform from shaders
		modelLoc = glGetUniformLocation(PointShader2.Program, "model");
		viewLoc = glGetUniformLocation(PointShader2.Program, "view");
		projLoc = glGetUniformLocation(PointShader2.Program, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(Lamp.GetModelMatrix()));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(PointLight2.GetModelMatrix()));

		//Lamp.Draw();
		//DirectionalLight.Draw();
		PointLight2.Draw();*/

		//Drawing the spot light
		/*SpotShader.USE();
		modelLoc = glGetUniformLocation(SpotShader.Program, "model");
		viewLoc = glGetUniformLocation(SpotShader.Program, "view");
		projLoc = glGetUniformLocation(SpotShader.Program, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));
	
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(SpotLight.GetModelMatrix()));

		SpotLight.Draw();*/

		//Movement checkers
		if (controls == 1) { Cube.check_movement(deltaTime); }
		else { 
			//Lamp.check_movement(deltaTime); 
			PointLight.check_movement(deltaTime);
		}

		//Swap the screen buffers
		glfwSwapBuffers(window);
	}

	//Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}
	

