#include <GL/glew.h>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>
#include <cassert>
#include <fstream>
#include <iostream>
#include <Qt3DInput/qmouseevent.h>
#include <Qt3DInput/qkeyevent.h>
#include "GlWindow.h"
#include "Primitives/Vertex.h"
#include "Primitives/ShapeGenerator.h"

using glm::mat4;
using glm::mat3;
using glm::vec4;

GLuint programId;
GLuint numShape1Indices;
GLuint numShape2Indices;
GLuint numShape1 = 1;
GLuint numShape2 = 1;
GLsizeiptr shape1Offset;

void GlWindow::sendDataToOpenGL() {

	ShapeData* shape1 = ShapeGenerator::makePlane();
	ShapeData* shape2 = ShapeGenerator::makeTeapot(30);

	// reserve some space for the positions buffer on the graphics card
	glGenBuffers(1, &vertexPositionBufferId);
	// read in the positions information
	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBufferId);
	glBufferData(GL_ARRAY_BUFFER, shape1->positionBufferSize() + shape2->positionBufferSize(), 0, GL_STATIC_DRAW);

	// reserve some space for the colors buffer on the graphics card
	glGenBuffers(1, &vertexColorBufferId);
	// read in the colors information
	glBindBuffer(GL_ARRAY_BUFFER, vertexColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, shape1->colorBufferSize() + shape2->colorBufferSize(), 0, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vertexTranslateBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mat4) * (numShape1 + numShape2), 0, GL_DYNAMIC_DRAW);

	// specify indices of the vertex array to draw
	glGenBuffers(1, &indexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape1->indexBufferSize() + shape2->indexBufferSize(), 0, GL_STATIC_DRAW);

	numShape1Indices = shape1->getNumIndices();
	numShape2Indices = shape2->getNumIndices();

	glGenVertexArrays(2, VAO);

	// bind the shape1
	glBindVertexArray(VAO[0]);

	// position
	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBufferId);
	glBufferSubData(GL_ARRAY_BUFFER, 0, shape1->positionBufferSize(), shape1->positionData());
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// color
	glBindBuffer(GL_ARRAY_BUFFER, vertexColorBufferId);
	glBufferSubData(GL_ARRAY_BUFFER, 0, shape1->colorBufferSize(), shape1->colorData());
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// translate
	glBindBuffer(GL_ARRAY_BUFFER, vertexTranslateBufferId);
	for (GLuint i = 0; i < 4; ++i) {
		glEnableVertexAttribArray(i + 2);
		glVertexAttribPointer(i + 2, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), reinterpret_cast<void*>(sizeof(GLfloat) * i * 4));
		glVertexAttribDivisor(i + 2, 1);
	}

	// index
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, shape1->indexBufferSize(), shape1->indexData());

	// bind the shape2
	glBindVertexArray(VAO[1]);

	// position
	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBufferId);
	glBufferSubData(GL_ARRAY_BUFFER, shape1->positionBufferSize(), shape2->positionBufferSize(), shape2->positionData());
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(shape1->positionBufferSize()));

	// color
	glBindBuffer(GL_ARRAY_BUFFER, vertexColorBufferId);
	glBufferSubData(GL_ARRAY_BUFFER, shape1->colorBufferSize(), shape2->colorBufferSize(), shape2->colorData());
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(shape1->colorBufferSize()));

	// translate
	glBindBuffer(GL_ARRAY_BUFFER, vertexTranslateBufferId);
	for (GLuint i = 0; i < 4; ++i) {
		glEnableVertexAttribArray(i + 2);
		glVertexAttribPointer(i + 2, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), reinterpret_cast<void*>(sizeof(mat4) * numShape1 + (sizeof(GLfloat) * i * 4)));
		glVertexAttribDivisor(i + 2, 1);
	}

	// index
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, shape1->indexBufferSize(), shape2->indexBufferSize(), shape2->indexData());

	shape1Offset = shape1->indexBufferSize();

	delete shape1;  // calls the destructor defined in ShapeData
	delete shape2;
}

void GlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	// prepare matrices to send to the vertex shader
	mat4 projectionMatrix = glm::perspective(glm::radians(80.0f), float(width()) / height(), 0.1f, 20.0f);
	mat4 worldToProjectionMatrix = projectionMatrix * camera.getWorldToViewMatrix();

	// translations for shape 1
	mat4 translateShape1[] = {
		worldToProjectionMatrix * glm::translate(vec3(0.0f, 0.0f, -3.0f)) * glm::rotate(glm::radians(26.0f), vec3(0.0f, 1.0f, 0.0f)),
	};

	// translations for shape 2
	mat4 translateShape2[] = {
		worldToProjectionMatrix * glm::translate(vec3(0.0f, 0.0f, -3.0f)) * glm::rotate(glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f)),
	};

	GLint ambientLightUniformLocation = glGetUniformLocation(programId, "ambientLight");
	vec3 ambientLight(0.1f, 0.9f, 1.0f);
	glUniform3f(ambientLightUniformLocation, ambientLight.x, ambientLight.y, ambientLight.z);

	// Shape 1
	glBindVertexArray(VAO[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(translateShape1), translateShape1);
	glDrawElementsInstanced(GL_TRIANGLES, numShape1Indices, GL_UNSIGNED_SHORT, 0, numShape1);

	// Shape 2
	glBindVertexArray(VAO[1]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(translateShape1), sizeof(translateShape2), translateShape2);
	glDrawElementsInstanced(GL_TRIANGLES, numShape2Indices, GL_UNSIGNED_SHORT, reinterpret_cast<void*>(shape1Offset), numShape2);
}

void GlWindow::mouseMoveEvent(QMouseEvent * e) {

	glm::vec2 newPosition = glm::vec2(e->x(), e->y());
	camera.mouseUpdate(newPosition);
	repaint();
}

void GlWindow::keyPressEvent(QKeyEvent * e) {

	bool shouldRepaint = true;

	switch (e->key()) {

	case Qt::Key::Key_W:
		camera.moveForward();
		break;
	case Qt::Key::Key_S:
		camera.moveBackward();
		break;
	case Qt::Key::Key_A:
		camera.strafeLeft();
		break;
	case Qt::Key::Key_D:
		camera.strafeRight();
		break;
	case Qt::Key::Key_R:
		camera.moveUp();
		break;
	case Qt::Key::Key_F:
		camera.moveDown();
		break;
	default:
		shouldRepaint = false;
		break;
	}

	if (shouldRepaint)
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
	glEnable(GL_CULL_FACE);
	sendDataToOpenGL();
	installShaders();
}

GlWindow::~GlWindow() {

	// delete the VAOs
	glDeleteVertexArrays(1, VAO);

	// delete all the buffers when done
	glDeleteBuffers(1, &vertexPositionBufferId);
	glDeleteBuffers(1, &vertexColorBufferId);
	glDeleteBuffers(1, &vertexTranslateBufferId);
	glDeleteBuffers(1, &indexBufferId);

	// delete the shader program
	glUseProgram(0);
	glDeleteProgram(programId);
}