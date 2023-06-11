#version 140
in vec3 UV;

out vec4 LFragment;
uniform sampler2DArray ourtexture;
void main() { 
	LFragment = texture(ourtexture, UV);

	//LFragment = vec4(UV,1);
}