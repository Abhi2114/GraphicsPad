#version 330

in layout(location=0) vec3 position;
in layout(location=1) vec3 color;
in layout(location=2) mat4 fullTransformMatrix;

uniform vec3 ambientLight;

out vec3 vertexColor;

void main()
{
	vec4 v = vec4(position, 1.0);
	gl_Position = fullTransformMatrix * v;
	vertexColor = color * ambientLight;
}