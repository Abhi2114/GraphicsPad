#pragma once

#include <QtWidgets/qopenglwidget.h>
#include <string>

typedef void (*lengthHandler)(GLuint, GLenum, GLint*);
typedef void (*infoLogHandler)(GLuint, GLsizei, GLsizei*, GLchar*);

struct Position;

class GlWindow : public QOpenGLWidget
{	
	Q_OBJECT;

	GLuint vertexPositionBufferId;
	GLuint vertexColorBufferId;
	GLuint indexBufferId;

	void sendDataToOpenGL(Position*, GLsizeiptr, GLuint);
	void installShaders();
	std::string readShaderCode(const GLchar*);

	GLint getShaderCompileStatus(GLuint& shaderId);
	GLint getShaderLinkStatus(GLuint& shaderId);
	void showShaderErrorLog(GLuint& shaderId, lengthHandler, infoLogHandler);
protected:
	void initializeGL();
	void paintGL();
};
