#version 140

in vec3 VertexPosition;
in vec3 VertexUV;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;

out vec3 UV;

void main() {
	UV = VertexUV;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(VertexPosition, 1 );
}
