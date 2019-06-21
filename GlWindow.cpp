#include <GL/glew.h>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>
#include <cassert>
#include <fstream>
#include <iostream>
#include <Qt3DInput/qmouseevent.h>
#include "GlWindow.h"
#include "Primitives/Vertex.h"
#include "Primitives/ShapeGenerator.h"

using glm::mat4;
using glm::mat3;
using glm::vec4;

GLuint programId;
GLubyte numIndices;
GLuint numCubes = 2;

void GlWindow::sendDataToOpenGL() {

	ShapeData* cube = ShapeGenerator::makeCube();

	// reserve some space for the positions buffer on the graphics card
	glGenBuffers(1, &vertexPositionBufferId);
	// read in the positions information
	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBufferId);
	glBufferData(GL_ARRAY_BUFFER, cube->positionBufferSize(), cube->positionData(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// reserve some space for the colors buffer on the graphics card
	glGenBuffers(1, &vertexColorBufferId);
	// read in the colors information
	glBindBuffer(GL_ARRAY_BUFFER, vertexColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, cube->colorBufferSize(), cube->colorData(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &vertexTranslateBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexTranslateBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mat4) * numCubes, 0, GL_DYNAMIC_DRAW);
	// send in the data in groups of 4
	for (GLuint i = 0; i < 4; ++i) {
		glVertexAttribPointer(i + 2, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(GLfloat) * i * 4));
		glEnableVertexAttribArray(i + 2);
		glVertexAttribDivisor(i + 2, 1);
	}

	// specify indices of the vertex array to draw
	glGenBuffers(1, &indexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube->indexBufferSize(), cube->indexData(), GL_STATIC_DRAW);

	numIndices = cube->getNumIndices();
	delete cube;  // calls the destructor defined in ShapeData
}

void GlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	// prepare matrices to send to the vertex shader
	mat4 projectionMatrix = glm::perspective(glm::radians(80.0f), float(width()) / height(), 0.1f, 10.0f);

	mat4 translates[] = {
			projectionMatrix * camera.getWorldToViewMatrix() * glm::translate(vec3(-1.0f, 0.0f, -3.0f)) * glm::rotate(glm::radians(36.0f), vec3(1.0f, 0.0f, 0.0f)),
			projectionMatrix * camera.getWorldToViewMatrix() * glm::translate(vec3(1.0f, 0.0f, -3.75f)) * glm::rotate(glm::radians(126.0f), vec3(0.0f, 1.0f, 0.0f)),
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(translates), translates, GL_DYNAMIC_DRAW);

	glDrawElementsInstanced(GL_TRIANGLES, numIndices, GL_UNSIGNED_BYTE, 0, numCubes);
}

void GlWindow::mouseMoveEvent(QMouseEvent* e) {

	glm::vec2 newPosition = glm::vec2(e->x(), e->y());
	camera.mouseUpdate(newPosition);
	repaint();
}

std::string GlWindow::readShaderCode(const GLchar * fileName) {

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
	programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);

	// delete the obj files for the shaders once we have created the program
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	// check if the program linked correctly or not
	GLint linkStatus = getShaderLinkStatus(programId);
	if (linkStatus == GL_FALSE) {
		showShaderErrorLog(programId, glGetProgramiv, glGetProgramInfoLog);
		return;
	}

	glUseProgram(programId);
}

GLint GlWindow::getShaderCompileStatus(GLuint & shaderId)
{
	GLint compileStatus;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);
	return compileStatus;
}

GLint GlWindow::getShaderLinkStatus(GLuint & shaderId)
{
	GLint linkStatus;
	glGetProgramiv(shaderId, GL_LINK_STATUS, &linkStatus);
	return linkStatus;
}

void GlWindow::showShaderErrorLog(GLuint & shaderId,
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
	setMouseTracking(true);

	GLenum error = glewInit();
	assert(error == 0);

	glEnable(GL_DEPTH_TEST);  // enable depth test
	sendDataToOpenGL();
	installShaders();
}

GlWindow::~GlWindow() {
	// delete the shader program
	glUseProgram(0);
	glDeleteProgram(programId);
}