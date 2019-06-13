#include <GL/glew.h>
#include "GlWindow.h"
#include <cassert>
#include <fstream>
#include <iostream>

// vertex position
struct Position {

	GLfloat x, y, z;

	Position(GLfloat x, GLfloat y, GLfloat z) : x(x), y(y), z(z) {}
};

// vertex colors
struct Color {

	GLfloat r, g, b;

	Color(GLfloat r, GLfloat g, GLfloat b): r(r), g(g), b(b) {}
};

// number of attributes in every struct
const GLint POSITION_SIZE = sizeof(Position) / sizeof(GLfloat);
const GLint COLOR_SIZE = sizeof(Color) / sizeof(GLfloat);

void GlWindow::sendDataToOpenGL() {

	// set the depth value of the two triangles
	const GLfloat RED_DEPTH = 0.5f;
	const GLfloat BLUE_DEPTH = -0.5f;

	// set the vertices of the triangle and also their colors
	Position positions[] =
	{	
		Position(0.0f, 1.0f, -1.0f),
		Position(-1.0f, -1.0f, RED_DEPTH),
		Position(1.0f, -1.0f, RED_DEPTH),
		Position(1.0f, 1.0f, BLUE_DEPTH),
		Position(0.0f, 1.0f, BLUE_DEPTH),
		Position(0.0f, -1.0f, BLUE_DEPTH)
	};

	Color colors[] =
	{
		Color(0.0f, 0.0f, 1.0f),
		Color(1.0f, 0.0f, 0.0f),
		Color(1.0f, 0.0f, 0.0f),
		Color(0.0f, 0.0f, 1.0f),
		Color(0.0f, 0.0f, 1.0f),
		Color(0.0f, 0.0f, 1.0f)
	};

	// reserve some space for the positions buffer on the graphics card
	glGenBuffers(1, &vertexPositionBufferId);
	// read in the positions information
	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, POSITION_SIZE, GL_FLOAT, GL_FALSE, 0, 0);

	// reserve some space for the colors buffer on the graphics card
	glGenBuffers(1, &vertexColorBufferId);
	// read in the colors information
	glBindBuffer(GL_ARRAY_BUFFER, vertexColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, COLOR_SIZE, GL_FLOAT, GL_FALSE, 0, 0);

	// specify indices of the vertex array to draw
	glGenBuffers(1, &indexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);

	GLubyte indices[] = { 0,1,2, 3,4,5 };

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

std::string GlWindow::readShaderCode(const GLchar* fileName) {
	
	std::ifstream file(fileName);
	std::string content = "";

	if (file.is_open()) {

		// read in the contents of the file into content
		content = std::string(std::istreambuf_iterator<char>(file), 
							  std::istreambuf_iterator<char>());
		file.close();
	}
	return content;
}

void GlWindow::installShaders() {

	// initialize shader ids 
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	// set up the vertex shader
	std::string temp = readShaderCode("VertexShader.glsl");
	const GLchar* adapter[] = { temp.c_str() };
	glShaderSource(vertexShaderId, 1, adapter, 0);

	// now set up the fragment shader
	temp = readShaderCode("FragmentShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderId, 1, adapter, 0);

	// now we will compile the shader programs
	glCompileShader(vertexShaderId);
	glCompileShader(fragmentShaderId);

	// check status of the compilation
	GLint compileStatus = getShaderCompileStatus(vertexShaderId);
	if (compileStatus == GL_FALSE) {
		showShaderErrorLog(vertexShaderId, glGetShaderiv, glGetShaderInfoLog);
		return;
	}
	// status of compilation for the fragment shader
	compileStatus = getShaderCompileStatus(fragmentShaderId);
	if (compileStatus == GL_FALSE) {
		showShaderErrorLog(fragmentShaderId, glGetShaderiv, glGetShaderInfoLog);
		return;
	}

	// link the program
	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);

	// check if the program linked correctly or not
	GLint linkStatus = getShaderLinkStatus(programId);
	if (linkStatus == GL_FALSE) {
		showShaderErrorLog(programId, glGetProgramiv, glGetProgramInfoLog);
		return;
	}

	glUseProgram(programId);
}

GLint GlWindow::getShaderCompileStatus(GLuint &shaderId)
{
	GLint compileStatus;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);
	return compileStatus;
}

GLint GlWindow::getShaderLinkStatus(GLuint& shaderId)
{
	GLint linkStatus;
	glGetProgramiv(shaderId, GL_LINK_STATUS, &linkStatus);
	return linkStatus;
}

void GlWindow::showShaderErrorLog(GLuint& shaderId, 
	lengthHandler lengthHandler, 
	infoLogHandler logHandler)
{
	// if there was an error, get the length of the error message
	GLint infoLogLength;
	lengthHandler(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
	
	// write the error message into a buffer
	GLchar* buffer = new GLchar[infoLogLength];

	GLsizei bufferSize;
	logHandler(shaderId, infoLogLength, &bufferSize, buffer);
	
	// print the error message
	std::cout << buffer << std::endl;

	delete[] buffer;
}

void GlWindow::initializeGL()
{	
	GLenum error = glewInit();
	assert(error == 0);

	glEnable(GL_DEPTH_TEST);  // enable depth test
	sendDataToOpenGL();
	installShaders();
}

void GlWindow::paintGL()
{	
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width(), height());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
}
