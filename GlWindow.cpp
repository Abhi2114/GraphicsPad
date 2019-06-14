#include <GL/glew.h>
#include "GlWindow.h"
#include <cassert>
#include <fstream>
#include <iostream>

const GLuint numTriangles = 2;

// vertex position
struct Position {

	GLfloat x, y, z;

	Position(): x(0.0f), y(0.0f), z(0.0f) {}
	Position(GLfloat x, GLfloat y, GLfloat z) : x(x), y(y), z(z) {}
};

// vertex colors
struct Color {

	GLfloat r, g, b;

	Color() : r(0.0f), g(0.0f), b(0.0f) {}
	Color(GLfloat r, GLfloat g, GLfloat b): r(r), g(g), b(b) {}
};

// number of attributes in every struct
const GLint POSITION_SIZE = sizeof(Position) / sizeof(GLfloat);
const GLint COLOR_SIZE = sizeof(Color) / sizeof(GLfloat);

// size is the number of unique vertices that we will draw
// num is the total number of vertices, so for numTriangles = 3, num = 3*3 = 9
void GlWindow::sendDataToOpenGL(Position *positions, GLsizeiptr size, GLuint num) {

	Color* colors = new Color[size];
	for (GLsizeiptr i = 0; i < size; ++i) {
		colors[i] = Color(1.0f, 0.9f, 0.0f);
	}

	// reserve some space for the positions buffer on the graphics card
	glGenBuffers(1, &vertexPositionBufferId);
	// read in the positions information
	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBufferId);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(Position), positions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, POSITION_SIZE, GL_FLOAT, GL_FALSE, 0, 0);

	// reserve some space for the colors buffer on the graphics card
	glGenBuffers(1, &vertexColorBufferId);
	// read in the colors information
	glBindBuffer(GL_ARRAY_BUFFER, vertexColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(Color), colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, COLOR_SIZE, GL_FLOAT, GL_FALSE, 0, 0);

	// specify indices of the vertex array to draw
	glGenBuffers(1, &indexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);

	// generate the indices
	GLubyte *indices = new GLubyte[num];
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	
	for (GLuint i = 3; i < num; ++i) {
		if (i % 3 == 0)
			indices[i] = indices[i - 1];
		else
			indices[i] = indices[i - 1] + 1;
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, num * sizeof(GLbyte), indices, GL_STATIC_DRAW);

	delete[] colors;
	delete[] indices;
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
	// set the depth value of the two triangles
	const GLfloat DEPTH = 0.0f;

	GLfloat base = 2 / (GLfloat)numTriangles;

	Position* positions = new Position[3];  // represents one triangle
	Position* all_positions = new Position[2 * numTriangles + 1];  // represents all triangles, some positions will be re-used
	GLuint a = 0;  // index for all positions

	positions[2] = Position(-1.0f, 1.0f, DEPTH);
	for (GLuint i = 0; i < numTriangles; ++i) {

		positions[0] = positions[2];
		positions[1] = Position(-1.0f + i * base, 0.0f, DEPTH);
		positions[2] = Position(-1.0f + (i + 1) * base, 1.0f, DEPTH);

		if (i == 0) {
			// push all 3 in
			all_positions[a++] = positions[0]; all_positions[a++] = positions[1]; all_positions[a++] = positions[2];
		}
		else {
			// push only 1 and 2 in
			all_positions[a++] = positions[1]; all_positions[a++] = positions[2];
		}
	}

	sendDataToOpenGL(all_positions, 2 * numTriangles + 1, 3 * numTriangles);

	delete[] positions;
	delete[] all_positions;

	installShaders();
}

void GlWindow::paintGL()
{	
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width(), height());
	glDrawElements(GL_TRIANGLES, 3 * numTriangles, GL_UNSIGNED_BYTE, 0);
}
