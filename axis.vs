#version 330 core

layout(location = 0) in vec3 vposition;
layout(location = 3) in vec3 vcolor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 color;

void main() {
	color = vcolor;
	gl_Position = projection * view * model * vec4(vposition, 1.0);
}
