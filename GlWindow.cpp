#include <GL/glew.h>
#include "GlWindow.h"
#include <cassert>
#include <fstream>
#include <iostream>

const GLfloat X_DELTA = 0.1f;
GLuint numTriangles = 0;
const GLuint MAX_TRIANGLES = 5;

void GlWindow::sendDataToOpenGL() {

	// reserve some space for the positions buffer on the graphics card
	glGenBuffers(1, &vertexPositionBufferId);
	// read in the positions information
	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBufferId);
	glBufferData(GL_ARRAY_BUFFER, MAX_TRIANGLES * 72, NULL, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);

}

void GlWindow::sendTriangleToGl() {

	const GLfloat THIS_TRI_X = -1.0f + numTriangles * X_DELTA;

	// store position and color information of the vertex
	GLfloat triangle[] = {

		THIS_TRI_X, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		THIS_TRI_X + X_DELTA, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		THIS_TRI_X, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f
	};

	glBufferSubData(GL_ARRAY_BUFFER, numTriangles * sizeof(triangle), sizeof(triangle), triangle);
	numTriangles++;
}

void GlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());
	sendTriangleToGl();
	// glDrawArrays(GL_TRIANGLES, (numTriangles - 1) * 3, 3);
	glDrawArrays(GL_TRIANGLES, 0, numTriangles * 3);
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

GLint GlWindow::getShaderCompileStatus(GLuint& shaderId)
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
