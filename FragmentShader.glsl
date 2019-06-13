#version 330

out vec4 daColor;
in vec3 vertexColor;

void main()
{
	daColor = vec4(vertexColor, 1.0);
}