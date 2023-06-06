#version 140

in vec3 VertexPosition;
in vec3 VertexNormal;
in vec2 VertexUV;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;

out vec3 norm;
out vec3 fragPos;
out vec2 UV;

void main() {
	UV = VertexUV;
	norm = mat3(normalMatrix) * VertexNormal;
	fragPos = vec3(modelMatrix * vec4(VertexPosition, 1));
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(VertexPosition, 1 );
}
