#pragma once

#include <QtWidgets/qopenglwidget.h>
#include <string>
#include "Camera.h"

typedef void (*lengthHandler)(GLuint, GLenum, GLint*);
typedef void (*infoLogHandler)(GLuint, GLsizei, GLsizei*, GLchar*);

class GlWindow : public QOpenGLWidget
{
	Q_OBJECT;

	GLuint vertexBufferId;
	GLuint vertexColorBufferId;
	GLuint vertexTranslateBufferId;
	GLuint indexBufferId;

	GLuint VAO[2];

	Camera camera;

	void sendDataToOpenGL();
	void setUpMatrices();
	void installShaders();
	std::string readShaderCode(const GLchar*);

	GLint getShaderCompileStatus(GLuint& shaderId);
	GLint getShaderLinkStatus(GLuint& shaderId);
	void showShaderErrorLog(GLuint& shaderId, lengthHandler, infoLogHandler);

protected:
	void initializeGL() override;
	void paintGL() override;
	void mouseMoveEvent(QMouseEvent*) override;
	void keyPressEvent(QKeyEvent*) override;

public:
	~GlWindow();
};
