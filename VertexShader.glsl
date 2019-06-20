#version 330

in layout(location=0) vec2 position;
in layout(location=1) float offset;

out vec3 vertexColor;

void main()
{
	gl_Position = vec4(position.x + offset, position.y, 0.0, 1.0);
	vertexColor = vec3(0.0f, 1.0f, 0.0f);
}