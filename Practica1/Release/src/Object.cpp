#include "Object.h"

//Object class constructor
Object::Object(vec3 scaleObject, vec3 rotationObject, vec3 positionObject, FigureType typef){
	
	GLfloat VertexBufferObject[]= {
		//Front side
		 1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  -1.0f, 
		 1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  -1.0f,
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  -1.0f,
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  -1.0f, 
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  -1.0f, 
		 1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  -1.0f, 
		//Back side
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 

		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		//Left side
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
		//Right side
		 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
		 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
		 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 

		 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
		 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
		 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
		//Down side
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 
		 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
		 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
		 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
		-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
		//Up side
		 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f
	};

	//VAO, VBO and EBO creation for the new object
	glGenVertexArrays(1, &VAO); 
	glGenBuffers(1, &VBO); 
	glGenBuffers(1, &EBO); 

	//Binding of the VAO and VBO
	glBindVertexArray(VAO); {
		glBindBuffer(GL_ARRAY_BUFFER, VBO); 
		switch (typef) { //Initialization of the vertex buffer, allocation of memory and data filling
			case Object::cube:
			glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferObject), &VertexBufferObject[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); //Vertex positions
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); //Vertex normals
			break;
		default:
			break;
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	glBindVertexArray(0);

	//Variable initialization and declaration from the constructor values
	position = positionObject;
	rotation = rotationObject;
	scale = scaleObject;
	moveUp = false;
	moveDown = false;
	moveRight = false;
	moveLeft = false;
	rotateUp = false;
	rotateDown = false;
	rotateRight = false;
	rotateLeft = false;
	rotationSum = 45.f;
}

//Object deconstructor
Object::~Object(){} 

//Object drawing
void Object::Draw(){

	glBindVertexArray(VAO); //Binded VAO
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Polygon mode set to fill
	glDrawArrays(GL_TRIANGLES, 0, 36); //Draw the arrays (No EBO needed)
	glBindVertexArray(0); //Bind to 0 (release)

}

//Object movement
void Object::Move(vec3 translationObject){position += translationObject;}

//Object rotation
void Object::Rotate(vec3 rotationObject) { rotation += rotationObject;}

//Object scalar
void Object::Scale(vec3 scaleObject){scale = scaleObject;}

//Get the model matrix of the object
mat4 Object::GetModelMatrix() {

	if (rotation.x >= 360)rotation.x = glm::mod(rotation.x, 360.f);

	if (rotation.x <= (-360))rotation.x = glm::mod(rotation.x, (-360.f));

	if (rotation.y >= 360)rotation.y = glm::mod(rotation.y, 360.f);

	if (rotation.y <= (-360))rotation.y = glm::mod(rotation.y, (-360.f));

	mat4 matrixModel;

	matrixModel = glm::translate(matrixModel, GetPosition());

	matrixModel = glm::rotate(matrixModel, glm::radians(rotation.x), vec3(1.0, 0.0, 0.0));

	matrixModel = glm::rotate(matrixModel, glm::radians(rotation.y), vec3(0.0, 1.0, 0.0));

	matrixModel = glm::scale(matrixModel, scale);

	return matrixModel;
}

//Function that returns the position of the object
vec3 Object::GetPosition() { return position; }

//Object and it's vertex and buffer arrays deleting
void Object::Delete(){

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

//Object movement and its correct varaible changes
void Object::check_movement(float deltaTime) {

	if (rotateUp) { Rotate(vec3(-rotationSum * deltaTime, 0.0f, 0.0f)); }

	if (rotateDown) { Rotate(vec3(rotationSum * deltaTime, 0.0f, 0.0f)); }

	if (rotateLeft) { Rotate(vec3(0.0f, -rotationSum * deltaTime, 0.0f)); }

	if (rotateRight) { Rotate(vec3(0.0f, rotationSum * deltaTime, 0.0f)); }

	if (moveUp) {Move(vec3(0.0f, 1.f * deltaTime, 0.0f));}

	if (moveDown) {Move(vec3(0.0f, -1.f * deltaTime, 0.0f));}
	
	if (moveLeft) {Move(vec3(-1.0f * deltaTime, 0.0f, 0.0f));}
	
	if (moveRight) {Move(vec3(1.0f * deltaTime, 0.0f, 0.0f));}

}

//Key detection for correct movement application to the object
void Object::do_movement(GLFWwindow *window) {

	moveUp = glfwGetKey(window, GLFW_KEY_UP);

	moveDown = glfwGetKey(window, GLFW_KEY_DOWN);
	
	moveLeft = glfwGetKey(window, GLFW_KEY_LEFT);
	
	moveRight = glfwGetKey(window, GLFW_KEY_RIGHT);
	
	rotateUp = glfwGetKey(window, GLFW_KEY_KP_8);
	
	rotateDown = glfwGetKey(window, GLFW_KEY_KP_2);
	
	rotateLeft = glfwGetKey(window, GLFW_KEY_KP_4);
	
	rotateRight = glfwGetKey(window, GLFW_KEY_KP_6);

}