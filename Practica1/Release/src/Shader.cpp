#include "Shader.h"

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {

	//Declaring variables
	std::string vertexCode;
	std::string fragmentCode;
	std::cout << vertexPath;
	std::cout << fragmentPath;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	//Ensuring ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);

	try{
		//Open the vertex and fragment files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		//Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		//Close file handlers
		vShaderFile.close();
		fShaderFile.close();
		//Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e){std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;} //Error if it can't read it

	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar * fShaderCode = fragmentCode.c_str();

	//Variable declaration for vertex and fragment shader
	GLuint vertexShader, fragmentShader;
	GLint success;
	GLchar infoLog[512];
	
	//Compiling the vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);

	//Detection and printing of errors for the vertex shader
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success){
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Compiling the fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);

	//Detection and printing of errors for the fragment shader
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Creation of the Shader Program
	this->Program = glCreateProgram();
	glAttachShader(this->Program, vertexShader); //Attaching shaders to the program
	glAttachShader(this->Program, fragmentShader);
	glLinkProgram(this->Program);

	//Print any linking errors
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success){
		glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//Delete the shaders we won't need
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

//Shader class function that enables the shader program
void Shader::USE() {glUseProgram(Program);}




