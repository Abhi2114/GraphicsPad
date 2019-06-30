#version 330

out vec4 daColor;

uniform vec3 lightPosition;
uniform vec4 ambientLight;

in vec3 vertexNormal;
in vec3 vertexPosition;

void main()
{	
	vec3 light = normalize(lightPosition - vertexPosition);
	float brightness = dot(light, vertexNormal);
	vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0);
	daColor = clamp(diffuseLight, 0, 1) + ambientLight;
}