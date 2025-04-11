#version 330 core
in vec3 UV;

out vec4 LFragment;
uniform sampler2DArray atlas;
void main() { 
	LFragment = texture(atlas, UV);
}