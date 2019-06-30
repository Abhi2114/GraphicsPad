#version 330

in layout(location=0) vec4 position;
in layout(location=1) vec3 color;
in layout(location=2) vec3 normal;
in layout(location=3) mat4 fullTransformMatrix;

uniform vec3 lightPosition;
uniform mat4 modelToWorldMatrix;

out vec3 vertexNormal;
out vec3 vertexPosition;

void main()
{
	gl_Position = fullTransformMatrix * position;

	vertexNormal = vec3(normalize(modelToWorldMatrix * vec4(normal, 0.0f)));

	vertexPosition = vec3(modelToWorldMatrix * position);
}