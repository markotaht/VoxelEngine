#version 140 core

in vec3 VertexPosition;
in vec3 VertexColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 color;

void main() {
	color = VertexColor;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(VertexPosition, 1 );
}
